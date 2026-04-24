#include "Skills/EMBERVEILSkillMasteryComponent.h"

#include "Skills/EMBERVEILSwordSkillDataAsset.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

UEMBERVEILSkillMasteryComponent::UEMBERVEILSkillMasteryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEMBERVEILSkillMasteryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEMBERVEILSkillMasteryComponent, MasteryRows);
	DOREPLIFETIME(UEMBERVEILSkillMasteryComponent, UnlockedSkillIDs);
	DOREPLIFETIME(UEMBERVEILSkillMasteryComponent, ActiveWeaponType);
}

// ─────────────────────────────────────────────────────────────────────────────
// API pública
// ─────────────────────────────────────────────────────────────────────────────

void UEMBERVEILSkillMasteryComponent::RestoreUnlockedSkillsFromSave(const TArray<FName>& InSkillIDs)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	UnlockedSkillIDs = InSkillIDs;
	MARK_PROPERTY_DIRTY_FROM_NAME(UEMBERVEILSkillMasteryComponent, UnlockedSkillIDs, this);
}

void UEMBERVEILSkillMasteryComponent::AddMasteryXP(EWeaponType WeaponType, float XPAmount)
{
	if (!GetOwner()->HasAuthority())
	{
		// Solo el servidor modifica el estado; el cliente recibe actualizaciones por replicación
		return;
	}

	if (WeaponType == EWeaponType::None || XPAmount <= 0.f)
	{
		return;
	}

	// Buscar o crear la entrada de maestría para este tipo de arma
	FWeaponMasteryData& MasteryData = GetOrAddMasteryRow(WeaponType);
	MasteryData.WeaponType = WeaponType;
	MasteryData.CurrentXP += XPAmount;

	// Bucle de subida de nivel: puede subir varios niveles de golpe si se añade mucha XP
	bool bLeveledUp = false;
	while (MasteryData.MasteryLevel < 20 && MasteryData.CurrentXP >= MasteryData.GetXPForNextLevel())
	{
		const float XPThreshold = MasteryData.GetXPForNextLevel();
		MasteryData.CurrentXP -= XPThreshold;
		MasteryData.MasteryLevel++;

		// Notificar subida de nivel
		OnMasteryLevelUp.Broadcast(WeaponType, MasteryData.MasteryLevel);

		// Intentar desbloquear skills elegibles para el nuevo nivel
		TryAutoUnlockSkills(WeaponType, MasteryData.MasteryLevel);

		bLeveledUp = true;
	}

	// Si estamos al nivel máximo, no acumulamos XP extra
	if (MasteryData.MasteryLevel >= 20)
	{
		MasteryData.CurrentXP = 0.f;
	}
}

bool UEMBERVEILSkillMasteryComponent::IsSkillUnlocked(FName SkillID) const
{
	return UnlockedSkillIDs.Contains(SkillID);
}

bool UEMBERVEILSkillMasteryComponent::CanUnlockSkill(UEMBERVEILSwordSkillDataAsset* SkillData) const
{
	if (!SkillData)
	{
		return false;
	}

	// La skill no debe estar ya desbloqueada
	if (UnlockedSkillIDs.Contains(SkillData->SkillID))
	{
		return false;
	}

	// Verificar nivel de maestría suficiente
	const int32 CurrentLevel = GetMasteryLevel(SkillData->RequiredWeaponType);
	if (CurrentLevel < SkillData->RequiredMasteryLevel)
	{
		return false;
	}

	// Verificar que todos los prerequisitos están desbloqueados
	for (const FName& RequiredID : SkillData->RequiredSkillIDs)
	{
		if (!UnlockedSkillIDs.Contains(RequiredID))
		{
			return false;
		}
	}

	return true;
}

int32 UEMBERVEILSkillMasteryComponent::GetMasteryLevel(EWeaponType WeaponType) const
{
	const FWeaponMasteryData* MasteryData = FindMasteryRow(WeaponType);
	return MasteryData ? MasteryData->MasteryLevel : 0;
}

float UEMBERVEILSkillMasteryComponent::GetMasteryXP(EWeaponType WeaponType) const
{
	const FWeaponMasteryData* MasteryData = FindMasteryRow(WeaponType);
	return MasteryData ? MasteryData->CurrentXP : 0.f;
}

FWeaponMasteryData UEMBERVEILSkillMasteryComponent::GetMasteryData(EWeaponType WeaponType) const
{
	const FWeaponMasteryData* MasteryData = FindMasteryRow(WeaponType);
	if (MasteryData)
	{
		return *MasteryData;
	}

	// Devolver un struct vacío con el WeaponType correcto si no hay datos todavía
	FWeaponMasteryData Empty;
	Empty.WeaponType = WeaponType;
	return Empty;
}

