#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "SOLOPlayerState.generated.h"

class UAbilitySystemComponent;
class USOLOAttributeSet;
class USOLOInventoryComponent;
class USOLOQuestComponent;
class USOLOCraftingComponent;
class USOLOSkillMasteryComponent;
class USOLOAbilityBarComponent;
class USOLOGuildComponent;

/**
 * PlayerState del jugador.
 * El ASC y el AttributeSet viven aquÃ­ (no en el Character) porque el PlayerState
 * persiste entre respawns y niveles, garantizando que los stats nunca se pierden.
 */
UCLASS()
class SOLOCORE_API ASOLOPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASOLOPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	USOLOAttributeSet* GetAttributeSet() const { return AttributeSet; }

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
	TObjectPtr<USOLOAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USOLOInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USOLOQuestComponent> QuestComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USOLOCraftingComponent> CraftingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USOLOSkillMasteryComponent> SkillMasteryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USOLOAbilityBarComponent> AbilityBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USOLOGuildComponent> GuildComponent;
};
