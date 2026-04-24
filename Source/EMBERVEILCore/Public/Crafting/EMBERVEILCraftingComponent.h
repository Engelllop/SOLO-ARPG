#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crafting/EMBERVEILCraftingTypes.h"
#include "EMBERVEILCraftingComponent.generated.h"

class UEMBERVEILRecipeDataAsset;
class UEMBERVEILInventoryComponent;
class UEMBERVEILSkillMasteryComponent;
class UAbilitySystemComponent;
enum class EEMBERVEILClass : uint8;

// ─── DELEGATES ────────────────────────────────────────────────────────────────

/** Broadcast cuando el jugador aprende una nueva receta */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeLearned,
    FName, RecipeID);

/** Broadcast cuando comienza un crafteo con timer */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCraftingStarted,
    FName, RecipeID,
    float, Duration);

/** Broadcast cuando un crafteo se completa (instantáneo o tras el timer) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingCompleted,
    FCraftingAttemptResult, Result);

/** Broadcast cuando el jugador cancela manualmente un crafteo en progreso */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingCancelled,
    FName, RecipeID);

// ─── COMPONENT ────────────────────────────────────────────────────────────────

/**
 * UEMBERVEILCraftingComponent
 *
 * Componente que vive en el PlayerState y gestiona:
 *   - El libro de recetas conocidas por el jugador.
 *   - La validación de requisitos para craftear (nivel, maestría, clase, ingredientes).
 *   - El proceso de crafteo instantáneo y con timer.
 *   - El cálculo de éxito crítico (subida de rareza o cantidad extra).
 *
 * Toda la lógica de modificación de estado (consumir ingredientes, añadir items)
 * debe ejecutarse en el servidor (HasAuthority). Los delegates notifican a la UI.
 */
UCLASS(ClassGroup = (EMBERVEIL), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class EMBERVEILCORE_API UEMBERVEILCraftingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEMBERVEILCraftingComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ─── DELEGATES ────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnRecipeLearned OnRecipeLearned;

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnCraftingStarted OnCraftingStarted;

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnCraftingCompleted OnCraftingCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Crafting|Delegates")
    FOnCraftingCancelled OnCraftingCancelled;

    // ─── ESTADO REPLICADO ─────────────────────────────────────────────────────

    /** IDs de recetas conocidas (TArray replicada; TSet replicado no está soportado por UHT). */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|State", Replicated)
    TArray<FName> KnownRecipeIDs;

    /** true mientras hay un crafteo con timer en progreso */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|State", Replicated)
    bool bIsCrafting = false;

    /** RecipeID del crafteo actualmente en progreso (solo válido si bIsCrafting == true) */
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|State", Replicated)
    FName CurrentCraftingRecipeID;

    // ─── API PÚBLICA ──────────────────────────────────────────────────────────

    /**
     * Añade la receta al libro del jugador.
     * @return false si la receta ya era conocida, true si se aprendió ahora.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool LearnRecipe(FName RecipeID);

    /** Devuelve true si el jugador ya conoce la receta con el ID dado */
    UFUNCTION(BlueprintPure, Category = "Crafting")
    bool KnowsRecipe(FName RecipeID) const;

    /**
     * Valida todos los requisitos de la receta sin consumir ningún recurso.
     * Puede llamarse desde la UI para mostrar por qué una receta no está disponible.
     *
     * @param Recipe  DataAsset de la receta a validar (no debe ser nullptr)
     * @return        ECraftingResult::Success si todo está en orden, o el primer fallo encontrado
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    ECraftingResult CanCraft(UEMBERVEILRecipeDataAsset* Recipe) const;

    /**
     * Intenta craftear el item de la receta indicada.
     *
     * Flujo:
     *   1. Busca la receta cargada en memoria (debe estar en el AssetManager).
     *   2. Llama a CanCraft para validar requisitos.
     *   3. Si CraftingTime > 0: inicia timer y retorna un resultado provisional.
     *      Si CraftingTime == 0: ejecuta FinishCrafting() de forma síncrona.
     *   4. FinishCrafting consume ingredientes, gasta currency, tira critical y añade el item.
     *
     * @param RecipeID  ID de la receta a craftear
     * @return          Resultado del intento. Si se inició un timer, Result == Success pero
     *                  CraftedQuantity == 0 hasta que el timer termine.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    FCraftingAttemptResult CraftItem(FName RecipeID);

    /**
     * Cancela el crafteo actualmente en progreso (si bIsCrafting == true).
     * Limpia el timer y devuelve los ingredientes al inventario si ya habían sido consumidos.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void CancelCrafting();

    /**
     * Devuelve todas las recetas conocidas que pertenecen a la categoría indicada.
     * Los DataAssets deben estar cargados en memoria (usa AssetManager síncrono).
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    TArray<UEMBERVEILRecipeDataAsset*> GetKnownRecipesByCategory(ECraftingCategory Category) const;

    /**
     * Devuelve el subconjunto de recetas conocidas para las que CanCraft devuelve Success.
     * Útil para la UI del panel de crafteo activo.
     */
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    TArray<UEMBERVEILRecipeDataAsset*> GetCraftableRecipes() const;

private:
    // ─── HELPERS ──────────────────────────────────────────────────────────────

    /** Obtiene el InventoryComponent del owner (PlayerState) */
    UEMBERVEILInventoryComponent* GetInventoryComponent() const;

    /** Obtiene el SkillMasteryComponent del owner (PlayerState) */
    UEMBERVEILSkillMasteryComponent* GetMasteryComponent() const;

    /** Obtiene el AbilitySystemComponent del owner (PlayerState) */
    UAbilitySystemComponent* GetASC() const;

    /** Lee el nivel del jugador desde el AttributeSet a través del ASC */
    int32 GetPlayerLevel() const;

    /** Lee la clase del jugador desde el GameInstance */
    EEMBERVEILClass GetPlayerClass() const;

    /**
     * Carga síncronamente todas las recetas conocidas desde el AssetManager.
     * Solo devuelve los assets que estén actualmente en memoria.
     */
    TArray<UEMBERVEILRecipeDataAsset*> GetAllKnownRecipesLoaded() const;

    /**
     * Ejecuta el crafteo real: consume ingredientes, gasta currency, calcula crítico
     * y añade el resultado al inventario.
     * Llamada síncronamente (crafteo instantáneo) o por el FTimerHandle (crafteo con timer).
     */
    void FinishCrafting();

    // ─── ESTADO INTERNO ───────────────────────────────────────────────────────

    /** Timer activo durante crafteos con duración > 0 */
    FTimerHandle CraftingTimerHandle;

    /**
     * Ingredientes que ya fueron consumidos del inventario al iniciar un crafteo con timer.
     * Se guardan aquí para poder devolverlos si el jugador cancela.
     * Se limpia tras FinishCrafting o tras devolver los ingredientes en CancelCrafting.
     */
    TArray<FCraftingIngredient> PendingConsumedIngredients;

    /** Currency consumida al iniciar el timer (para devolución en cancelación) */
    int32 PendingConsumedCurrency = 0;

    /** Puntero cacheado a la receta en progreso (solo válido durante bIsCrafting == true) */
    UPROPERTY()
    TObjectPtr<UEMBERVEILRecipeDataAsset> ActiveRecipe;

    /**
     * Almacena el FCraftingAttemptResult del último crafteo instantáneo,
     * escrito por FinishCrafting justo antes del broadcast.
     * Permite que CraftItem lo devuelva directamente al caller síncrono.
     * No tiene sentido en crafteos con timer (el resultado llega por delegate).
     */
    FCraftingAttemptResult LastInstantResult;
};
