#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Engine/GameInstance.h"
#include "EMBERVEILGameInstance.generated.h"

UENUM(BlueprintType)
enum class EEMBERVEILRace : uint8
{
	Human       UMETA(DisplayName = "Humano"),
	Elf         UMETA(DisplayName = "Elfo"),
	Dwarf       UMETA(DisplayName = "Enano"),
	HalfOrc     UMETA(DisplayName = "Medio-Orco"),
	Semimorph   UMETA(DisplayName = "Semihumano"),
	Demon       UMETA(DisplayName = "Demonio"),
	Fae         UMETA(DisplayName = "Feérico")
};

UENUM(BlueprintType)
enum class EEMBERVEILClass : uint8
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
	UPROPERTY(BlueprintReadWrite) EEMBERVEILRace Race    = EEMBERVEILRace::Human;
	UPROPERTY(BlueprintReadWrite) EEMBERVEILClass Class  = EEMBERVEILClass::Warrior;
	UPROPERTY(BlueprintReadWrite) int32 Level             = 1;
	UPROPERTY(BlueprintReadWrite) float TotalPlaytimeHours = 0.f;
	UPROPERTY(BlueprintReadWrite) int32 TowerFloorReached  = 0;
	UPROPERTY(BlueprintReadWrite) FString LastSaveLocation;
	UPROPERTY(BlueprintReadWrite) FDateTime LastSaved;
};

/**
 * GameInstance de EMBERVEIL — persiste durante toda la sesión (incluye cambios de nivel).
 * Guarda datos de personaje activo, configuración y caché de sesión de red.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEMBERVEILGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	// ─── Datos del personaje activo ───────────────────────────────────────────
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	FPlayerSaveData ActivePlayerData;

	// ─── Configuración de sesión online ──────────────────────────────────────
	UPROPERTY(BlueprintReadOnly, Category = "Online")
	FString CurrentSessionId;

	UPROPERTY(BlueprintReadOnly, Category = "Online")
	bool bIsOnlineSession = false;

	/** Sesión con credenciales Admin0..Admin10 + contraseña correcta (sin restricciones de gameplay). */
	UPROPERTY(BlueprintReadOnly, Category = "Admin")
	bool bAdminUnrestrictedAccess = false;

	/** Nombre canónico del admin (p. ej. Admin3); vacío si no aplica. */
	UPROPERTY(BlueprintReadOnly, Category = "Admin")
	FString AdminSessionAccountName;

	UFUNCTION(BlueprintPure, Category = "Admin")
	bool HasAdminUnrestrictedAccess() const { return bAdminUnrestrictedAccess; }

	/** Llamar al cerrar sesión o antes de un login no admin. */
	UFUNCTION(BlueprintCallable, Category = "Admin")
	void ClearAdminSession();

	UFUNCTION(BlueprintCallable, Category = "Admin")
	void SetAdminSession(bool bEnabled, const FString& CanonicalAdminName);

	// ─── Helpers ──────────────────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetActivePlayerData(const FPlayerSaveData& Data) { ActivePlayerData = Data; }

	UFUNCTION(BlueprintCallable, Category = "Character")
	const FPlayerSaveData& GetActivePlayerData() const { return ActivePlayerData; }

	UFUNCTION(BlueprintCallable, Category = "Travel")
	void TravelToMap(const FString& MapPath, bool bAbsolute = false);

	/** Mapa de gameplay por defecto (paquete `/Game/...` sin extensión). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maps")
	FString GameplayMapPackage = TEXT("/Game/Maps/L_OpenWorld");

	/** Abre el mapa de gameplay y fuerza el GameMode de combate/exploración. */
	UFUNCTION(BlueprintCallable, Category = "Travel")
	void TravelToGameplayMap();

	// Retorna el ID de usuario local (para guardado y EOS)
	UFUNCTION(BlueprintCallable, Category = "Online")
	FString GetLocalUserId() const;
};
