#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/SOLOClassData.h"
#include "Data/SOLOSkillData.h"
#include "SOLOClassSubsystem.generated.h"

UCLASS()
class SOLOCORE_API USOLOClassSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FSOLOClassData* GetClassData(FName ClassID) const;
	TArray<FSOLOSkillData> GetSkillsForClass(FName ClassID) const;
	bool IsSkillUnlocked(const FString& PlayerID, FName SkillID) const;
	bool UnlockSkill(const FString& PlayerID, FName SkillID);
	bool MeetsPrerequisites(const FString& PlayerID, const FSOLOSkillData& Skill) const;

protected:
	UPROPERTY() UDataTable* ClassDataTable;
	UPROPERTY() UDataTable* SkillDataTable;

private:
	// PlayerID -> set of unlocked skill IDs (runtime only; saved via SaveSubsystem)
	TMap<FString, TSet<FName>> UnlockedSkills;
};
