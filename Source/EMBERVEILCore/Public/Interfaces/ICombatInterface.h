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
class EMBERVEILCORE_API ICombatInterface
{
	GENERATED_BODY()

public:
	// Devuelve el nivel actual del actor (usado por GAS para escalar daño)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int32 GetCharacterLevel() const;

	// Punto de aparición de proyectiles / efectos de ataque
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag) const;

	// True si el personaje murió y está en animación de muerte
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsDead() const;

	// Llamado por GAS cuando el personaje muere
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void Die(AActor* InstigatorActor);

	// Devuelve el mesh principal del personaje (para VFX/hit reactions)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	USkeletalMeshComponent* GetCombatMesh() const;
};
