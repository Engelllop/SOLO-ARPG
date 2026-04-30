#include "Attributes/SOLOAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Character/SOLOEnemyCharacter.h"
#include "Player/SOLOPlayerState.h"
#include "Skills/SOLOAbilityBarComponent.h"
#include "AbilitySystem/GameplayEffects/GE_LevelUpRestore.h"
#include "UI/SOLONotificationSystem.h"
#include "Player/SOLOPlayerController.h"
#include "Character/SOLOPlayerCharacter.h"

USOLOAttributeSet::USOLOAttributeSet()
{
	// Valores base â€” los GameplayEffects de inicio de partida sobreescriben estos
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);

	InitStrength(10.f);
	InitDexterity(10.f);
	InitIntelligence(10.f);
	InitVitality(10.f);
	InitAgility(10.f);
	InitSpirit(10.f);
	InitLuck(10.f);

	InitAttackDamage(15.f);
	InitMagicDamage(10.f);
	InitDefense(5.f);
	InitMagicResistance(5.f);
	InitCritChance(0.05f);   // 5%
	InitCritMultiplier(1.5f); // x1.5 daÃ±o en crÃ­tico
	InitMoveSpeed(600.f);
	InitAttackSpeed(1.0f);
	InitCooldownReduction(0.f);

	InitLevel(1.f);
	InitExperience(0.f);
	InitExperienceToNextLevel(GetExperienceRequiredToAdvanceFromLevel(1));
	InitUnspentStatPoints(0.f);
	InitUnspentTalentPoints(0.f);
}

float USOLOAttributeSet::GetExperienceRequiredToAdvanceFromLevel(int32 CurrentLevel)
{
	CurrentLevel = FMath::Clamp(CurrentLevel, 1, 100);
	return FMath::RoundToFloat(100.f * FMath::Pow(static_cast<float>(CurrentLevel), 1.8f));
}

void USOLOAttributeSet::FlushExperienceLevelUps()
{
	ProcessExperienceLevelUps();
}

void USOLOAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Health,               COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, MaxHealth,            COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Mana,                 COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, MaxMana,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Stamina,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, MaxStamina,           COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Strength,             COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Dexterity,            COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Intelligence,         COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Vitality,             COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Agility,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Spirit,               COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Luck,                 COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, AttackDamage,         COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, MagicDamage,          COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Defense,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, MagicResistance,      COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, CritChance,           COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, CritMultiplier,       COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, MoveSpeed,            COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, AttackSpeed,          COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, CooldownReduction,    COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Level,                COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, Experience,           COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, ExperienceToNextLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, UnspentStatPoints,    COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USOLOAttributeSet, UnspentTalentPoints,  COND_None, REPNOTIFY_Always);
}

void USOLOAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void USOLOAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	AActor* TargetActor = Data.Target.GetAvatarActor();

	// â”€â”€â”€ DaÃ±o entrante â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalDamage = GetIncomingDamage();
		SetIncomingDamage(0.f); // Limpiamos el meta-atributo

		if (LocalDamage > 0.f)
		{
			// Apply damage directly (Mitigation is now handled purely in SOLODamageExecCalc)
			const float NewHealth = FMath::Max(GetHealth() - LocalDamage, 0.f);
			SetHealth(NewHealth);

			bool bIsCritical = Data.EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.IsCritical")), false, 0.f) > 0.f;

			uint8 DamageType = 0; // 0 = Physical
			if (Data.EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Fire")), false, 0.f) > 0.f) DamageType = 1;
			else if (Data.EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Ice")), false, 0.f) > 0.f) DamageType = 2;
			else if (Data.EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Lightning")), false, 0.f) > 0.f) DamageType = 3;
			else if (Data.EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Poison")), false, 0.f) > 0.f) DamageType = 4;
			else if (Data.EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Dark")), false, 0.f) > 0.f) DamageType = 5;
			else if (Data.EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Holy")), false, 0.f) > 0.f) DamageType = 6;

			if (Source && TargetActor)
			{
				if (ASOLOPlayerController* PC = Cast<ASOLOPlayerController>(Source->AbilityActorInfo->PlayerController.Get()))
				{
					// Notify Client to show floating damage number
					PC->Client_ShowDamageNumber(LocalDamage, TargetActor->GetActorLocation(), DamageType, bIsCritical);
					
					// Increment Combo if avatar is player
					if (ASOLOPlayerCharacter* PlayerChar = Cast<ASOLOPlayerCharacter>(Source->GetAvatarActor()))
					{
						PlayerChar->OnBasicAttackLanded();
					}
				}
			}

			if (TargetActor)
			{
				if (ASOLOEnemyCharacter* BossEnemy = Cast<ASOLOEnemyCharacter>(TargetActor))
				{
					if (BossEnemy->IsBossEnemy())
					{
						const float MaxHP = GetMaxHealth();
						if (MaxHP > KINDA_SMALL_NUMBER)
						{
							BossEnemy->CheckBossPhase(GetHealth() / MaxHP);
						}
					}
				}
			}

			if (NewHealth <= 0.f && TargetActor)
			{
				if (ASOLOEnemyCharacter* Enemy = Cast<ASOLOEnemyCharacter>(TargetActor))
				{
					AActor* InstigatorAvatar = Source ? Source->GetAvatarActor() : nullptr;
					Enemy->RememberKillInstigator(InstigatorAvatar);
				}

				// Notificar muerte via GameplayEvent para que las abilities lo manejen
				FGameplayEventData EventData;
				EventData.Instigator = Source ? Source->GetAvatarActor() : nullptr;
				EventData.Target     = TargetActor;
				Data.Target.HandleGameplayEvent(
					FGameplayTag::RequestGameplayTag(FName("Event.Character.Death")),
					&EventData
				);
			}
		}
	}

	// â”€â”€â”€ CuraciÃ³n entrante â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	if (Data.EvaluatedData.Attribute == GetIncomingHealingAttribute())
	{
		const float LocalHealing = GetIncomingHealing();
		SetIncomingHealing(0.f);

		if (LocalHealing > 0.f)
		{
			const float NewHealth = FMath::Min(GetHealth() + LocalHealing, GetMaxHealth());
			SetHealth(NewHealth);
		}
	}

	// â”€â”€â”€ Experiencia entrante (meta-atributo) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	if (Data.EvaluatedData.Attribute == GetIncomingExperienceAttribute())
	{
		const float LocalXP = GetIncomingExperience();
		SetIncomingExperience(0.f);

		if (LocalXP > 0.f)
		{
			const float NewXP = GetExperience() + LocalXP;
			SetExperience(NewXP);
			ProcessExperienceLevelUps();
		}
	}

	// â”€â”€â”€ Experience modificada directamente por un GE (p. ej. kill XP) â”€â”€â”€â”€â”€â”€â”€
	if (Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		ProcessExperienceLevelUps();
	}

	// â”€â”€â”€ Clampeo post-modificaciÃ³n de MaxHealth/MaxMana/MaxStamina â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
		SetHealth(FMath::Min(GetHealth(), GetMaxHealth()));

	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
		SetMana(FMath::Min(GetMana(), GetMaxMana()));

	if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
		SetStamina(FMath::Min(GetStamina(), GetMaxStamina()));
}

void USOLOAttributeSet::ProcessExperienceLevelUps()
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (!ASC || !ASC->GetOwnerActor() || !ASC->GetOwnerActor()->HasAuthority())
	{
		return;
	}

	constexpr float MaxLevelF = 100.f;

	while (GetLevel() < MaxLevelF)
	{
		const int32 CurLevel = FMath::Clamp(FMath::FloorToInt(GetLevel()), 1, 99);
		const float Need = GetExperienceRequiredToAdvanceFromLevel(CurLevel);
		if (GetExperience() + KINDA_SMALL_NUMBER < Need)
		{
			break;
		}

		const float Remainder = GetExperience() - Need;
		SetExperience(FMath::Max(0.f, Remainder));
		SetLevel(GetLevel() + 1.f);
		
		// Otorga Puntos de Stat y Talento (Fase 1 Plan)
		SetUnspentStatPoints(GetUnspentStatPoints() + 5.f);
		SetUnspentTalentPoints(GetUnspentTalentPoints() + 1.f);

		const int32 NewLevelInt = FMath::FloorToInt(GetLevel());

		// RestauraciÃ³n HP/MP al subir de nivel
		const float HealHP = GetMaxHealth() - GetHealth();
		const float HealMana = GetMaxMana() - GetMana();
		if (HealHP > KINDA_SMALL_NUMBER || HealMana > KINDA_SMALL_NUMBER)
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			const FGameplayEffectSpecHandle RestoreSpec =
				ASC->MakeOutgoingSpec(USOLO_GE_LevelUpRestore::StaticClass(), NewLevelInt, Ctx);
			if (RestoreSpec.IsValid())
			{
				RestoreSpec.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(FName("Data.LevelUpRestoreHP")), HealHP);
				RestoreSpec.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(FName("Data.LevelUpRestoreMana")), HealMana);
				ASC->ApplyGameplayEffectSpecToSelf(*RestoreSpec.Data.Get());
			}
		}

		if (APawn* Avatar = Cast<APawn>(ASC->GetAvatarActor()))
		{
			if (APlayerController* PC = Cast<APlayerController>(Avatar->GetController()))
			{
				USOLONotificationSystem::NotifyLevelUpOwningClient(PC, NewLevelInt);
			}
		}

		if (NewLevelInt == 5 || NewLevelInt == 10 || NewLevelInt == 20 || NewLevelInt == 30)
		{
			if (ASOLOPlayerState* PS = Cast<ASOLOPlayerState>(ASC->GetOwnerActor()))
			{
				if (USOLOAbilityBarComponent* Bar = PS->FindComponentByClass<USOLOAbilityBarComponent>())
				{
					Bar->UnlockSlot(Bar->GetUnlockedSlots());
					Bar->UnlockSlot(Bar->GetUnlockedSlots());
				}
			}
		}
	}

	const int32 L = FMath::Clamp(FMath::FloorToInt(GetLevel()), 1, 100);
	if (L >= 100)
	{
		SetExperienceToNextLevel(0.f);
	}
	else
	{
		SetExperienceToNextLevel(GetExperienceRequiredToAdvanceFromLevel(L));
	}
}

void USOLOAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())             NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	else if (Attribute == GetManaAttribute())          NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	else if (Attribute == GetStaminaAttribute())       NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	else if (Attribute == GetMoveSpeedAttribute())     NewValue = FMath::Max(NewValue, 0.f);
	else if (Attribute == GetAttackSpeedAttribute())   NewValue = FMath::Clamp(NewValue, 0.1f, 5.f);
	else if (Attribute == GetCritChanceAttribute())    NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	else if (Attribute == GetCooldownReductionAttribute()) NewValue = FMath::Clamp(NewValue, 0.f, 0.75f); // mÃ¡x 75% CDR
	else if (Attribute == GetLevelAttribute())         NewValue = FMath::Clamp(NewValue, 1.f, 100.f);
}

// â”€â”€â”€ OnRep implementations â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

#define IMPLEMENT_ONREP(AttributeName) \
void USOLOAttributeSet::OnRep_##AttributeName(const FGameplayAttributeData& Old##AttributeName) \
{ GAMEPLAYATTRIBUTE_REPNOTIFY(USOLOAttributeSet, AttributeName, Old##AttributeName); }

IMPLEMENT_ONREP(Health)
IMPLEMENT_ONREP(MaxHealth)
IMPLEMENT_ONREP(Mana)
IMPLEMENT_ONREP(MaxMana)
IMPLEMENT_ONREP(Stamina)
IMPLEMENT_ONREP(MaxStamina)
IMPLEMENT_ONREP(Strength)
IMPLEMENT_ONREP(Dexterity)
IMPLEMENT_ONREP(Intelligence)
IMPLEMENT_ONREP(Vitality)
IMPLEMENT_ONREP(Agility)
IMPLEMENT_ONREP(Spirit)
IMPLEMENT_ONREP(Luck)
IMPLEMENT_ONREP(AttackDamage)
IMPLEMENT_ONREP(MagicDamage)
IMPLEMENT_ONREP(Defense)
IMPLEMENT_ONREP(MagicResistance)
IMPLEMENT_ONREP(CritChance)
IMPLEMENT_ONREP(CritMultiplier)
IMPLEMENT_ONREP(MoveSpeed)
IMPLEMENT_ONREP(AttackSpeed)
IMPLEMENT_ONREP(CooldownReduction)
IMPLEMENT_ONREP(Level)
IMPLEMENT_ONREP(Experience)
IMPLEMENT_ONREP(ExperienceToNextLevel)
IMPLEMENT_ONREP(UnspentStatPoints)
IMPLEMENT_ONREP(UnspentTalentPoints)

#undef IMPLEMENT_ONREP
