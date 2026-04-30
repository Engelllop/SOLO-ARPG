#include "Player/SOLOPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Quest/SOLOQuestComponent.h"
#include "Crafting/SOLOCraftingComponent.h"
#include "Skills/SOLOSkillMasteryComponent.h"
#include "Skills/SOLOAbilityBarComponent.h"
#include "Guild/SOLOGuildComponent.h"

ASOLOPlayerState::ASOLOPlayerState()
{
	// El PlayerState se replica por defecto â€” aseguramos una frecuencia razonable
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// Mixed: el ASC se replica al owner con datos completos, al resto solo con GameplayTags y Events
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USOLOAttributeSet>(TEXT("AttributeSet"));

	InventoryComponent = CreateDefaultSubobject<USOLOInventoryComponent>(TEXT("InventoryComponent"));
	QuestComponent = CreateDefaultSubobject<USOLOQuestComponent>(TEXT("QuestComponent"));
	CraftingComponent = CreateDefaultSubobject<USOLOCraftingComponent>(TEXT("CraftingComponent"));
	SkillMasteryComponent = CreateDefaultSubobject<USOLOSkillMasteryComponent>(TEXT("SkillMasteryComponent"));
	AbilityBarComponent = CreateDefaultSubobject<USOLOAbilityBarComponent>(TEXT("AbilityBarComponent"));
	GuildComponent = CreateDefaultSubobject<USOLOGuildComponent>(TEXT("GuildComponent"));
}

UAbilitySystemComponent* ASOLOPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 ASOLOPlayerState::GetPlayerLevel() const
{
	return AttributeSet ? static_cast<int32>(AttributeSet->GetLevel()) : 1;
}

float ASOLOPlayerState::GetHealth() const
{
	return AttributeSet ? AttributeSet->GetHealth() : 0.f;
}

float ASOLOPlayerState::GetMaxHealth() const
{
	return AttributeSet ? AttributeSet->GetMaxHealth() : 1.f;
}

float ASOLOPlayerState::GetMana() const
{
	return AttributeSet ? AttributeSet->GetMana() : 0.f;
}

float ASOLOPlayerState::GetMaxMana() const
{
	return AttributeSet ? AttributeSet->GetMaxMana() : 1.f;
}
