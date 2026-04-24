#include "Character/EMBERVEILPlayerCharacter.h"

#include "Player/EMBERVEILPlayerState.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

AEMBERVEILPlayerCharacter::AEMBERVEILPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// ─── Spring Arm ───────────────────────────────────────────────────────────
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength         = CameraArmLength;
	CameraBoom->bUsePawnControlRotation = true;   // Rota con el controller (mouse/stick)
	CameraBoom->bEnableCameraLag        = true;
	CameraBoom->CameraLagSpeed          = CameraLagSpeed;
	CameraBoom->CameraLagMaxDistance    = 200.f;

	// Offset de hombro derecho — estilo Dark Souls / SAO
	CameraBoom->SocketOffset = FVector(0.f, 80.f, 60.f);

	// ─── Camera ───────────────────────────────────────────────────────────────
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;  // El brazo ya maneja la rotación

	// El personaje rota hacia el movimiento; el controller maneja solo la cámara
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// ─── BeginPlay ────────────────────────────────────────────────────────────────

void AEMBERVEILPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEMBERVEILPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LockOnTarget)
	{
		// Si el target murió o quedó fuera de rango, cancela el lock automáticamente
		if (ICombatInterface::Execute_IsDead(LockOnTarget) ||
			FVector::Dist(GetActorLocation(), LockOnTarget->GetActorLocation()) > LockOnRadius * 1.5f)
		{
			ClearLockOn();
		}
		else
		{
			UpdateLockOnCamera(DeltaSeconds);
		}
	}
}

// ─── GAS init ─────────────────────────────────────────────────────────────────

void AEMBERVEILPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();

	// Solo el servidor aplica atributos y concede habilidades para evitar duplicados
	ApplyDefaultAttributes();
	GrantDefaultAbilities();
}

void AEMBERVEILPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// En clientes el PlayerState llega replicado — misma inicialización
	InitAbilityActorInfo();
}

void AEMBERVEILPlayerCharacter::InitAbilityActorInfo()
{
	AEMBERVEILPlayerState* PS = GetPlayerState<AEMBERVEILPlayerState>();
	if (!ensure(PS)) return;

	// El ASC y el AttributeSet viven en el PlayerState; el Character es el avatar
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSet           = PS->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(PS, this);

	BindGASCallbacks();
}

// ─── ICombatInterface ─────────────────────────────────────────────────────────

int32 AEMBERVEILPlayerCharacter::GetCharacterLevel_Implementation() const
{
	const AEMBERVEILPlayerState* PS = GetPlayerState<AEMBERVEILPlayerState>();
	return PS ? PS->GetPlayerLevel() : 1;
}

// ─── Lock-on ──────────────────────────────────────────────────────────────────

void AEMBERVEILPlayerCharacter::ToggleLockOn()
{
	if (LockOnTarget)
	{
		ClearLockOn();
		return;
	}

	// Esfera de overlap para encontrar actores que implementen ICombatInterface
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(LockOnRadius),
		Params
	);

	AActor* BestTarget    = nullptr;
	float   BestDot       = -1.f;
	const FVector CamFwd  = FollowCamera->GetForwardVector();

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* Actor = Overlap.GetActor();
		if (!Actor || !Actor->Implements<UCombatInterface>()) continue;
		if (ICombatInterface::Execute_IsDead(Actor))         continue;

		// Prefiere el enemy más alineado con la dirección de la cámara
		const FVector ToTarget = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const float   Dot      = FVector::DotProduct(CamFwd, ToTarget);

		if (Dot > BestDot)
		{
			BestDot    = Dot;
			BestTarget = Actor;
		}
	}

	if (!BestTarget) return;

	LockOnTarget = BestTarget;

	// Con lock-on el personaje rota hacia el target, no hacia el movimiento
	bUseControllerRotationYaw                         = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AEMBERVEILPlayerCharacter::ClearLockOn()
{
	LockOnTarget = nullptr;

	bUseControllerRotationYaw                         = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEMBERVEILPlayerCharacter::UpdateLockOnCamera(float DeltaSeconds)
{
	if (!LockOnTarget || !GetController()) return;

	const FVector FromChar  = GetActorLocation();
	const FVector ToTarget  = LockOnTarget->GetActorLocation();
	const FVector Direction = (ToTarget - FromChar).GetSafeNormal();

	// Calcula la rotación deseada mirando al target y la interpola suavemente
	const FRotator TargetRotation = Direction.Rotation();
	const FRotator CurrentRot     = GetController()->GetControlRotation();
	const FRotator NewRot         = FMath::RInterpTo(CurrentRot, TargetRotation, DeltaSeconds, CameraLagSpeed);

	GetController()->SetControlRotation(NewRot);
}

// ─── Combo ────────────────────────────────────────────────────────────────────

void AEMBERVEILPlayerCharacter::OnBasicAttackLanded()
{
	ComboIndex++;
	bCanCombo = true;

	// Reinicia el timer: si el jugador no ataca antes de ComboResetTime, se cancela
	GetWorldTimerManager().ClearTimer(ComboResetTimer);
	GetWorldTimerManager().SetTimer(
		ComboResetTimer,
		this,
		&AEMBERVEILPlayerCharacter::ResetCombo,
		ComboResetTime,
		false
	);
}

void AEMBERVEILPlayerCharacter::ResetCombo()
{
	ComboIndex = 0;
	bCanCombo  = false;
	GetWorldTimerManager().ClearTimer(ComboResetTimer);
}
