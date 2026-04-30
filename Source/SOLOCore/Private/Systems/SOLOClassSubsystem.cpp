#include "Systems/SOLOClassSubsystem.h"
#include "Engine/DataTable.h"

void USOLOClassSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Data tables assigned via DefaultEngine.ini or BP override; loaded on demand
}

const FSOLOClassData* USOLOClassSubsystem::GetClassData(FName ClassID) const
{
	if (!ClassDataTable) return nullptr;
	return ClassDataTable->FindRow<FSOLOClassData>(ClassID, TEXT("ClassSubsystem"));
}

TArray<FSOLOSkillData> USOLOClassSubsystem::GetSkillsForClass(FName ClassID) const
{
	TArray<FSOLOSkillData> Result;
	if (!SkillDataTable) return Result;

	TArray<FSOLOSkillData*> AllRows;
	SkillDataTable->GetAllRows<FSOLOSkillData>(TEXT("SkillSubsystem"), AllRows);
	for (const FSOLOSkillData* Row : AllRows)
		if (Row && Row->ClassID == ClassID) Result.Add(*Row);
	return Result;
}

bool USOLOClassSubsystem::IsSkillUnlocked(const FString& PlayerID, FName SkillID) const
{
	const TSet<FName>* Skills = UnlockedSkills.Find(PlayerID);
	return Skills && Skills->Contains(SkillID);
}

bool USOLOClassSubsystem::MeetsPrerequisites(const FString& PlayerID, const FSOLOSkillData& Skill) const
{
	for (const FName& Prereq : Skill.PrerequisiteSkillIDs)
		if (!IsSkillUnlocked(PlayerID, Prereq)) return false;
	return true;
}

bool USOLOClassSubsystem::UnlockSkill(const FString& PlayerID, FName SkillID)
{
	if (!SkillDataTable) return false;
	const FSOLOSkillData* Skill = SkillDataTable->FindRow<FSOLOSkillData>(SkillID, TEXT("Unlock"));
	if (!Skill || !MeetsPrerequisites(PlayerID, *Skill)) return false;

	UnlockedSkills.FindOrAdd(PlayerID).Add(SkillID);
	return true;
}
