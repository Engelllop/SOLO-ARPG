#include "Skills/SOLOSkillMasteryComponent.h"

#include "Skills/SOLOSwordSkillDataAsset.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

USOLOSkillMasteryComponent::USOLOSkillMasteryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USOLOSkillMasteryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USOLOSkillMasteryComponent, MasteryRows);
	DOREPLIFETIME(USOLOSkillMasteryComponent, UnlockedSkillIDs);
	DOREPLIFETIME(USOLOSkillMasteryComponent, ActiveWeaponType);
}

// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
// API pÃºblica
// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOSkillMasteryComponent::RestoreUnlockedSkillsFromSave(const TArray<FName>& InSkillIDs)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	UnlockedSkillIDs = InSkillIDs;
	MARK_PROPERTY_DIRTY_FROM_NAME(USOLOSkillMasteryComponent, UnlockedSkillIDs, this);
}

void USOLOSkillMasteryComponent::AddMasteryXP(EWeaponType WeaponType, float XPAmount)
{
	if (!GetOwner()->HasAuthority())
	{
		// Solo el servidor modifica el estado; el cliente recibe actualizaciones por replicaciÃ³n
		return;
	}

	if (WeaponType == EWeaponType::None || XPAmount <= 0.f)
	{
		return;
	}

	// Buscar o crear la entrada de maestrÃ­a para este tipo de arma
	FWeaponMasteryData& MasteryData = GetOrAddMasteryRow(WeaponType);
	MasteryData.WeaponType = WeaponType;
	MasteryData.CurrentXP += XPAmount;

	// Bucle de subida de nivel: puede subir varios niveles de golpe si se aÃ±ade mucha XP
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

	// Si estamos al nivel mÃ¡ximo, no acumulamos XP extra
	if (MasteryData.MasteryLevel >= 20)
	{
		MasteryData.CurrentXP = 0.f;
	}
}

bool USOLOSkillMasteryComponent::IsSkillUnlocked(FName SkillID) const
{
	return UnlockedSkillIDs.Contains(SkillID);
}

bool USOLOSkillMasteryComponent::CanUnlockSkill(USOLOSwordSkillDataAsset* SkillData) const
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

	// Verificar nivel de maestrÃ­a suficiente
	const int32 CurrentLevel = GetMasteryLevel(SkillData->RequiredWeaponType);
	if (CurrentLevel < SkillData->RequiredMasteryLevel)
	{
		return false;
	}

	// Verificar que todos los prerequisitos estÃ¡n desbloqueados
	for (const FName& RequiredID : SkillData->RequiredSkillIDs)
	{
		if (!UnlockedSkillIDs.Contains(RequiredID))
		{
			return false;
		}
	}

	return true;
}

int32 USOLOSkillMasteryComponent::GetMasteryLevel(EWeaponType WeaponType) const
{
	const FWeaponMasteryData* MasteryData = FindMasteryRow(WeaponType);
	return MasteryData ? MasteryData->MasteryLevel : 0;
}

float USOLOSkillMasteryComponent::GetMasteryXP(EWeaponType WeaponType) const
{
	const FWeaponMasteryData* MasteryData = FindMasteryRow(WeaponType);
	return MasteryData ? MasteryData->CurrentXP : 0.f;
}

FWeaponMasteryData USOLOSkillMasteryComponent::GetMasteryData(EWeaponType WeaponType) const
{
	const FWeaponMasteryData* MasteryData = FindMasteryRow(WeaponType);
	if (MasteryData)
	{
		return *MasteryData;
	}

	// Devolver un struct vacÃ­o con el WeaponType correcto si no hay datos todavÃ­a
	FWeaponMasteryData Empty;
	Empty.WeaponType = WeaponType;
	return Empty;
}

TArray<USOLOSwordSkillDataAsset*> USOLOSkillMasteryComponent::GetUnlockedSkillsForWeapon(EWeaponType WeaponType) const
{
	TArray<USOLOSwordSkillDataAsset*> Result;

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!AssetManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("SOLOSkillMasteryComponent: AssetManager no disponible"));
		return Result;
	}

	// Obtener todos los IDs registrados del tipo "SwordSkill"
	TArray<FPrimaryAssetId> SkillAssetIds;
	AssetManager->GetPrimaryAssetIdList(FPrimaryAssetType("SwordSkill"), SkillAssetIds);

	for (const FPrimaryAssetId& AssetId : SkillAssetIds)
	{
		// Intentar obtener el asset de forma sÃ­ncrona (debe estar en memoria o en un bundle cargado)
		USOLOSwordSkillDataAsset* SkillAsset = Cast<USOLOSwordSkillDataAsset>(
			AssetManager->GetPrimaryAssetObject(AssetId));

		if (!SkillAsset)
		{
			// Si el asset no estÃ¡ cargado en memoria aÃºn, intentar cargarlo sÃ­ncronamente
			FSoftObjectPath AssetPath = AssetManager->GetPrimaryAssetPath(AssetId);
			if (!AssetPath.IsNull())
			{
				SkillAsset = Cast<USOLOSwordSkillDataAsset>(AssetPath.TryLoad());
			}
		}

		if (!SkillAsset)
		{
			continue;
		}

		// Filtrar por tipo de arma y por si estÃ¡ desbloqueada
		if (SkillAsset->RequiredWeaponType == WeaponType && UnlockedSkillIDs.Contains(SkillAsset->SkillID))
		{
			Result.Add(SkillAsset);
		}
	}

	return Result;
}

void USOLOSkillMasteryComponent::SetActiveWeaponType(EWeaponType NewType)
{
	if (ActiveWeaponType != NewType)
	{
		ActiveWeaponType = NewType;
	}
}

// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
// LÃ³gica interna
// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOSkillMasteryComponent::TryAutoUnlockSkills(EWeaponType WeaponType, int32 CurrentLevel)
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
		USOLOSwordSkillDataAsset* SkillAsset = Cast<USOLOSwordSkillDataAsset>(
			AssetManager->GetPrimaryAssetObject(AssetId));

		if (!SkillAsset)
		{
			FSoftObjectPath AssetPath = AssetManager->GetPrimaryAssetPath(AssetId);
			if (!AssetPath.IsNull())
			{
				SkillAsset = Cast<USOLOSwordSkillDataAsset>(AssetPath.TryLoad());
			}
		}

		if (!SkillAsset)
		{
			continue;
		}

		// Filtro rÃ¡pido antes de CanUnlockSkill para evitar iteraciones innecesarias
		if (SkillAsset->RequiredWeaponType != WeaponType)
		{
			continue;
		}

		if (SkillAsset->RequiredMasteryLevel > CurrentLevel)
		{
			continue;
		}

		// CanUnlockSkill verifica tambiÃ©n prerequisitos y duplicados
		if (CanUnlockSkill(SkillAsset))
		{
			UnlockSkillInternal(SkillAsset->SkillID);
		}
	}
}

void USOLOSkillMasteryComponent::UnlockSkillInternal(FName SkillID)
{
	if (SkillID.IsNone() || UnlockedSkillIDs.Contains(SkillID))
	{
		return;
	}

	UnlockedSkillIDs.AddUnique(SkillID);
	OnSkillUnlocked.Broadcast(SkillID);

	UE_LOG(LogTemp, Log, TEXT("SOLOSkillMasteryComponent: Skill desbloqueada -> %s"), *SkillID.ToString());
}

FWeaponMasteryData& USOLOSkillMasteryComponent::GetOrAddMasteryRow(EWeaponType WeaponType)
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

const FWeaponMasteryData* USOLOSkillMasteryComponent::FindMasteryRow(EWeaponType WeaponType) const
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
