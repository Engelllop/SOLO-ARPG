#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SOLOSaveManager.generated.h"

class ASOLOPlayerCharacter;

UCLASS()
class SOLOCORE_API USOLOSaveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static constexpr int32 DefaultUserIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SavePlayerGame(ASOLOPlayerCharacter* Player, FString SlotName = TEXT("Slot1"));

	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadPlayerGame(ASOLOPlayerCharacter* Player, FString SlotName = TEXT("Slot1"));

	UFUNCTION(BlueprintCallable, Category = "Save")
	void AutoSave(ASOLOPlayerCharacter* Player);

	UFUNCTION(BlueprintPure, Category = "Save")
	bool DoesSaveExist(const FString& SlotName) const;
};
