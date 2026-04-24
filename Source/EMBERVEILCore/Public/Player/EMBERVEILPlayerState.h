#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "EMBERVEILPlayerState.generated.h"

class UAbilitySystemComponent;
class UEMBERVEILAttributeSet;
class UEMBERVEILInventoryComponent;
class UEMBERVEILQuestComponent;
class UEMBERVEILCraftingComponent;
class UEMBERVEILSkillMasteryComponent;
class UEMBERVEILAbilityBarComponent;
class UEMBERVEILGuildComponent;

/**
 * PlayerState del jugador.
 * El ASC y el AttributeSet viven aquí (no en el Character) porque el PlayerState
 * persiste entre respawns y niveles, garantizando que los stats nunca se pierden.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEMBERVEILPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UEMBERVEILAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Helpers para BP y HUD
	UFUNCTION(BlueprintCallable, Category = "PlayerState|Stats")
	int32 GetPlayerLevel() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Stats")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Stats")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Stats")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Stats")
	float GetMaxMana() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UEMBERVEILAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEMBERVEILInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEMBERVEILQuestComponent> QuestComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEMBERVEILCraftingComponent> CraftingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEMBERVEILSkillMasteryComponent> SkillMasteryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEMBERVEILAbilityBarComponent> AbilityBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEMBERVEILGuildComponent> GuildComponent;
};
