#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Engine/GameInstance.h"
#include "SOLOGameInstance.generated.h"

UENUM(BlueprintType)
enum class ESOLORace : uint8
{
	Human       UMETA(DisplayName = "Humano"),
	Elf         UMETA(DisplayName = "Elfo"),
	Dwarf       UMETA(DisplayName = "Enano"),
	HalfOrc     UMETA(DisplayName = "Medio-Orco"),
	Semimorph   UMETA(DisplayName = "Semihumano"),
	Demon       UMETA(DisplayName = "Demonio"),
	Fae         UMETA(DisplayName = "FeÃ©rico")
};

UENUM(BlueprintType)
enum class ESOLOClass : uint8
{
	Warrior     UMETA(DisplayName = "Guerrero"),
	Mage        UMETA(DisplayName = "Mago"),
	Archer      UMETA(DisplayName = "Arquero"),
	Healer      UMETA(DisplayName = "Curandero"),
	Blacksmith  UMETA(DisplayName = "Herrero")
};

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) FString CharacterName;
	UPROPERTY(BlueprintReadWrite) ESOLORace Race    = ESOLORace::Human;
	UPROPERTY(BlueprintReadWrite) ESOLOClass Class  = ESOLOClass::Warrior;
	UPROPERTY(BlueprintReadWrite) int32 Level             = 1;
	UPROPERTY(BlueprintReadWrite) float TotalPlaytimeHours = 0.f;
	UPROPERTY(BlueprintReadWrite) int32 TowerFloorReached  = 0;
	UPROPERTY(BlueprintReadWrite) FString LastSaveLocation;
	UPROPERTY(BlueprintReadWrite) FDateTime LastSaved;
};

/**
 * GameInstance de SOLO â€” persiste durante toda la sesiÃ³n (incluye cambios de nivel).
 * Guarda datos de personaje activo, configuraciÃ³n y cachÃ© de sesiÃ³n de red.
 */
UCLASS()
class SOLOCORE_API USOLOGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USOLOGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	// â”€â”€â”€ Datos del personaje activo â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	FPlayerSaveData ActivePlayerData;

	// â”€â”€â”€ ConfiguraciÃ³n de sesiÃ³n online â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	UPROPERTY(BlueprintReadOnly, Category = "Online")
	FString CurrentSessionId;

	UPROPERTY(BlueprintReadOnly, Category = "Online")
	bool bIsOnlineSession = false;

	/** SesiÃ³n con credenciales Admin0..Admin10 + contraseÃ±a correcta (sin restricciones de gameplay). */
	UPROPERTY(BlueprintReadOnly, Category = "Admin")
	bool bAdminUnrestrictedAccess = false;

	/** Nombre canÃ³nico del admin (p. ej. Admin3); vacÃ­o si no aplica. */
	UPROPERTY(BlueprintReadOnly, Category = "Admin")
	FString AdminSessionAccountName;

	UFUNCTION(BlueprintPure, Category = "Admin")
	bool HasAdminUnrestrictedAccess() const { return bAdminUnrestrictedAccess; }

	/** Llamar al cerrar sesiÃ³n o antes de un login no admin. */
	UFUNCTION(BlueprintCallable, Category = "Admin")
	void ClearAdminSession();

	UFUNCTION(BlueprintCallable, Category = "Admin")
	void SetAdminSession(bool bEnabled, const FString& CanonicalAdminName);

	// ─── Story Flags ────────────────────────────────────────────────────────────

	UFUNCTION(BlueprintCallable, Category = "Story")
	void SetStoryFlag(FName FlagName, bool bValue);

	UFUNCTION(BlueprintPure, Category = "Story")
	bool HasStoryFlag(FName FlagName) const;

	UFUNCTION(BlueprintCallable, Category = "Story")
	void ClearAllStoryFlags();

	UFUNCTION(BlueprintPure, Category = "Story")
	const TMap<FName, bool>& GetAllStoryFlags() const { return StoryFlags; }

	// ─── Helpers ────────────────────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetActivePlayerData(const FPlayerSaveData& Data) { ActivePlayerData = Data; }

	UFUNCTION(BlueprintCallable, Category = "Character")
	const FPlayerSaveData& GetActivePlayerData() const { return ActivePlayerData; }

	UFUNCTION(BlueprintCallable, Category = "Travel")
	void TravelToMap(const FString& MapPath, bool bAbsolute = false);

	/** Mapa de gameplay por defecto (paquete `/Game/...` sin extensiÃ³n). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maps")
	FString GameplayMapPackage = TEXT("/Game/Maps/L_OpenWorld");

	/** Abre el mapa de gameplay y fuerza el GameMode de combate/exploraciÃ³n. */
	UFUNCTION(BlueprintCallable, Category = "Travel")
	void TravelToGameplayMap();

	// Retorna el ID de usuario local (para guardado y EOS)
	UFUNCTION(BlueprintCallable, Category = "Online")
	FString GetLocalUserId() const;

private:
	UPROPERTY()
	TMap<FName, bool> StoryFlags;
};
