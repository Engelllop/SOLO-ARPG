#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EMBERVEILProfileSubsystem.generated.h"

class UEMBERVEILLocalProfileSave;

/**
 * Perfil local: nombre visible, persistencia en slot de SaveGame.
 * No es "login" online; es identidad en máquina antes de entrar al gameplay.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILProfileSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Profile")
	FString GetDisplayName() const { return CachedDisplayName; }

	UFUNCTION(BlueprintCallable, Category = "Profile")
	bool HasValidDisplayName() const;

	UFUNCTION(BlueprintCallable, Category = "Profile")
	void SetDisplayNameAndPersist(const FString& InName);

	/**
	 * Finaliza el paso del menú: nombre visible + contraseña opcional (solo para Admin0..Admin10).
	 * Si la contraseña no está vacía, debe ser login admin válido o se rechaza.
	 * (Solo C++; evita UFUNCTION con FString& de salida que rompe UHT/Blueprint.)
	 */
	bool TryCompleteMainMenuLogin(const FString& DisplayName, const FString& OptionalPassword, FString& OutError);

private:
	void LoadFromDisk();
	void SaveToDisk();
	FString SanitizeDisplayName(const FString& Raw) const;

	UPROPERTY()
	FString CachedDisplayName;

	static const TCHAR* SaveSlotName;
	static constexpr int32 SaveUserIndex = 0;
};