TArray<UEMBERVEILSwordSkillDataAsset*> UEMBERVEILSkillMasteryComponent::GetUnlockedSkillsForWeapon(EWeaponType WeaponType) const
{
	TArray<UEMBERVEILSwordSkillDataAsset*> Result;

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!AssetManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("EMBERVEILSkillMasteryComponent: AssetManager no disponible"));
		return Result;
	}

	// Obtener todos los IDs registrados del tipo "SwordSkill"
	TArray<FPrimaryAssetId> SkillAssetIds;
	AssetManager->GetPrimaryAssetIdList(FPrimaryAssetType("SwordSkill"), SkillAssetIds);

	for (const FPrimaryAssetId& AssetId : SkillAssetIds)
	{
		// Intentar obtener el asset de forma síncrona (debe estar en memoria o en un bundle cargado)
		UEMBERVEILSwordSkillDataAsset* SkillAsset = Cast<UEMBERVEILSwordSkillDataAsset>(
			AssetManager->GetPrimaryAssetObject(AssetId));

		if (!SkillAsset)
		{
			// Si el asset no está cargado en memoria aún, intentar cargarlo síncronamente
			FSoftObjectPath AssetPath = AssetManager->GetPrimaryAssetPath(AssetId);
			if (!AssetPath.IsNull())
			{
				SkillAsset = Cast<UEMBERVEILSwordSkillDataAsset>(AssetPath.TryLoad());
			}
		}

		if (!SkillAsset)
		{
			continue;
		}

		// Filtrar por tipo de arma y por si está desbloqueada
		if (SkillAsset->RequiredWeaponType == WeaponType && UnlockedSkillIDs.Contains(SkillAsset->SkillID))
		{
			Result.Add(SkillAsset);
		}
	}

	return Result;
}

void UEMBERVEILSkillMasteryComponent::SetActiveWeaponType(EWeaponType NewType)
{
	if (ActiveWeaponType != NewType)
	{
		ActiveWeaponType = NewType;
	}
}

// ─────────────────────────────────────────────────────────────────────────────
// Lógica interna
// ─────────────────────────────────────────────────────────────────────────────

void UEMBERVEILSkillMasteryComponent::TryAutoUnlockSkills(EWeaponType WeaponType, int32 CurrentLevel)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!AssetManager)
	{
		return;
	}

	TArray<FPrimaryAssetId> SkillAssetIds;
	AssetManager->GetPrimaryAssetIdList(FPrimaryAssetType("SwordSkill"), SkillAssetIds);

	for (const FPrimaryAssetId& AssetId : SkillAssetIds)
	{
		UEMBERVEILSwordSkillDataAsset* SkillAsset = Cast<UEMBERVEILSwordSkillDataAsset>(
			AssetManager->GetPrimaryAssetObject(AssetId));

		if (!SkillAsset)
		{
			FSoftObjectPath AssetPath = AssetManager->GetPrimaryAssetPath(AssetId);
			if (!AssetPath.IsNull())
			{
				SkillAsset = Cast<UEMBERVEILSwordSkillDataAsset>(AssetPath.TryLoad());
			}
		}

		if (!SkillAsset)
		{
			continue;
		}

		// Filtro rápido antes de CanUnlockSkill para evitar iteraciones innecesarias
		if (SkillAsset->RequiredWeaponType != WeaponType)
		{
			continue;
		}

		if (SkillAsset->RequiredMasteryLevel > CurrentLevel)
		{
			continue;
		}

		// CanUnlockSkill verifica también prerequisitos y duplicados
		if (CanUnlockSkill(SkillAsset))
		{
			UnlockSkillInternal(SkillAsset->SkillID);
		}
	}
}

void UEMBERVEILSkillMasteryComponent::UnlockSkillInternal(FName SkillID)
{
	if (SkillID.IsNone() || UnlockedSkillIDs.Contains(SkillID))
	{
		return;
	}

	UnlockedSkillIDs.AddUnique(SkillID);
	OnSkillUnlocked.Broadcast(SkillID);

	UE_LOG(LogTemp, Log, TEXT("EMBERVEILSkillMasteryComponent: Skill desbloqueada -> %s"), *SkillID.ToString());
}

FWeaponMasteryData& UEMBERVEILSkillMasteryComponent::GetOrAddMasteryRow(EWeaponType WeaponType)
{
	for (FWeaponMasteryData& Row : MasteryRows)
	{
		if (Row.WeaponType == WeaponType)
		{
			return Row;
		}
	}

	FWeaponMasteryData NewRow;
	NewRow.WeaponType = WeaponType;
	const int32 Index = MasteryRows.Add(NewRow);
	return MasteryRows[Index];
}

const FWeaponMasteryData* UEMBERVEILSkillMasteryComponent::FindMasteryRow(EWeaponType WeaponType) const
{
	for (const FWeaponMasteryData& Row : MasteryRows)
	{
		if (Row.WeaponType == WeaponType)
		{
			return &Row;
		}
	}
	return nullptr;
}
