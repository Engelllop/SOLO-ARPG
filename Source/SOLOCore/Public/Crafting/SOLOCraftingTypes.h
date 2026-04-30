#pragma once

#include "CoreMinimal.h"
#include "Skills/SOLOSwordSkillTypes.h"   // EWeaponType
#include "SOLOCraftingTypes.generated.h"

class USOLOItemBase;

// â”€â”€â”€ ENUMS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/** CategorÃ­a de oficio a la que pertenece una receta */
UENUM(BlueprintType)
enum class ECraftingCategory : uint8
{
    Weaponsmithing  UMETA(DisplayName = "Forja de Armas"),
    Armorsmithing   UMETA(DisplayName = "Forja de Armaduras"),
    Alchemy         UMETA(DisplayName = "Alquimia"),
    Enchanting      UMETA(DisplayName = "Encantamiento"),
    Cooking         UMETA(DisplayName = "Cocina"),
    General         UMETA(DisplayName = "General")
};

/** Resultado devuelto tras un intento de crafteo o una validaciÃ³n previa */
UENUM(BlueprintType)
enum class ECraftingResult : uint8
{
    Success                     UMETA(DisplayName = "Ã‰xito"),
    Failed_MissingIngredients   UMETA(DisplayName = "Faltan ingredientes"),
    Failed_LevelTooLow          UMETA(DisplayName = "Nivel insuficiente"),
    Failed_MasteryTooLow        UMETA(DisplayName = "MaestrÃ­a insuficiente"),
    Failed_WrongClass           UMETA(DisplayName = "Clase incorrecta"),
    Failed_InventoryFull        UMETA(DisplayName = "Inventario lleno"),
    Failed_InsufficientCurrency UMETA(DisplayName = "Monedas insuficientes"),
    Failed_RecipeUnknown        UMETA(DisplayName = "Receta desconocida")
};

// â”€â”€â”€ STRUCTS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * Define un ingrediente necesario para una receta de crafteo.
 * ItemID es la clave de bÃºsqueda rÃ¡pida en el inventario.
 * Item es la referencia soft para que la UI pueda mostrar el icono sin cargar el asset.
 */
USTRUCT(BlueprintType)
struct SOLOCORE_API FCraftingIngredient
{
    GENERATED_BODY()

    /** ID del item requerido â€” debe coincidir con USOLOItemBase::ItemID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    FName ItemID;

    /** Referencia soft al DataAsset para la UI (tooltip, icono) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    TSoftObjectPtr<USOLOItemBase> Item;

    /** Cantidad necesaria de este ingrediente (mÃ­nimo 1) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting", meta = (ClampMin = "1"))
    int32 Quantity = 1;
};

/**
 * Encapsula el resultado completo de un intento de crafteo.
 * Se devuelve desde CraftItem y tambiÃ©n se pasa al delegate OnCraftingCompleted.
 */
USTRUCT(BlueprintType)
struct SOLOCORE_API FCraftingAttemptResult
{
    GENERATED_BODY()

    /** CÃ³digo de resultado â€” Success o el motivo del fallo */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting")
    ECraftingResult Result = ECraftingResult::Success;

    /** Referencia soft al item producido (vÃ¡lida solo si Result == Success) */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting")
    TSoftObjectPtr<USOLOItemBase> CraftedItem;

    /** Cantidad final aÃ±adida al inventario (incluye el bonus de critical si aplica) */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting")
    int32 CraftedQuantity = 0;

    /**
     * true si hubo Ã©xito crÃ­tico.
     * En ese caso CraftedQuantity puede ser +1 sobre el base,
     * o el item puede haber subido de rareza.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting")
    bool bWasCriticalSuccess = false;
};
