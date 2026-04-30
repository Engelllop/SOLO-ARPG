#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_BattleLogWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UWidgetAnimation;

UENUM(BlueprintType)
enum class EBattleLogEntryType : uint8
{
	Damage,
	Heal,
	Buff,
	System,
	Miss
};

USTRUCT(BlueprintType)
struct SOLOCORE_API FBattleLogEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "BattleLog")
	FText Message;

	UPROPERTY(BlueprintReadWrite, Category = "BattleLog")
	EBattleLogEntryType Type = EBattleLogEntryType::Damage;

	UPROPERTY(BlueprintReadWrite, Category = "BattleLog")
	float Timestamp = 0.f;
};

/**
 * Battle log widget con hover expandible.
 * Por defecto muestra solo los Ãºltimos 2 mensajes.
 * Al hacer hover expande a lista completa con colores por tipo.
 * Estilo sketchy â€” igual que el quest log.
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API UWG_BattleLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/** AÃ±ade una entrada al log y descarta las mÃ¡s viejas si excede el lÃ­mite */
	UFUNCTION(BlueprintCallable, Category = "BattleLog")
	void AddEntry(const FBattleLogEntry& Entry);

	/** Limpia todo el log */
	UFUNCTION(BlueprintCallable, Category = "BattleLog")
	void ClearLog();

	/** NÃºmero mÃ¡ximo de entradas que se mantienen en el log */
	UPROPERTY(EditAnywhere, Category = "BattleLog")
	int32 MaxEntries = 50;

	/** NÃºmero de entradas visibles en modo colapsado */
	UPROPERTY(EditAnywhere, Category = "BattleLog")
	int32 CollapsedVisibleCount = 2;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> LogContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HintText; // "[Tab] ver log completo"

	UPROPERTY(EditAnywhere, Category = "BattleLog|Style")
	FLinearColor DamageColor = FLinearColor(0.91f, 0.49f, 0.42f, 0.8f); // #e87c6a

	UPROPERTY(EditAnywhere, Category = "BattleLog|Style")
	FLinearColor HealColor = FLinearColor(0.42f, 0.66f, 0.49f, 0.8f); // #6aa87c

	UPROPERTY(EditAnywhere, Category = "BattleLog|Style")
	FLinearColor BuffColor = FLinearColor(0.96f, 0.85f, 0.48f, 0.8f); // #f5d87a

	UPROPERTY(EditAnywhere, Category = "BattleLog|Style")
	FLinearColor SystemColor = FLinearColor(1.f, 1.f, 1.f, 0.35f);

	UPROPERTY(EditAnywhere, Category = "BattleLog|Style")
	FLinearColor MissColor = FLinearColor(0.7f, 0.7f, 0.7f, 0.4f);

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> ExpandAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> CollapseAnimation;

	UPROPERTY(Transient)
	TArray<FBattleLogEntry> Entries;

private:
	bool bExpanded = false;

	void RebuildVisible();
	FLinearColor GetColorForType(EBattleLogEntryType Type) const;
	float GetOpacityForIndex(int32 Index) const;
};
