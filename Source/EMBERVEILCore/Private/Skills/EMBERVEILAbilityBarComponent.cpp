#include "Skills/EMBERVEILAbilityBarComponent.h"

#include "Skills/EMBERVEILSwordSkillDataAsset.h"
#include "Skills/EMBERVEILSkillMasteryComponent.h"
#include "AbilitySystem/EMBERVEILGameplayAbility.h"
#include "Player/EMBERVEILPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UEMBERVEILAbilityBarComponent::UEMBERVEILAbilityBarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEMBERVEILAbilityBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// Inicializar el array de slots con entradas vacías
	AssignedSlots.SetNum(NumSlots);
	for (int32 i = 0; i < NumSlots; ++i)
	{
		AssignedSlots[i] = FAbilitySlot();
		AssignedSlots[i].SlotIndex = -1; // marca slot vacío
	}

	UnlockedSlots = InitialUnlockedSlots;

	// Cachear referencias al ASC y MasteryComponent
	CacheDependencies();
}

void UEMBERVEILAbilityBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEMBERVEILAbilityBarComponent, AssignedSlots);
	DOREPLIFETIME(UEMBERVEILAbilityBarComponent, UnlockedSlots);
}

// ─────────────────────────────────────────────────────────────────────────────
// API pública
// ─────────────────────────────────────────────────────────────────────────────

bool UEMBERVEILAbilityBarComponent::AssignSkillToSlot(int32 SlotIndex, UEMBERVEILSwordSkillDataAsset* SkillData)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot — índice de slot inválido: %d"), SlotIndex);
		return false;
	}

	if (!IsSlotUnlocked(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot — slot %d está bloqueado"), SlotIndex);
		return false;
	}

	if (!SkillData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot — SkillData es nulo"));
		return false;
	}

	// Verificar que el jugador tiene la skill desbloqueada en su MasteryComponent
	if (CachedMasteryComponent && !CachedMasteryComponent->IsSkillUnlocked(SkillData->SkillID))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot — skill %s no desbloqueada"), *SkillData->SkillID.ToString());
		return false;
	}

	// Si el slot ya tenía una skill, revocarla del ASC antes de reemplazarla
	FAbilitySlot& Slot = AssignedSlots[SlotIndex];
	if (Slot.IsValid() && Slot.AbilityHandle.IsValid())
	{
		RevokeAbilityFromASC(Slot.AbilityHandle);
	}

	// Otorgar la nueva ability al ASC
	FGameplayAbilitySpecHandle NewHandle = GrantAbilityToASC(SkillData);
	if (!NewHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: AssignSkillToSlot — no se pudo otorgar la ability al ASC para skill %s"), *SkillData->SkillID.ToString());
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

bool UEMBERVEILAbilityBarComponent::RemoveSkillFromSlot(int32 SlotIndex)
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

bool UEMBERVEILAbilityBarComponent::SwapSlots(int32 SlotA, int32 SlotB)
{
	if (!IsValidSlotIndex(SlotA) || !IsValidSlotIndex(SlotB))
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: SwapSlots — índice inválido (A=%d, B=%d)"), SlotA, SlotB);
		return false;
	}

	if (SlotA == SlotB)
	{
		// Swap consigo mismo no es un error, pero tampoco hace nada útil
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

FAbilitySlot UEMBERVEILAbilityBarComponent::GetSlot(int32 SlotIndex) const
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return FAbilitySlot();
	}
	return AssignedSlots[SlotIndex];
}

void UEMBERVEILAbilityBarComponent::UnlockSlot(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return;
	}

	// Solo desbloquear si el slot está actualmente bloqueado
	if (IsSlotUnlocked(SlotIndex))
	{
		return;
	}

	// UnlockedSlots es el contador de cuántos slots están habilitados (desde 0 hasta UnlockedSlots-1)
	// Desbloquear el siguiente slot en orden
	if (SlotIndex == UnlockedSlots && UnlockedSlots < NumSlots)
	{
		UnlockedSlots++;
		OnSlotUnlocked.Broadcast(SlotIndex);
		UE_LOG(LogTemp, Log, TEXT("AbilityBarComponent: Slot %d desbloqueado. Total desbloqueados: %d"), SlotIndex, UnlockedSlots);
	}
}

bool UEMBERVEILAbilityBarComponent::IsSlotUnlocked(int32 SlotIndex) const
{
	return IsValidSlotIndex(SlotIndex) && SlotIndex < UnlockedSlots;
}

void UEMBERVEILAbilityBarComponent::ActivateSkillInSlot(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return;
	}

	const FAbilitySlot& Slot = AssignedSlots[SlotIndex];
	if (!Slot.IsValid())
	{
		// Slot vacío — ignorar silenciosamente
		return;
	}

	if (!Slot.AbilityHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: ActivateSkillInSlot — slot %d tiene handle inválido"), SlotIndex);
		return;
	}

	if (!CachedASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: ActivateSkillInSlot — ASC no disponible"));
		return;
	}

	CachedASC->TryActivateAbility(Slot.AbilityHandle);
}

// ─────────────────────────────────────────────────────────────────────────────
// Utilidades internas
// ─────────────────────────────────────────────────────────────────────────────

void UEMBERVEILAbilityBarComponent::CacheDependencies()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	AEMBERVEILPlayerState* PS = Cast<AEMBERVEILPlayerState>(Owner);
	if (!PS)
	{
		// El componente puede vivir también en un Character; intentar obtener el PlayerState desde él
		if (APawn* OwnerPawn = Cast<APawn>(Owner))
		{
			PS = OwnerPawn->GetPlayerState<AEMBERVEILPlayerState>();
		}
	}

	if (PS)
	{
		CachedASC = PS->GetAbilitySystemComponent();
		CachedMasteryComponent = PS->FindComponentByClass<UEMBERVEILSkillMasteryComponent>();
	}

	if (!CachedASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: No se encontró ASC en el owner o su PlayerState"));
	}

	if (!CachedMasteryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityBarComponent: No se encontró UEMBERVEILSkillMasteryComponent en el PlayerState"));
	}
}

bool UEMBERVEILAbilityBarComponent::IsValidSlotIndex(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < NumSlots && SlotIndex < AssignedSlots.Num();
}

FGameplayAbilitySpecHandle UEMBERVEILAbilityBarComponent::GrantAbilityToASC(UEMBERVEILSwordSkillDataAsset* SkillData)
{
	if (!CachedASC || !SkillData || !SkillData->AbilityClass)
	{
		return FGameplayAbilitySpecHandle();
	}

	// Construir el spec y otorgar la ability al ASC
	FGameplayAbilitySpec AbilitySpec(
		SkillData->AbilityClass,
		SkillData->AbilityLevel,
		static_cast<int32>(EEMBERVEILAbilityInputID::None),
		GetOwner()
	);

	return CachedASC->GiveAbility(AbilitySpec);
}

void UEMBERVEILAbilityBarComponent::RevokeAbilityFromASC(FGameplayAbilitySpecHandle Handle)
{
	if (!CachedASC || !Handle.IsValid())
	{
		return;
	}

	CachedASC->ClearAbility(Handle);
}

void UEMBERVEILAbilityBarComponent::NotifySlotChanged(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return;
	}

	OnAbilityBarChanged.Broadcast(SlotIndex, AssignedSlots[SlotIndex]);
}
