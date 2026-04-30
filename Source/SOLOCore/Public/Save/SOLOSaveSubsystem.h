#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Save/SOLOSaveGame.h"
#include "SOLOSaveSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveCompleted, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadCompleted, bool, bSuccess);

UCLASS()
class SOLOCORE_API USOLOSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable) void SaveGame(int32 SlotIndex = 0);
	UFUNCTION(BlueprintCallable) void AutoSave();
	UFUNCTION(BlueprintCallable) bool LoadGame(int32 SlotIndex = 0);
	UFUNCTION(BlueprintCallable) void DeleteSave(int32 SlotIndex);
	UFUNCTION(BlueprintCallable) bool DoesSaveExist(int32 SlotIndex) const;

	USOLOSaveGame* GetCurrentSave() const { return CurrentSave; }

	UPROPERTY(BlueprintAssignable) FOnSaveCompleted OnSaveCompleted;
	UPROPERTY(BlueprintAssignable) FOnLoadCompleted OnLoadCompleted;

protected:
	void GatherSaveData();
	void ApplySaveData();
	void CreateBackup(int32 SlotIndex);

	static const int32 MaxBackups = 3;
	static const FString AutoSaveSlotName;

	UPROPERTY() USOLOSaveGame* CurrentSave = nullptr;

private:
	FTimerHandle AutoSaveTimer;
	UPROPERTY(EditDefaultsOnly) float AutoSaveIntervalSeconds = 300.f;
};
