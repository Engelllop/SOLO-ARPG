#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Inventory/SOLOItemTypes.h"
#include "SOLORuneObject.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class SOLOCORE_API USOLORuneObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FName RuneID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FText RuneName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTag RuneTag;
	// GE applied to the item owner while this rune is socketed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TSubclassOf<class UGameplayEffect> RuneEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) ESOLOItemRarity RequiredRarity = ESOLOItemRarity::Rare;
};
