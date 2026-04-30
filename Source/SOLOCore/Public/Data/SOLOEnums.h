#pragma once

#include "CoreMinimal.h"
#include "SOLOEnums.generated.h"

UENUM(BlueprintType)
enum class ESOLOSkillType : uint8
{
	Passive,
	Active,
	Ultimate,
	ClassSkill
};

UENUM(BlueprintType)
enum class ESOLORank : uint8
{
	E,
	D,
	C,
	B,
	A,
	S,
	SS,
	SSS
};

UENUM(BlueprintType)
enum class ESOLOResourceType : uint8
{
	Mana,
	Stamina,
	Rage,
	Energy,
	None
};

UENUM(BlueprintType)
enum class ESOLODamageType : uint8
{
	Slashing,
	Blunt,
	Piercing,
	Fire,
	Ice,
	Electric,
	Holy,
	Dark,
	Poison,
	Weave
};

UENUM(BlueprintType)
enum class ESOLODamageCategory : uint8
{
	Physical,
	Magical,
	Special
};

UENUM(BlueprintType)
enum class ESOLOChestType : uint8
{
	Wood,
	Iron,
	Silver,
	Gold
};

UENUM(BlueprintType)
enum class ESOLOStationLevel : uint8
{
	Basic    UMETA(DisplayName = "Basic (N1)"),
	Improved UMETA(DisplayName = "Improved (N5)"),
	Master   UMETA(DisplayName = "Master (N9)")
};

UENUM(BlueprintType)
enum class ESOLOCampfireType : uint8
{
	Common,
	Greater,
	Hidden,
	Boss
};

UENUM(BlueprintType)
enum class ESOLOCompanionArchetype : uint8
{
	RangedDPS,
	Tank,
	Flanker,
	SpeedDPS
};

UENUM(BlueprintType)
enum class ESOLOCompanionCommand : uint8
{
	Attack,
	Defend,
	Shoot,
	Retreat,
	SpecialAbility,
	Disperse,
	Rally
};

UENUM(BlueprintType)
enum class ESOLOAttackType : uint8
{
	Light,
	Heavy
};
