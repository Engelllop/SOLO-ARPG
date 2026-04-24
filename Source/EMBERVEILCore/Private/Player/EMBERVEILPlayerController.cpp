#include "Player/EMBERVEILPlayerController.h"
#include "UI/EMBERVEILNotificationSystem.h"
#include "UI/HUD/EMBERVEILHUDBase.h"
#include "Interaction/IEMBERVEILInteractable.h"
#include "World/AEMBERVEILSavePoint.h"
#include "Player/EMBERVEILPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "InputCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/EMBERVEILPlayerCharacter.h"

#include <type_traits>
static_assert(std::is_base_of_v<AActor, AEMBERVEILSavePoint>, "SavePoint include / interactable actor");

AEMBERVEILPlayerController::AEMBERVEILPlayerController()
{
	bShowMouseCursor = false;
}

void AEMBERVEILPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!DefaultMappingContext)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AEMBERVEILPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CachePlayerState();
}

void AEMBERVEILPlayerController::CachePlayerState()
{
	EMBERVEILPlayerState = GetPlayerState<AEMBERVEILPlayerState>();
}

void AEMBERVEILPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_Move)           EnhancedInput->BindAction(IA_Move,           ETriggerEvent::Triggered, this, &AEMBERVEILPlayerController::HandleMove);
		if (IA_Look)           EnhancedInput->BindAction(IA_Look,           ETriggerEvent::Triggered, this, &AEMBERVEILPlayerController::HandleLook);
		if (IA_Jump)
		{
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started,   this, &AEMBERVEILPlayerController::HandleJump);
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AEMBERVEILPlayerController::HandleStopJump);
		}
		if (IA_Dodge)          EnhancedInput->BindAction(IA_Dodge,          ETriggerEvent::Started,   this, &AEMBERVEILPlayerController::HandleDodge);
		if (IA_BasicAttack)    EnhancedInput->BindAction(IA_BasicAttack,    ETriggerEvent::Started,   this, &AEMBERVEILPlayerController::HandleBasicAttack);
		if (IA_Interact)       EnhancedInput->BindAction(IA_Interact,       ETriggerEvent::Started,   this, &AEMBERVEILPlayerController::HandleInteract);
		if (IA_ToggleInventory) EnhancedInput->BindAction(IA_ToggleInventory, ETriggerEvent::Started,   this, &AEMBERVEILPlayerController::HandleToggleInventory);
	}
	else
	{
		BindLegacyMovementAxes();
	}
}

void AEMBERVEILPlayerController::EnsureDefaultLegacyMappings()
{
	UInputSettings* Settings = UInputSettings::GetInputSettings();
	if (!Settings)
	{
		return;
	}

	auto AxisMissing = [Settings](FName AxisName, FKey Key) -> bool
	{
		TArray<FInputAxisKeyMapping> Mappings;
		Settings->GetAxisMappingByName(AxisName, Mappings);
		for (const FInputAxisKeyMapping& M : Mappings)
		{
			if (M.Key == Key)
			{
				return false;
			}
		}
		return true;
	};

	if (AxisMissing(TEXT("MoveForward"), EKeys::W))
	{
		Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveForward"), EKeys::W, 1.f));
	}
	if (AxisMissing(TEXT("MoveForward"), EKeys::S))
	{
		Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveForward"), EKeys::S, -1.f));
	}
	if (AxisMissing(TEXT("MoveRight"), EKeys::D))
	{
		Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveRight"), EKeys::D, 1.f));
	}
	if (AxisMissing(TEXT("MoveRight"), EKeys::A))
	{
		Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveRight"), EKeys::A, -1.f));
	}
	if (AxisMissing(TEXT("Turn"), EKeys::MouseX))
	{
		Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("Turn"), EKeys::MouseX, 1.f));
	}
	if (AxisMissing(TEXT("LookUp"), EKeys::MouseY))
	{
		Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("LookUp"), EKeys::MouseY, -1.f));
	}

	TArray<FInputActionKeyMapping> JumpMappings;
	Settings->GetActionMappingByName(TEXT("Jump"), JumpMappings);
	bool bHasSpaceJump = false;
	for (const FInputActionKeyMapping& M : JumpMappings)
	{
		if (M.Key == EKeys::SpaceBar)
		{
			bHasSpaceJump = true;
			break;
		}
	}
	if (!bHasSpaceJump)
	{
		Settings->AddActionMapping(FInputActionKeyMapping(TEXT("Jump"), EKeys::SpaceBar));
	}
}

