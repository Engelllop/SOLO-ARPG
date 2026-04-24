#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EMBERVEILSaveManager.generated.h"

class AEMBERVEILPlayerCharacter;

UCLASS()
class EMBERVEILCORE_API UEMBERVEILSaveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static constexpr int32 DefaultUserIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SavePlayerGame(AEMBERVEILPlayerCharacter* Player, FString SlotName = TEXT("Slot1"));

	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadPlayerGame(AEMBERVEILPlayerCharacter* Player, FString SlotName = TEXT("Slot1"));

	UFUNCTION(BlueprintCallable, Category = "Save")
	void AutoSave(AEMBERVEILPlayerCharacter* Player);

	UFUNCTION(BlueprintPure, Category = "Save")
	bool DoesSaveExist(const FString& SlotName) const;
};
