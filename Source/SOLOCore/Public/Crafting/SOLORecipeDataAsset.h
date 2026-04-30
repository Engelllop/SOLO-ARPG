#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Crafting/SOLOCraftingTypes.h"
#include "GameFramework/SOLOGameInstance.h"   // ESOLOClass
#include "SOLORecipeDataAsset.generated.h"

class UTexture2D;
class USOLOItemBase;

/**
 * DataAsset que define completamente una receta de crafteo.
 *
 * Crea instancias de esta clase en el Content Browser para cada receta del juego.
 * El AssetManager las registra bajo el tipo primario "CraftingRecipe".
 *
 * Convenciones de nomenclatura sugeridas para RecipeID:
 *   RECIPE_WEAPON_IRONSWORD_01
 *   RECIPE_ARMOR_LEATHERCHEST_01
 *   RECIPE_ALCHEMY_HEALTHPOTION_01
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API USOLORecipeDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    USOLORecipeDataAsset();

    // â”€â”€â”€ IDENTIDAD â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Identificador Ãºnico de la receta. Usado como clave en KnownRecipeIDs. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity")
    FName RecipeID;

    /** Nombre localizable mostrado en el panel de crafteo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity")
    FText RecipeName;

    /** DescripciÃ³n localizable (lore, tips de uso, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity")
    FText RecipeDescription;

    /** Icono de la receta para el panel de crafteo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity")
    TSoftObjectPtr<UTexture2D> RecipeIcon;

    /** CategorÃ­a de oficio a la que pertenece esta receta */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity")
    ECraftingCategory Category = ECraftingCategory::General;

    // â”€â”€â”€ INGREDIENTES Y RESULTADO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Lista de materiales necesarios para craftear esta receta */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Ingredients")
    TArray<FCraftingIngredient> Ingredients;

    /** Item que se produce al completar el crafteo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Result")
    TSoftObjectPtr<USOLOItemBase> ResultItem;

    /** Cantidad base producida por crafteo (sin crÃ­tico) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Result", meta = (ClampMin = "1"))
    int32 ResultQuantity = 1;

    /** Coste en monedas (herramientas, comisiÃ³n de taller, etc.). 0 = sin coste. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Result", meta = (ClampMin = "0"))
    int32 CraftingCurrencyCost = 0;

    // â”€â”€â”€ REQUISITOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Nivel mÃ­nimo del jugador para poder usar esta receta */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements", meta = (ClampMin = "1"))
    int32 RequiredPlayerLevel = 1;

    /**
     * Nivel mÃ­nimo de maestrÃ­a requerido (0 = sin requisito de maestrÃ­a).
     * Si es > 0, tambiÃ©n debe especificarse RequiredMasteryType.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements", meta = (ClampMin = "0"))
    int32 RequiredMasteryLevel = 0;

    /** Tipo de maestrÃ­a requerida. Ignorado si RequiredMasteryLevel == 0. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
    EWeaponType RequiredMasteryType = EWeaponType::None;

    /**
     * Clases que pueden usar esta receta. VacÃ­o = todas las clases pueden craftearla.
     * Ãštil para recetas exclusivas de Blacksmith, por ejemplo.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
    TArray<ESOLOClass> AllowedClasses;

    // â”€â”€â”€ CRAFTEO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Probabilidad de Ã©xito crÃ­tico [0.0, 1.0].
     * En crÃ­tico: +1 de cantidad o subida de rareza del item resultante.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Crafting",
        meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float CriticalSuccessChance = 0.05f;

    /**
     * Tiempo de crafteo en segundos. 0 = instantÃ¡neo.
     * Si es > 0, el componente inicia un timer y bloquea nuevos crafteos hasta que termine.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Crafting", meta = (ClampMin = "0.0"))
    float CraftingTime = 0.f;

    /**
     * Si true, el jugador conoce esta receta desde el inicio de la partida sin necesidad
     * de descubrirla. Usado en BeginPlay del CraftingComponent.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Discovery")
    bool bIsDiscoveredByDefault = false;

    // â”€â”€â”€ MÃ‰TODOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Registra el asset bajo el tipo primario "CraftingRecipe" para el AssetManager */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
