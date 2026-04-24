#include "Player/EMBERVEILPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Quest/EMBERVEILQuestComponent.h"
#include "Crafting/EMBERVEILCraftingComponent.h"
#include "Skills/EMBERVEILSkillMasteryComponent.h"
#include "Skills/EMBERVEILAbilityBarComponent.h"
#include "Guild/EMBERVEILGuildComponent.h"

AEMBERVEILPlayerState::AEMBERVEILPlayerState()
{
	// El PlayerState se replica por defecto — aseguramos una frecuencia razonable
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// Mixed: el ASC se replica al owner con datos completos, al resto solo con GameplayTags y Events
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEMBERVEILAttributeSet>(TEXT("AttributeSet"));

	InventoryComponent = CreateDefaultSubobject<UEMBERVEILInventoryComponent>(TEXT("InventoryComponent"));
	QuestComponent = CreateDefaultSubobject<UEMBERVEILQuestComponent>(TEXT("QuestComponent"));
	CraftingComponent = CreateDefaultSubobject<UEMBERVEILCraftingComponent>(TEXT("CraftingComponent"));
	SkillMasteryComponent = CreateDefaultSubobject<UEMBERVEILSkillMasteryComponent>(TEXT("SkillMasteryComponent"));
	AbilityBarComponent = CreateDefaultSubobject<UEMBERVEILAbilityBarComponent>(TEXT("AbilityBarComponent"));
	GuildComponent = CreateDefaultSubobject<UEMBERVEILGuildComponent>(TEXT("GuildComponent"));
}

UAbilitySystemComponent* AEMBERVEILPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AEMBERVEILPlayerState::GetPlayerLevel() const
{
	return AttributeSet ? static_cast<int32>(AttributeSet->GetLevel()) : 1;
}

float AEMBERVEILPlayerState::GetHealth() const
{
	return AttributeSet ? AttributeSet->GetHealth() : 0.f;
}

float AEMBERVEILPlayerState::GetMaxHealth() const
{
	return AttributeSet ? AttributeSet->GetMaxHealth() : 1.f;
}

float AEMBERVEILPlayerState::GetMana() const
{
	return AttributeSet ? AttributeSet->GetMana() : 0.f;
}

float AEMBERVEILPlayerState::GetMaxMana() const
{
	return AttributeSet ? AttributeSet->GetMaxMana() : 1.f;
}
