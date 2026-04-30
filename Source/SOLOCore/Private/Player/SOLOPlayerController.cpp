#include "Player/SOLOPlayerController.h"
#include "UI/SOLONotificationSystem.h"
#include "UI/HUD/SOLOHUDBase.h"
#include "Interaction/ISOLOInteractable.h"
#include "World/ASOLOSavePoint.h"
#include "Player/SOLOPlayerState.h"
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
#include "Character/SOLOPlayerCharacter.h"

#include <type_traits>
static_assert(std::is_base_of_v<AActor, ASOLOSavePoint>, "SavePoint include / interactable actor");

ASOLOPlayerController::ASOLOPlayerController()
{
	bShowMouseCursor = false;
}

void ASOLOPlayerController::BeginPlay()
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

void ASOLOPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CachePlayerState();
}

void ASOLOPlayerController::CachePlayerState()
{
	SOLOPlayerState = GetPlayerState<ASOLOPlayerState>();
}

void ASOLOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_Move)           EnhancedInput->BindAction(IA_Move,           ETriggerEvent::Triggered, this, &ASOLOPlayerController::HandleMove);
		if (IA_Look)           EnhancedInput->BindAction(IA_Look,           ETriggerEvent::Triggered, this, &ASOLOPlayerController::HandleLook);
		if (IA_Jump)
		{
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started,   this, &ASOLOPlayerController::HandleJump);
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ASOLOPlayerController::HandleStopJump);
		}
		if (IA_Dodge)          EnhancedInput->BindAction(IA_Dodge,          ETriggerEvent::Started,   this, &ASOLOPlayerController::HandleDodge);
		if (IA_BasicAttack)    EnhancedInput->BindAction(IA_BasicAttack,    ETriggerEvent::Started,   this, &ASOLOPlayerController::HandleBasicAttack);
		if (IA_Interact)       EnhancedInput->BindAction(IA_Interact,       ETriggerEvent::Started,   this, &ASOLOPlayerController::HandleInteract);
		if (IA_ToggleInventory) EnhancedInput->BindAction(IA_ToggleInventory, ETriggerEvent::Started,   this, &ASOLOPlayerController::HandleToggleInventory);
	}
	else
	{
		BindLegacyMovementAxes();
	}
}

void ASOLOPlayerController::EnsureDefaultLegacyMappings()
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

void ASOLOPlayerController::BindLegacyMovementAxes()
{
	EnsureDefaultLegacyMappings();

	UInputComponent* IC = InputComponent;
	if (!IC)
	{
		return;
	}

	IC->BindAxis(TEXT("MoveForward"), this, &ASOLOPlayerController::Legacy_MoveForward);
	IC->BindAxis(TEXT("MoveRight"), this, &ASOLOPlayerController::Legacy_MoveRight);
	IC->BindAxis(TEXT("Turn"), this, &ASOLOPlayerController::Legacy_Turn);
	IC->BindAxis(TEXT("LookUp"), this, &ASOLOPlayerController::Legacy_LookUp);
	IC->BindAction(TEXT("Jump"), IE_Pressed, this, &ASOLOPlayerController::Legacy_JumpPressed);
	IC->BindAction(TEXT("Jump"), IE_Released, this, &ASOLOPlayerController::Legacy_JumpReleased);
}

void ASOLOPlayerController::Legacy_MoveForward(float Value)
{
	if (!GetPawn() || FMath::IsNearlyZero(Value))
	{
		return;
	}
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	GetPawn()->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
}

void ASOLOPlayerController::Legacy_MoveRight(float Value)
{
	if (!GetPawn() || FMath::IsNearlyZero(Value))
	{
		return;
	}
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	GetPawn()->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
}

void ASOLOPlayerController::Legacy_Turn(float Value)
{
	AddYawInput(Value);
}

void ASOLOPlayerController::Legacy_LookUp(float Value)
{
	AddPitchInput(Value);
}

void ASOLOPlayerController::Legacy_JumpPressed()
{
	HandleJump();
}

void ASOLOPlayerController::Legacy_JumpReleased()
{
	HandleStopJump();
}

// â”€â”€â”€ Input callbacks â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOPlayerController::HandleMove(const FInputActionValue& Value)
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

void ASOLOPlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddYawInput(Axis.X);
	AddPitchInput(Axis.Y);
}

void ASOLOPlayerController::HandleJump()
{
	if (ACharacter* Char = GetCharacter())
		Char->Jump();
}

void ASOLOPlayerController::HandleStopJump()
{
	if (ACharacter* Char = GetCharacter())
		Char->StopJumping();
}

void ASOLOPlayerController::HandleDodge()
{
	if (SOLOPlayerState)
	{
		SOLOPlayerState->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Dodge")))
		);
	}
}

void ASOLOPlayerController::HandleBasicAttack()
{
	if (SOLOPlayerState)
	{
		SOLOPlayerState->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Combat.BasicAttack")))
		);
	}
}

void ASOLOPlayerController::HandleInteract()
{
	ASOLOPlayerCharacter* PlayerChar = Cast<ASOLOPlayerCharacter>(GetCharacter());
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
			// if hit actor implements ISOLOInteractable call Interact(this)
			if (HitActor->GetClass()->ImplementsInterface(USOLOInteractable::StaticClass()))
			{
				ISOLOInteractable::Execute_Interact(HitActor, PlayerChar);
			}
		}
	}
}

void ASOLOPlayerController::HandleToggleInventory()
{
	if (ASOLOHUDBase* HUD = GetHUD<ASOLOHUDBase>())
	{
		// gets HUDBase, toggles WBP_Inventory visibility
		HUD->ToggleInventory();
	}
}

void ASOLOPlayerController::Client_ShowDamageNumber_Implementation(
	float DamageAmount,
	FVector WorldLocation,
	uint8 DamageType,
	bool bIsCritical)
{
	if (ASOLOHUDBase* HUD = GetHUD<ASOLOHUDBase>())
	{
		HUD->ShowDamageNumber(
			DamageAmount,
			WorldLocation,
			static_cast<EDamageNumberType>(DamageType),
			bIsCritical);
	}
}

void ASOLOPlayerController::ClientReceiveNotification_Implementation(
	const FText& Message,
	uint8 NotificationType,
	float Duration)
{
	FSOLONotification Payload;
	Payload.Message = Message;
	Payload.Type = static_cast<ENotificationType>(NotificationType);
	Payload.Duration = Duration;

	if (UGameInstance* GI = GetGameInstance())
	{
		if (USOLONotificationSystem* Sys = GI->GetSubsystem<USOLONotificationSystem>())
		{
			Sys->BroadcastLocal(Payload);
		}
	}
}
