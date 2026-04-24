#include "Character/EMBERVEILCharacterBase.h"
#include "GameplayEffectExtension.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameplayTagsModule.h"
#include "UI/Widgets/WG_DamageNumber.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

AEMBERVEILCharacterBase::AEMBERVEILCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Todos los personajes usan capsule + mesh estándar de UE5
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Configuración de movimiento base — se sobreescribe por raza/clase vía GAS
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

UAbilitySystemComponent* AEMBERVEILCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEMBERVEILCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	BindGASCallbacks();
}

void AEMBERVEILCharacterBase::ApplyDefaultAttributes() const
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

void AEMBERVEILCharacterBase::GrantDefaultAbilities()
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (!AbilityClass) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityClass, GetCharacterLevel());
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void AEMBERVEILCharacterBase::BindGASCallbacks()
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

// ─── ICombatInterface ─────────────────────────────────────────────────────────

int32 AEMBERVEILCharacterBase::GetCharacterLevel_Implementation() const
{
	if (AttributeSet)
		return static_cast<int32>(AttributeSet->GetLevel());
	return 1;
}

FVector AEMBERVEILCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) const
{
	// Determina el socket según el tag (WeaponTip, LeftHand, etc.)
	if (GetMesh())
	{
		const FName SocketName = SocketTag.GetTagName() == FName("CombatSocket.Weapon")
			? WeaponTipSocketName
			: LeftHandSocketName;

		return GetMesh()->GetSocketLocation(SocketName);
	}
	return GetActorLocation();
}

bool AEMBERVEILCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

void AEMBERVEILCharacterBase::Die_Implementation(AActor* InstigatorActor)
{
	if (bDead) return;
	bDead = true;

	// Desactiva colisión de cápsula pero mantiene el mesh visible
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// Animación de muerte
	if (DeathMontage && GetMesh()->GetAnimInstance())
		GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);

	// VFX de muerte (Niagara)
	if (DeathEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, GetActorLocation());

	// Destruct después de X segundos (configurable por subclase)
	SetLifeSpan(5.f);
}

USkeletalMeshComponent* AEMBERVEILCharacterBase::GetCombatMesh_Implementation() const
{
	return GetMesh();
}

