#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ICombatInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface { GENERATED_BODY() };

/**
 * Interfaz que implementan todos los actores que participan en combate.
 * Permite que habilidades y efectos pregunten por datos de combate
 * sin depender de una clase concreta.
 */
class SOLOCORE_API ICombatInterface
{
	GENERATED_BODY()

public:
	// Devuelve el nivel actual del actor (usado por GAS para escalar daÃ±o)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int32 GetCharacterLevel() const;

	// Punto de apariciÃ³n de proyectiles / efectos de ataque
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag) const;

	// True si el personaje muriÃ³ y estÃ¡ en animaciÃ³n de muerte
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsDead() const;

	// Llamado por GAS cuando el personaje muere
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void Die(AActor* InstigatorActor);

	// Devuelve el mesh principal del personaje (para VFX/hit reactions)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	USkeletalMeshComponent* GetCombatMesh() const;
};
