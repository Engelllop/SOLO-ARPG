#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_LevelUpRestore.generated.h"

/**
 * GE instantÃ¡neo: cura HP y MP de forma aditiva vÃ­a SetByCaller (Data.LevelUpRestoreHP / Mana).
 * Magnitudes = delta hasta el mÃ¡ximo, calculadas en servidor al subir de nivel.
 */
UCLASS()
class SOLOCORE_API USOLO_GE_LevelUpRestore : public UGameplayEffect
{
	GENERATED_BODY()

public:
	USOLO_GE_LevelUpRestore();
};
