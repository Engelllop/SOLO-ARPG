#include "Character/SOLOCharacterBase.h"
#include "GameplayEffectExtension.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameplayTagsModule.h"
#include "UI/Widgets/WG_DamageNumber.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

ASOLOCharacterBase::ASOLOCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Todos los personajes usan capsule + mesh estÃ¡ndar de UE5
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// ConfiguraciÃ³n de movimiento base â€” se sobreescribe por raza/clase vÃ­a GAS
	GetCharacterMovement()->bOrientRotationToMovement  = true;
	GetCharacterMovement()->RotationRate               = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane          = false;
	GetCharacterMovement()->bSnapToPlaneAtStart        = false;
	GetCharacterMovement()->GravityScale               = 1.75f;
	GetCharacterMovement()->MaxWalkSpeed               = 600.f;
	GetCharacterMovement()->JumpZVelocity              = 700.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationYaw   = false;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* ASOLOCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASOLOCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	BindGASCallbacks();
}

void ASOLOCharacterBase::ApplyDefaultAttributes() const
{
	if (!AbilitySystemComponent || !DefaultAttributesEffect) return;

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	auto ApplyGE = [&](TSubclassOf<UGameplayEffect> GEClass)
	{
		if (!GEClass) return;
		const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(GEClass, GetCharacterLevel(), Context);
		if (Spec.IsValid())
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	};

	ApplyGE(DefaultAttributesEffect);
	for (const TSubclassOf<UGameplayEffect>& GE : SecondaryAttributesEffects)
		ApplyGE(GE);
}

void ASOLOCharacterBase::GrantDefaultAbilities()
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (!AbilityClass) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityClass, GetCharacterLevel());
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void ASOLOCharacterBase::BindGASCallbacks()
{
	if (!AbilitySystemComponent || bGASTagDeathCallbackBound)
	{
		return;
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(
		DeadTag,
		EGameplayTagEventType::NewOrRemoved
	).AddLambda([this](const FGameplayTag Tag, int32 NewCount)
	{
		if (NewCount > 0 && !bDead)
			Die_Implementation(nullptr);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			const float DamageDone = Data.OldValue - Data.NewValue;
			if (DamageDone <= 0.f || !DamageNumberClass) return;

			APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
			if (!PC) return;

			if (UWG_DamageNumber* W = CreateWidget<UWG_DamageNumber>(PC, DamageNumberClass))
			{
				W->AddToViewport();
				W->Activate(DamageDone, EDamageNumberType::Physical, false,
				             GetActorLocation() + FVector(0.f, 0.f, 100.f));
			}
		});

	bGASTagDeathCallbackBound = true;
}

// â”€â”€â”€ ICombatInterface â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

int32 ASOLOCharacterBase::GetCharacterLevel_Implementation() const
{
	if (AttributeSet)
		return static_cast<int32>(AttributeSet->GetLevel());
	return 1;
}

FVector ASOLOCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) const
{
	// Determina el socket segÃºn el tag (WeaponTip, LeftHand, etc.)
	if (GetMesh())
	{
		const FName SocketName = SocketTag.GetTagName() == FName("CombatSocket.Weapon")
			? WeaponTipSocketName
			: LeftHandSocketName;

		return GetMesh()->GetSocketLocation(SocketName);
	}
	return GetActorLocation();
}

bool ASOLOCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

void ASOLOCharacterBase::Die_Implementation(AActor* InstigatorActor)
{
	if (bDead) return;
	bDead = true;

	// Desactiva colisiÃ³n de cÃ¡psula pero mantiene el mesh visible
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// AnimaciÃ³n de muerte
	if (DeathMontage && GetMesh()->GetAnimInstance())
		GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);

	// VFX de muerte (Niagara)
	if (DeathEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, GetActorLocation());

	// Destruct despuÃ©s de X segundos (configurable por subclase)
	SetLifeSpan(5.f);
}

USkeletalMeshComponent* ASOLOCharacterBase::GetCombatMesh_Implementation() const
{
	return GetMesh();
}

