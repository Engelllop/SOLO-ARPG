#include "Skills/SOLOAbilityBarComponent.h"

#include "Skills/SOLOSwordSkillDataAsset.h"
#include "Skills/SOLOSkillMasteryComponent.h"
#include "AbilitySystem/SOLOGameplayAbility.h"
#include "Player/SOLOPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

USOLOAbilityBarComponent::USOLOAbilityBarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USOLOAbilityBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// Inicializar el array de slots con entradas vacÃ­as
	AssignedSlots.SetNum(NumSlots);
	for (int32 i = 0; i < NumSlots; ++i)
	{
		AssignedSlots[i] = FAbilitySlot();
		AssignedSlots[i].SlotIndex = -1; // marca slot vacÃ­o
	}

	UnlockedSlots = InitialUnlockedSlots;

	// Cachear referencias al ASC y MasteryComponent
	CacheDependencies();
}

void USOLOAbilityBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USOLOAbilityBarComponent, AssignedSlots);
	DOREPLIFETIME(USOLOAbilityBarComponent, UnlockedSlots);
}

// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
// API pÃºblica
// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

bool USOLOAbilityBarComponent::AssignSkillToSlot(int32 SlotIndex, USOLOSwordSkillDataAsset* SkillData)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot â€” Ã­ndice de slot invÃ¡lido: %d"), SlotIndex);
		return false;
	}

	if (!IsSlotUnlocked(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot â€” slot %d estÃ¡ bloqueado"), SlotIndex);
		return false;
	}

	if (!SkillData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot â€” SkillData es nulo"));
		return false;
	}

	// Verificar que el jugador tiene la skill desbloqueada en su MasteryComponent
	if (CachedMasteryComponent && !CachedMasteryComponent->IsSkillUnlocked(SkillData->SkillID))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot â€” skill %s no desbloqueada"), *SkillData->SkillID.ToString());
		return false;
	}

	// Si el slot ya tenÃ­a una skill, revocarla del ASC antes de reemplazarla
	FAbilitySlot& Slot = AssignedSlots[SlotIndex];
	if (Slot.IsValid() && Slot.AbilityHandle.IsValid())
	{
		RevokeAbilityFromASC(Slot.AbilityHandle);
	}

	// Otorgar la nueva ability al ASC
	FGameplayAbilitySpecHandle NewHandle = GrantAbilityToASC(SkillData);
	if (!NewHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot â€” no se pudo otorgar la ability al ASC para skill %s"), *SkillData->SkillID.ToString());
		return false;
	}

	// Actualizar el slot
	Slot.SlotIndex = SlotIndex;
	Slot.SkillData = SkillData;
	Slot.AbilityHandle = NewHandle;

	NotifySlotChanged(SlotIndex);

	UE_LOG(LogTemp, Log, TEXT("AbilityBarComponent: Skill %s asignada al slot %d"), *SkillData->SkillID.ToString(), SlotIndex);
	return true;
}

bool USOLOAbilityBarComponent::RemoveSkillFromSlot(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return false;
	}

	FAbilitySlot& Slot = AssignedSlots[SlotIndex];
	if (!Slot.IsValid())
	{
		return false;
	}

	// Revocar la ability del ASC
	if (Slot.AbilityHandle.IsValid())
	{
		RevokeAbilityFromASC(Slot.AbilityHandle);
	}

	// Vaciar el slot
	Slot = FAbilitySlot();
	Slot.SlotIndex = -1;

	NotifySlotChanged(SlotIndex);
	return true;
}

bool USOLOAbilityBarComponent::SwapSlots(int32 SlotA, int32 SlotB)
{
	if (!IsValidSlotIndex(SlotA) || !IsValidSlotIndex(SlotB))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: SwapSlots â€” Ã­ndice invÃ¡lido (A=%d, B=%d)"), SlotA, SlotB);
		return false;
	}

	if (SlotA == SlotB)
	{
		// Swap consigo mismo no es un error, pero tampoco hace nada Ãºtil
		return true;
	}

	// Intercambiar el contenido de los dos slots
	FAbilitySlot TempSlot = AssignedSlots[SlotA];

	AssignedSlots[SlotA] = AssignedSlots[SlotB];
	AssignedSlots[SlotA].SlotIndex = SlotA;

	AssignedSlots[SlotB] = TempSlot;
	AssignedSlots[SlotB].SlotIndex = SlotB;

	NotifySlotChanged(SlotA);
	NotifySlotChanged(SlotB);
	return true;
}