void AEMBERVEILPlayerController::BindLegacyMovementAxes()
{
	EnsureDefaultLegacyMappings();

	UInputComponent* IC = InputComponent;
	if (!IC)
	{
		return;
	}

	IC->BindAxis(TEXT("MoveForward"), this, &AEMBERVEILPlayerController::Legacy_MoveForward);
	IC->BindAxis(TEXT("MoveRight"), this, &AEMBERVEILPlayerController::Legacy_MoveRight);
	IC->BindAxis(TEXT("Turn"), this, &AEMBERVEILPlayerController::Legacy_Turn);
	IC->BindAxis(TEXT("LookUp"), this, &AEMBERVEILPlayerController::Legacy_LookUp);
	IC->BindAction(TEXT("Jump"), IE_Pressed, this, &AEMBERVEILPlayerController::Legacy_JumpPressed);
	IC->BindAction(TEXT("Jump"), IE_Released, this, &AEMBERVEILPlayerController::Legacy_JumpReleased);
}

void AEMBERVEILPlayerController::Legacy_MoveForward(float Value)
{
	if (!GetPawn() || FMath::IsNearlyZero(Value))
	{
		return;
	}
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	GetPawn()->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
}

void AEMBERVEILPlayerController::Legacy_MoveRight(float Value)
{
	if (!GetPawn() || FMath::IsNearlyZero(Value))
	{
		return;
	}
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	GetPawn()->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
}

void AEMBERVEILPlayerController::Legacy_Turn(float Value)
{
	AddYawInput(Value);
}

void AEMBERVEILPlayerController::Legacy_LookUp(float Value)
{
	AddPitchInput(Value);
}

void AEMBERVEILPlayerController::Legacy_JumpPressed()
{
	HandleJump();
}

void AEMBERVEILPlayerController::Legacy_JumpReleased()
{
	HandleStopJump();
}

// ─── Input callbacks ──────────────────────────────────────────────────────────

void AEMBERVEILPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (!GetPawn()) return;

	const FVector2D Axis = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	// calls AddMovementInput on controlled pawn using forward/right vectors
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDir, Axis.Y);
	GetPawn()->AddMovementInput(RightDir, Axis.X);
}

void AEMBERVEILPlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddYawInput(Axis.X);
	AddPitchInput(Axis.Y);
}

void AEMBERVEILPlayerController::HandleJump()
{
	if (ACharacter* Char = GetCharacter())
		Char->Jump();
}

void AEMBERVEILPlayerController::HandleStopJump()
{
	if (ACharacter* Char = GetCharacter())
		Char->StopJumping();
}

void AEMBERVEILPlayerController::HandleDodge()
{
	if (EMBERVEILPlayerState)
	{
		EMBERVEILPlayerState->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Dodge")))
		);
	}
}

void AEMBERVEILPlayerController::HandleBasicAttack()
{
	if (EMBERVEILPlayerState)
	{
		EMBERVEILPlayerState->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Combat.BasicAttack")))
		);
	}
}

void AEMBERVEILPlayerController::HandleInteract()
{
	AEMBERVEILPlayerCharacter* PlayerChar = Cast<AEMBERVEILPlayerCharacter>(GetCharacter());
	if (!PlayerChar) return;

	FVector TraceStart;
	FVector TraceDirection;
	
	if (UCameraComponent* Camera = PlayerChar->FindComponentByClass<UCameraComponent>())
	{
		TraceStart     = Camera->GetComponentLocation();
		TraceDirection = Camera->GetForwardVector();
	}
	else
	{
		FRotator ViewRotation;
		GetPlayerViewPoint(TraceStart, ViewRotation);
		TraceDirection = ViewRotation.Vector();
	}

	// line trace 200cm forward
	const FVector TraceEnd = TraceStart + TraceDirection * 200.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerChar);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			// if hit actor implements IEMBERVEILInteractable call Interact(this)
			if (HitActor->GetClass()->ImplementsInterface(UEMBERVEILInteractable::StaticClass()))
			{
				IEMBERVEILInteractable::Execute_Interact(HitActor, PlayerChar);
			}
		}
	}
}

void AEMBERVEILPlayerController::HandleToggleInventory()
{
	if (AEMBERVEILHUDBase* HUD = GetHUD<AEMBERVEILHUDBase>())
	{
		// gets HUDBase, toggles WBP_Inventory visibility
		HUD->ToggleInventory();
	}
}

void AEMBERVEILPlayerController::Client_ShowDamageNumber_Implementation(
	float DamageAmount,
	FVector WorldLocation,
	uint8 DamageType,
	bool bIsCritical)
{
	if (AEMBERVEILHUDBase* HUD = GetHUD<AEMBERVEILHUDBase>())
	{
		HUD->ShowDamageNumber(
			DamageAmount,
			WorldLocation,
			static_cast<EDamageNumberType>(DamageType),
			bIsCritical);
	}
}

void AEMBERVEILPlayerController::ClientReceiveNotification_Implementation(
	const FText& Message,
	uint8 NotificationType,
	float Duration)
{
	FEMBERVEILNotification Payload;
	Payload.Message = Message;
	Payload.Type = static_cast<ENotificationType>(NotificationType);
	Payload.Duration = Duration;

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UEMBERVEILNotificationSystem* Sys = GI->GetSubsystem<UEMBERVEILNotificationSystem>())
		{
			Sys->BroadcastLocal(Payload);
		}
	}
}
