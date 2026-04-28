#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_StatusEffectsWidget.generated.h"

class UHorizontalBox;
class UTexture2D;

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect")
	FText EffectName;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect")
	FLinearColor TintColor = FLinearColor::White;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect")
	float RemainingDuration = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect")
	float MaxDuration = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect")
	bool bIsBuff = true; // false = debuff

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect")
	FString IconEmoji; // fallback si no hay texture
};

/**
 * Status effects widget — muestra buffs/debuffs activos como iconos pequeños.
 * Aparece junto a las vital bars o en posición configurable.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API UWG_StatusEffectsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** Reemplaza todos los efectos activos */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects")
	void SetEffects(const TArray<FStatusEffectData>& Effects);

	/** Añade o actualiza un efecto individual */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects")
	void AddOrUpdateEffect(const FStatusEffectData& Effect);

	/** Elimina un efecto por nombre */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects")
	void RemoveEffect(const FString& EffectName);

	/** Limpia todos los efectos */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects")
	void ClearEffects();

	/** Número máximo de iconos visibles */
	UPROPERTY(EditAnywhere, Category = "StatusEffects")
	int32 MaxVisibleIcons = 6;

	/** Tamaño de cada icono */
	UPROPERTY(EditAnywhere, Category = "StatusEffects")
	float IconSize = 18.f;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> EffectsContainer;

	UPROPERTY(Transient)
	TArray<FStatusEffectData> ActiveEffects;

private:
	void RebuildIcons();
	FLinearColor GetBorderColorForEffect(const FStatusEffectData& Effect) const;
};
