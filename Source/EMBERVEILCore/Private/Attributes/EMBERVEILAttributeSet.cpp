#include "Attributes/EMBERVEILAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "Player/EMBERVEILPlayerState.h"
#include "Skills/EMBERVEILAbilityBarComponent.h"
#include "AbilitySystem/GameplayEffects/GE_LevelUpRestore.h"
#include "UI/EMBERVEILNotificationSystem.h"
#include "Player/EMBERVEILPlayerController.h"
#include "Character/EMBERVEILPlayerCharacter.h"

UEMBERVEILAttributeSet::UEMBERVEILAttributeSet()
{
	// Valores base — los GameplayEffects de inicio de partida sobreescriben estos
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
	InitCritMultiplier(1.5f); // x1.5 daño en crítico
	InitMoveSpeed(600.f);
	InitAttackSpeed(1.0f);
	InitCooldownReduction(0.f);

	InitLevel(1.f);
	InitExperience(0.f);
	InitExperienceToNextLevel(GetExperienceRequiredToAdvanceFromLevel(1));
	InitUnspentStatPoints(0.f);
	InitUnspentTalentPoints(0.f);
}

float UEMBERVEILAttributeSet::GetExperienceRequiredToAdvanceFromLevel(int32 CurrentLevel)
{
	CurrentLevel = FMath::Clamp(CurrentLevel, 1, 100);
	return FMath::RoundToFloat(100.f * FMath::Pow(static_cast<float>(CurrentLevel), 1.8f));
}

void UEMBERVEILAttributeSet::FlushExperienceLevelUps()
{
	ProcessExperienceLevelUps();
}

void UEMBERVEILAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Health,               COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, MaxHealth,            COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Mana,                 COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, MaxMana,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Stamina,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, MaxStamina,           COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Strength,             COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Dexterity,            COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Intelligence,         COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Vitality,             COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Agility,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Spirit,               COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Luck,                 COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, AttackDamage,         COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, MagicDamage,          COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Defense,              COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, MagicResistance,      COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, CritChance,           COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, CritMultiplier,       COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, MoveSpeed,            COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, AttackSpeed,          COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, CooldownReduction,    COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Level,                COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, Experience,           COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, ExperienceToNextLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, UnspentStatPoints,    COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEMBERVEILAttributeSet, UnspentTalentPoints,  COND_None, REPNOTIFY_Always);
}

void UEMBERVEILAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UEMBERVEILAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	AActor* TargetActor = Data.Target.GetAvatarActor();

	// ─── Daño entrante ────────────────────────────────────────────────────────
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalDamage = GetIncomingDamage();
		SetIncomingDamage(0.f); // Limpiamos el meta-atributo

		if (LocalDamage > 0.f)
		{
			// Apply damage directly (Mitigation is now handled purely in EMBERVEILDamageExecCalc)
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
				if (AEMBERVEILPlayerController* PC = Cast<AEMBERVEILPlayerController>(Source->AbilityActorInfo->PlayerController.Get()))
				{
					// Notify Client to show floating damage number
					PC->Client_ShowDamageNumber(LocalDamage, TargetActor->GetActorLocation(), DamageType, bIsCritical);
					
					// Increment Combo if avatar is player
					if (AEMBERVEILPlayerCharacter* PlayerChar = Cast<AEMBERVEILPlayerCharacter>(Source->GetAvatarActor()))
					{
						PlayerChar->OnBasicAttackLanded();
					}
				}
			}

			if (TargetActor)
			{
				if (AEMBERVEILEnemyCharacter* BossEnemy = Cast<AEMBERVEILEnemyCharacter>(TargetActor))
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
				if (AEMBERVEILEnemyCharacter* Enemy = Cast<AEMBERVEILEnemyCharacter>(TargetActor))
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

	// ─── Curación entrante ────────────────────────────────────────────────────
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

	// ─── Experiencia entrante (meta-atributo) ─────────────────────────────────
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

	// ─── Experience modificada directamente por un GE (p. ej. kill XP) ───────
	if (Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		ProcessExperienceLevelUps();
	}

	// ─── Clampeo post-modificación de MaxHealth/MaxMana/MaxStamina ───────────
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
		SetHealth(FMath::Min(GetHealth(), GetMaxHealth()));

	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
		SetMana(FMath::Min(GetMana(), GetMaxMana()));

	if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
		SetStamina(FMath::Min(GetStamina(), GetMaxStamina()));
}

void UEMBERVEILAttributeSet::ProcessExperienceLevelUps()
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

		// Restauración HP/MP al subir de nivel
		const float HealHP = GetMaxHealth() - GetHealth();
		const float HealMana = GetMaxMana() - GetMana();
		if (HealHP > KINDA_SMALL_NUMBER || HealMana > KINDA_SMALL_NUMBER)
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			const FGameplayEffectSpecHandle RestoreSpec =
				ASC->MakeOutgoingSpec(UEMBERVEIL_GE_LevelUpRestore::StaticClass(), NewLevelInt, Ctx);
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
				UEMBERVEILNotificationSystem::NotifyLevelUpOwningClient(PC, NewLevelInt);
			}
		}

		if (NewLevelInt == 5 || NewLevelInt == 10 || NewLevelInt == 20 || NewLevelInt == 30)
		{
			if (AEMBERVEILPlayerState* PS = Cast<AEMBERVEILPlayerState>(ASC->GetOwnerActor()))
			{
				if (UEMBERVEILAbilityBarComponent* Bar = PS->FindComponentByClass<UEMBERVEILAbilityBarComponent>())
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

void UEMBERVEILAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())             NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	else if (Attribute == GetManaAttribute())          NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	else if (Attribute == GetStaminaAttribute())       NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	else if (Attribute == GetMoveSpeedAttribute())     NewValue = FMath::Max(NewValue, 0.f);
	else if (Attribute == GetAttackSpeedAttribute())   NewValue = FMath::Clamp(NewValue, 0.1f, 5.f);
	else if (Attribute == GetCritChanceAttribute())    NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	else if (Attribute == GetCooldownReductionAttribute()) NewValue = FMath::Clamp(NewValue, 0.f, 0.75f); // máx 75% CDR
	else if (Attribute == GetLevelAttribute())         NewValue = FMath::Clamp(NewValue, 1.f, 100.f);
}

// ─── OnRep implementations ────────────────────────────────────────────────────

#define IMPLEMENT_ONREP(AttributeName) \
void UEMBERVEILAttributeSet::OnRep_##AttributeName(const FGameplayAttributeData& Old##AttributeName) \
{ GAMEPLAYATTRIBUTE_REPNOTIFY(UEMBERVEILAttributeSet, AttributeName, Old##AttributeName); }

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
