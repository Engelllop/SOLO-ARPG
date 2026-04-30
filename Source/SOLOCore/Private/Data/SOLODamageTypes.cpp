#include "Data/SOLODamageTypes.h"
#include "GameplayTagsManager.h"

namespace SOLODamageTags
{
	FGameplayTag Damage_Slashing;
	FGameplayTag Damage_Blunt;
	FGameplayTag Damage_Piercing;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Electric;
	FGameplayTag Damage_Holy;
	FGameplayTag Damage_Dark;
	FGameplayTag Damage_Poison;
	FGameplayTag Damage_Weave;

	void InitializeDamageTags()
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		Damage_Slashing  = Manager.RequestGameplayTag(FName("Damage.Type.Slashing"));
		Damage_Blunt     = Manager.RequestGameplayTag(FName("Damage.Type.Blunt"));
		Damage_Piercing  = Manager.RequestGameplayTag(FName("Damage.Type.Piercing"));
		Damage_Fire      = Manager.RequestGameplayTag(FName("Damage.Type.Fire"));
		Damage_Ice       = Manager.RequestGameplayTag(FName("Damage.Type.Ice"));
		Damage_Electric  = Manager.RequestGameplayTag(FName("Damage.Type.Electric"));
		Damage_Holy      = Manager.RequestGameplayTag(FName("Damage.Type.Holy"));
		Damage_Dark      = Manager.RequestGameplayTag(FName("Damage.Type.Dark"));
		Damage_Poison    = Manager.RequestGameplayTag(FName("Damage.Type.Poison"));
		Damage_Weave     = Manager.RequestGameplayTag(FName("Damage.Type.Weave"));
	}

	FGameplayTag GetDamageTag(ESOLODamageType Type)
	{
		switch (Type)
		{
		case ESOLODamageType::Slashing:  return Damage_Slashing;
		case ESOLODamageType::Blunt:     return Damage_Blunt;
		case ESOLODamageType::Piercing:  return Damage_Piercing;
		case ESOLODamageType::Fire:      return Damage_Fire;
		case ESOLODamageType::Ice:       return Damage_Ice;
		case ESOLODamageType::Electric:  return Damage_Electric;
		case ESOLODamageType::Holy:      return Damage_Holy;
		case ESOLODamageType::Dark:      return Damage_Dark;
		case ESOLODamageType::Poison:    return Damage_Poison;
		case ESOLODamageType::Weave:     return Damage_Weave;
		default:                         return Damage_Slashing;
		}
	}
}
