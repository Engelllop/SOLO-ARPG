#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_LevelUpRestore.generated.h"

/**
 * GE instantáneo: cura HP y MP de forma aditiva vía SetByCaller (Data.LevelUpRestoreHP / Mana).
 * Magnitudes = delta hasta el máximo, calculadas en servidor al subir de nivel.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEIL_GE_LevelUpRestore : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UEMBERVEIL_GE_LevelUpRestore();
};
