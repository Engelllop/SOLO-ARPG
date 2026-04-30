#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crafting/SOLOCraftingTypes.h"
#include "SOLOCraftingComponent.generated.h"

class USOLORecipeDataAsset;
class USOLOInventoryComponent;
class USOLOSkillMasteryComponent;
class UAbilitySystemComponent;
enum class ESOLOClass : uint8;

// â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/** Broadcast cuando el jugador aprende una nueva receta */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeLearned,
    FName, RecipeID);

/** Broadcast cuando comienza un crafteo con timer */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCraftingStarted,
    FName, RecipeID,
    float, Duration);

/** Broadcast cuando un crafteo se completa (instantÃ¡neo o tras el timer) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingCompleted,
    FCraftingAttemptResult, Result);

/** Broadcast cuando el jugador cancela manualmente un crafteo en progreso */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingCancelled,
    FName, RecipeID);

// â”€â”€â”€ COMPONENT â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * USOLOCraftingComponent
 *
 * Componente que vive en el PlayerState y gestiona:
 *   - El libro de recetas conocidas por el jugador.
 *   - La validaciÃ³n de requisitos para craftear (nivel, maestrÃ­a, clase, ingredientes).
 *   - El proceso de crafteo instantÃ¡neo y con timer.
 *   - El cÃ¡lculo de Ã©xito crÃ­tico (subida de rareza o cantidad extra).
 *
 * Toda la lÃ³gica de modificaciÃ³n de estado (consumir ingredientes, aÃ±adir items)
 * debe ejecutarse en el servidor (HasAuthority). Los delegates notifican a la UI.
 */
UCLASS(ClassGroup = (SOLO), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOCraftingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USOLOCraftingComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnRecipeLearned OnRecipeLearned;

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnCraftingStarted OnCraftingStarted;

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnCraftingCompleted OnCraftingCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnCraftingCancelled OnCraftingCancelled;

    // â”€â”€â”€ ESTADO REPLICADO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** IDs de recetas conocidas (TArray replicada; TSet replicado no estÃ¡ soportado por UHT). */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|State", Replicated)
    TArray<FName> KnownRecipeIDs;

    /** true mientras hay un crafteo con timer en progreso */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|State", Replicated)
    bool bIsCrafting = false;

    /** RecipeID del crafteo actualmente en progreso (solo vÃ¡lido si bIsCrafting == true) */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|State", Replicated)
    FName CurrentCraftingRecipeID;

    // â”€â”€â”€ API PÃšBLICA â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * AÃ±ade la receta al libro del jugador.
     * @return false si la receta ya era conocida, true si se aprendiÃ³ ahora.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool LearnRecipe(FName RecipeID);

    /** Devuelve true si el jugador ya conoce la receta con el ID dado */
    UFUNCTION(BlueprintPure, Category = "Crafting")
    bool KnowsRecipe(FName RecipeID) const;

    /**
     * Valida todos los requisitos de la receta sin consumir ningÃºn recurso.
     * Puede llamarse desde la UI para mostrar por quÃ© una receta no estÃ¡ disponible.
     *
     * @param Recipe  DataAsset de la receta a validar (no debe ser nullptr)
     * @return        ECraftingResult::Success si todo estÃ¡ en orden, o el primer fallo encontrado
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    ECraftingResult CanCraft(USOLORecipeDataAsset* Recipe) const;

    /**
     * Intenta craftear el item de la receta indicada.
     *
     * Flujo:
     *   1. Busca la receta cargada en memoria (debe estar en el AssetManager).
     *   2. Llama a CanCraft para validar requisitos.
     *   3. Si CraftingTime > 0: inicia timer y retorna un resultado provisional.
     *      Si CraftingTime == 0: ejecuta FinishCrafting() de forma sÃ­ncrona.
     *   4. FinishCrafting consume ingredientes, gasta currency, tira critical y aÃ±ade el item.
     *
     * @param RecipeID  ID de la receta a craftear
     * @return          Resultado del intento. Si se iniciÃ³ un timer, Result == Success pero
     *                  CraftedQuantity == 0 hasta que el timer termine.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    FCraftingAttemptResult CraftItem(FName RecipeID);

    /**
     * Cancela el crafteo actualmente en progreso (si bIsCrafting == true).
     * Limpia el timer y devuelve los ingredientes al inventario si ya habÃ­an sido consumidos.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void CancelCrafting();

    /**
     * Devuelve todas las recetas conocidas que pertenecen a la categorÃ­a indicada.
     * Los DataAssets deben estar cargados en memoria (usa AssetManager sÃ­ncrono).
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    TArray<USOLORecipeDataAsset*> GetKnownRecipesByCategory(ECraftingCategory Category) const;

    /**
     * Devuelve el subconjunto de recetas conocidas para las que CanCraft devuelve Success.
     * Ãštil para la UI del panel de crafteo activo.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    TArray<USOLORecipeDataAsset*> GetCraftableRecipes() const;

private:
    // â”€â”€â”€ HELPERS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Obtiene el InventoryComponent del owner (PlayerState) */
    USOLOInventoryComponent* GetInventoryComponent() const;

    /** Obtiene el SkillMasteryComponent del owner (PlayerState) */
    USOLOSkillMasteryComponent* GetMasteryComponent() const;

    /** Obtiene el AbilitySystemComponent del owner (PlayerState) */
    UAbilitySystemComponent* GetASC() const;

    /** Lee el nivel del jugador desde el AttributeSet a travÃ©s del ASC */
    int32 GetPlayerLevel() const;

    /** Lee la clase del jugador desde el GameInstance */
    ESOLOClass GetPlayerClass() const;

    /**
     * Carga sÃ­ncronamente todas las recetas conocidas desde el AssetManager.
     * Solo devuelve los assets que estÃ©n actualmente en memoria.
     */
    TArray<USOLORecipeDataAsset*> GetAllKnownRecipesLoaded() const;

    /**
     * Ejecuta el crafteo real: consume ingredientes, gasta currency, calcula crÃ­tico
     * y aÃ±ade el resultado al inventario.
     * Llamada sÃ­ncronamente (crafteo instantÃ¡neo) o por el FTimerHandle (crafteo con timer).
     */
    void FinishCrafting();

    // â”€â”€â”€ ESTADO INTERNO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Timer activo durante crafteos con duraciÃ³n > 0 */
    FTimerHandle CraftingTimerHandle;

    /**
     * Ingredientes que ya fueron consumidos del inventario al iniciar un crafteo con timer.
     * Se guardan aquÃ­ para poder devolverlos si el jugador cancela.
     * Se limpia tras FinishCrafting o tras devolver los ingredientes en CancelCrafting.
     */
    TArray<FCraftingIngredient> PendingConsumedIngredients;

    /** Currency consumida al iniciar el timer (para devoluciÃ³n en cancelaciÃ³n) */
    int32 PendingConsumedCurrency = 0;

    /** Puntero cacheado a la receta en progreso (solo vÃ¡lido durante bIsCrafting == true) */
    UPROPERTY()
    TObjectPtr<USOLORecipeDataAsset> ActiveRecipe;

    /**
     * Almacena el FCraftingAttemptResult del Ãºltimo crafteo instantÃ¡neo,
     * escrito por FinishCrafting justo antes del broadcast.
     * Permite que CraftItem lo devuelva directamente al caller sÃ­ncrono.
     * No tiene sentido en crafteos con timer (el resultado llega por delegate).
     */
    FCraftingAttemptResult LastInstantResult;
};
