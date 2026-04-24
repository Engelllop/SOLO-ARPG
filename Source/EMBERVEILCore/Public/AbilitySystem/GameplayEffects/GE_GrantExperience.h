#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_GrantExperience.generated.h"

/**
 * GE instantáneo: añade Experience (aditivo) usando SetByCaller con la tag Data.XPReward.
 * Valor por defecto si no se asigna en el spec: usar el magnitude en C++ al aplicar.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEIL_GE_GrantExperience : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UEMBERVEIL_GE_GrantExperience();
};