FAbilitySlot USOLOAbilityBarComponent::GetSlot(int32 SlotIndex) const
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return FAbilitySlot();
	}
	return AssignedSlots[SlotIndex];
}

void USOLOAbilityBarComponent::UnlockSlot(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return;
	}

	// Solo desbloquear si el slot estÃ¡ actualmente bloqueado
	if (IsSlotUnlocked(SlotIndex))
	{
		return;
	}

	// UnlockedSlots es el contador de cuÃ¡ntos slots estÃ¡n habilitados (desde 0 hasta UnlockedSlots-1)
	// Desbloquear el siguiente slot en orden
	if (SlotIndex == UnlockedSlots && UnlockedSlots < NumSlots)
	{
		UnlockedSlots++;
		OnSlotUnlocked.Broadcast(SlotIndex);
		UE_LOG(LogTemp, Log, TEXT("AbilityBarComponent: Slot %d desbloqueado. Total desbloqueados: %d"), SlotIndex, UnlockedSlots);
	}
}

bool USOLOAbilityBarComponent::IsSlotUnlocked(int32 SlotIndex) const
{
	return IsValidSlotIndex(SlotIndex) && SlotIndex < UnlockedSlots;
}

void USOLOAbilityBarComponent::ActivateSkillInSlot(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return;
	}

	const FAbilitySlot& Slot = AssignedSlots[SlotIndex];
	if (!Slot.IsValid())
	{
		// Slot vacÃ­o â€” ignorar silenciosamente
		return;
	}

	if (!Slot.AbilityHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: ActivateSkillInSlot â€” slot %d tiene handle invÃ¡lido"), SlotIndex);
		return;
	}

	if (!CachedASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: ActivateSkillInSlot â€” ASC no disponible"));
		return;
	}

	CachedASC->TryActivateAbility(Slot.AbilityHandle);
}

// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
// Utilidades internas
// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOAbilityBarComponent::CacheDependencies()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	ASOLOPlayerState* PS = Cast<ASOLOPlayerState>(Owner);
	if (!PS)
	{
		// El componente puede vivir tambiÃ©n en un Character; intentar obtener el PlayerState desde Ã©l
		if (APawn* OwnerPawn = Cast<APawn>(Owner))
		{
			PS = OwnerPawn->GetPlayerState<ASOLOPlayerState>();
		}
	}

	if (PS)
	{
		CachedASC = PS->GetAbilitySystemComponent();
		CachedMasteryComponent = PS->FindComponentByClass<USOLOSkillMasteryComponent>();
	}

	if (!CachedASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: No se encontrÃ³ ASC en el owner o su PlayerState"));
	}

	if (!CachedMasteryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: No se encontrÃ³ USOLOSkillMasteryComponent en el PlayerState"));
	}
}

bool USOLOAbilityBarComponent::IsValidSlotIndex(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < NumSlots && SlotIndex < AssignedSlots.Num();
}

FGameplayAbilitySpecHandle USOLOAbilityBarComponent::GrantAbilityToASC(USOLOSwordSkillDataAsset* SkillData)
{
	if (!CachedASC || !SkillData || !SkillData->AbilityClass)
	{
		return FGameplayAbilitySpecHandle();
	}

	// Construir el spec y otorgar la ability al ASC
	FGameplayAbilitySpec AbilitySpec(
		SkillData->AbilityClass,
		SkillData->AbilityLevel,
		static_cast<int32>(ESOLOAbilityInputID::None),
		GetOwner()
	);

	return CachedASC->GiveAbility(AbilitySpec);
}

void USOLOAbilityBarComponent::RevokeAbilityFromASC(FGameplayAbilitySpecHandle Handle)
{
	if (!CachedASC || !Handle.IsValid())
	{
		return;
	}

	CachedASC->ClearAbility(Handle);
}

void USOLOAbilityBarComponent::NotifySlotChanged(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return;
	}

	OnAbilityBarChanged.Broadcast(SlotIndex, AssignedSlots[SlotIndex]);
}
