#include "Crafting/EMBERVEILCraftingComponent.h"

#include "Crafting/EMBERVEILRecipeDataAsset.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Inventory/EMBERVEILItemBase.h"
#include "Inventory/EMBERVEILItemInstance.h"
#include "Inventory/EMBERVEILItemTypes.h"
#include "Skills/EMBERVEILSkillMasteryComponent.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "Player/EMBERVEILPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────

UEMBERVEILCraftingComponent::UEMBERVEILCraftingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// ─── LIFECYCLE ────────────────────────────────────────────────────────────────

void UEMBERVEILCraftingComponent::BeginPlay()
{
    Super::BeginPlay();

    // Solo el servidor registra las recetas por defecto para evitar duplicados
    if (!GetOwner()->HasAuthority())
    {
        return;
    }

    // Cargamos síncronamente todos los CraftingRecipe DataAssets que estén en memoria
    // y que tengan bIsDiscoveredByDefault == true, añadiéndolos al libro del jugador.
    UAssetManager& AssetManager = UAssetManager::Get();
    TArray<FPrimaryAssetId> AssetIds;
    AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("CraftingRecipe"), AssetIds);

    for (const FPrimaryAssetId& AssetId : AssetIds)
    {
        UEMBERVEILRecipeDataAsset* Recipe = Cast<UEMBERVEILRecipeDataAsset>(
            AssetManager.GetPrimaryAssetObject(AssetId));

        if (Recipe && Recipe->bIsDiscoveredByDefault && !Recipe->RecipeID.IsNone())
        {
            KnownRecipeIDs.AddUnique(Recipe->RecipeID);
        }
    }
}

// ─── REPLICATION ──────────────────────────────────────────────────────────────

void UEMBERVEILCraftingComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UEMBERVEILCraftingComponent, KnownRecipeIDs);
    DOREPLIFETIME(UEMBERVEILCraftingComponent, bIsCrafting);
    DOREPLIFETIME(UEMBERVEILCraftingComponent, CurrentCraftingRecipeID);
}

// ─── BOOK OF RECIPES ──────────────────────────────────────────────────────────

bool UEMBERVEILCraftingComponent::LearnRecipe(FName RecipeID)
{
    if (RecipeID.IsNone())
    {
        return false;
    }

    if (KnownRecipeIDs.Contains(RecipeID))
    {
        return false;
    }

    KnownRecipeIDs.AddUnique(RecipeID);
    OnRecipeLearned.Broadcast(RecipeID);
    return true;
}

bool UEMBERVEILCraftingComponent::KnowsRecipe(FName RecipeID) const
{
    return KnownRecipeIDs.Contains(RecipeID);
}

// ─── VALIDATION ───────────────────────────────────────────────────────────────

ECraftingResult UEMBERVEILCraftingComponent::CanCraft(UEMBERVEILRecipeDataAsset* Recipe) const
{
    if (!Recipe)
    {
        return ECraftingResult::Failed_RecipeUnknown;
    }

    // 1. El jugador debe conocer la receta
    if (!KnowsRecipe(Recipe->RecipeID))
    {
        return ECraftingResult::Failed_RecipeUnknown;
    }

    // 2. Nivel del jugador
    const int32 PlayerLevel = GetPlayerLevel();
    if (PlayerLevel < Recipe->RequiredPlayerLevel)
    {
        return ECraftingResult::Failed_LevelTooLow;
    }

    // 3. Maestría (solo si la receta requiere un tipo y nivel específicos)
    if (Recipe->RequiredMasteryLevel > 0 && Recipe->RequiredMasteryType != EWeaponType::None)
    {
        UEMBERVEILSkillMasteryComponent* MasteryComp = GetMasteryComponent();
        if (!MasteryComp)
        {
            return ECraftingResult::Failed_MasteryTooLow;
        }

        const int32 CurrentMastery = MasteryComp->GetMasteryLevel(Recipe->RequiredMasteryType);
        if (CurrentMastery < Recipe->RequiredMasteryLevel)
        {
            return ECraftingResult::Failed_MasteryTooLow;
        }
    }

    // 4. Clase del jugador (solo si la receta restringe las clases)
    if (Recipe->AllowedClasses.Num() > 0)
    {
        const EEMBERVEILClass PlayerClass = GetPlayerClass();
        if (!Recipe->AllowedClasses.Contains(PlayerClass))
        {
            return ECraftingResult::Failed_WrongClass;
        }
    }

    // 5. Ingredientes
    UEMBERVEILInventoryComponent* InvComp = GetInventoryComponent();
    if (!InvComp)
    {
        return ECraftingResult::Failed_MissingIngredients;
    }

    for (const FCraftingIngredient& Ingredient : Recipe->Ingredients)
    {
        if (!InvComp->HasItem(Ingredient.ItemID, Ingredient.Quantity))
        {
            return ECraftingResult::Failed_MissingIngredients;
        }
    }

    // 6. Currency
    if (Recipe->CraftingCurrencyCost > 0)
    {
        if (InvComp->Currency < Recipe->CraftingCurrencyCost)
        {
            return ECraftingResult::Failed_InsufficientCurrency;
        }
    }

    // 7. Espacio en inventario para el resultado
    //    Comprobamos si hay un slot con espacio o un slot vacío disponible.
    //    Cargamos el DataAsset del resultado para obtener MaxStackSize.
    //    Si no está cargado aún, asumimos que se necesita un slot libre.
    {
        UEMBERVEILItemBase* ResultAsset = Recipe->ResultItem.Get();
        bool bHasSpace = false;

        if (ResultAsset)
        {
            // Si el item tiene MaxStackSize > 1, puede apilarse en un slot existente
            if (ResultAsset->MaxStackSize > 1)
            {
                const int32 CurrentQty = InvComp->GetItemQuantity(ResultAsset->ItemID);
                // Calculamos cuántos items caben en los stacks actuales
                // (cantidad total en slots existentes vs. máximo si todos estuvieran llenos)
                const int32 SlotsUsedCount = [&]() -> int32
                {
                    int32 Count = 0;
                    for (const FInventorySlot& Slot : InvComp->InventorySlots)
                    {
                        if (!Slot.IsEmpty() &&
                            Slot.ItemInstance &&
                            Slot.ItemInstance->GetClass())  // instancia válida
                        {
                            Count++;
                        }
                    }
                    return Count;
                }();

                // Si la cantidad actual en todos los stacks no llena un stack completo,
                // todavía hay espacio. O si hay slots vacíos, también podemos añadir.
                const int32 SpaceInExistingStacks =
                    (CurrentQty % ResultAsset->MaxStackSize == 0 && CurrentQty > 0)
                    ? 0
                    : (ResultAsset->MaxStackSize - (CurrentQty % ResultAsset->MaxStackSize));

                if (SpaceInExistingStacks >= Recipe->ResultQuantity)
                {
                    bHasSpace = true;
                }
                else
                {
                    // Verificar si hay un slot vacío
                    const int32 OccupiedSlots = SlotsUsedCount;
                    bHasSpace = OccupiedSlots < InvComp->MaxInventorySlots;
                }
            }
            else
            {
                // MaxStackSize == 1: necesita slot vacío por cada unidad
                int32 EmptySlots = 0;
                for (const FInventorySlot& Slot : InvComp->InventorySlots)
                {
                    if (Slot.IsEmpty())
                    {
                        EmptySlots++;
                    }
                }
                // También contar slots que aún no existen en el array (por debajo de MaxInventorySlots)
                const int32 UnallocatedSlots =
                    InvComp->MaxInventorySlots - InvComp->InventorySlots.Num();
                EmptySlots += UnallocatedSlots;

                bHasSpace = EmptySlots >= Recipe->ResultQuantity;
            }
        }
        else
        {
            // Asset no cargado — verificar si hay al menos un slot libre
            int32 EmptySlots = 0;
            for (const FInventorySlot& Slot : InvComp->InventorySlots)
            {
                if (Slot.IsEmpty())
                {
                    EmptySlots++;
                }
            }
            const int32 UnallocatedSlots =
                InvComp->MaxInventorySlots - InvComp->InventorySlots.Num();
            EmptySlots += UnallocatedSlots;

            bHasSpace = EmptySlots > 0;
        }

        if (!bHasSpace)
        {
            return ECraftingResult::Failed_InventoryFull;
        }
    }

    return ECraftingResult::Success;
}

// ─── CRAFTING ENTRY POINT ─────────────────────────────────────────────────────

FCraftingAttemptResult UEMBERVEILCraftingComponent::CraftItem(FName RecipeID)
{
    FCraftingAttemptResult FailResult;

    // Evitar crafteos concurrentes
    if (bIsCrafting)
    {
        FailResult.Result = ECraftingResult::Failed_RecipeUnknown; // ya ocupado
        return FailResult;
    }

    // Buscar el DataAsset en el AssetManager
    UAssetManager& AssetManager = UAssetManager::Get();
    UEMBERVEILRecipeDataAsset* Recipe = Cast<UEMBERVEILRecipeDataAsset>(
        AssetManager.GetPrimaryAssetObject(
            FPrimaryAssetId(FPrimaryAssetType("CraftingRecipe"), RecipeID)));

    if (!Recipe)
    {
        FailResult.Result = ECraftingResult::Failed_RecipeUnknown;
        return FailResult;
    }

    // Validar requisitos
    const ECraftingResult ValidationResult = CanCraft(Recipe);
    if (ValidationResult != ECraftingResult::Success)
    {
        FailResult.Result = ValidationResult;
        return FailResult;
    }

    UEMBERVEILInventoryComponent* InvComp = GetInventoryComponent();
    if (!InvComp)
    {
        FailResult.Result = ECraftingResult::Failed_MissingIngredients;
        return FailResult;
    }

    if (Recipe->CraftingTime > 0.f)
    {
        // ── Crafteo con timer ─────────────────────────────────────────────────
        // Consumir los ingredientes AHORA para que no puedan gastarse en otra cosa
        // mientras el timer transcurre.
        PendingConsumedIngredients.Empty();
        PendingConsumedCurrency = 0;

        for (const FCraftingIngredient& Ingredient : Recipe->Ingredients)
        {
            if (InvComp->RemoveItem(Ingredient.ItemID, Ingredient.Quantity))
            {
                PendingConsumedIngredients.Add(Ingredient);
            }
        }

        if (Recipe->CraftingCurrencyCost > 0)
        {
            if (InvComp->SpendCurrency(Recipe->CraftingCurrencyCost))
            {
                PendingConsumedCurrency = Recipe->CraftingCurrencyCost;
            }
        }

        // Guardar estado y arrancar timer
        ActiveRecipe = Recipe;
        bIsCrafting = true;
        CurrentCraftingRecipeID = RecipeID;

        UWorld* World = GetWorld();
        if (World)
        {
            World->GetTimerManager().SetTimer(
                CraftingTimerHandle,
                this,
                &UEMBERVEILCraftingComponent::FinishCrafting,
                Recipe->CraftingTime,
                false);
        }

        OnCraftingStarted.Broadcast(RecipeID, Recipe->CraftingTime);

        // Devolvemos Success con Quantity == 0 para indicar que está en progreso
        FCraftingAttemptResult PendingResult;
        PendingResult.Result = ECraftingResult::Success;
        PendingResult.CraftedItem = Recipe->ResultItem;
        PendingResult.CraftedQuantity = 0;
        return PendingResult;
    }
    else
    {
        // ── Crafteo instantáneo ───────────────────────────────────────────────
        ActiveRecipe = Recipe;
        bIsCrafting = true;
        CurrentCraftingRecipeID = RecipeID;
        PendingConsumedIngredients.Empty();
        PendingConsumedCurrency = 0;
        LastInstantResult = FCraftingAttemptResult(); // Reset before FinishCrafting fills it

        FinishCrafting();

        // FinishCrafting wrote into LastInstantResult before broadcasting — return it directly.
        return LastInstantResult;
    }
}

// ─── FINISH CRAFTING ──────────────────────────────────────────────────────────

void UEMBERVEILCraftingComponent::FinishCrafting()
{
    if (!ActiveRecipe)
    {
        bIsCrafting = false;
        CurrentCraftingRecipeID = NAME_None;
        return;
    }

    UEMBERVEILInventoryComponent* InvComp = GetInventoryComponent();

    FCraftingAttemptResult Result;
    Result.CraftedItem = ActiveRecipe->ResultItem;

    // Si los ingredientes aún no fueron consumidos (crafteo instantáneo), consumirlos ahora
    if (PendingConsumedIngredients.Num() == 0 && InvComp)
    {
        for (const FCraftingIngredient& Ingredient : ActiveRecipe->Ingredients)
        {
            InvComp->RemoveItem(Ingredient.ItemID, Ingredient.Quantity);
        }

        if (ActiveRecipe->CraftingCurrencyCost > 0)
        {
            InvComp->SpendCurrency(ActiveRecipe->CraftingCurrencyCost);
        }
    }
    // Si vinieron de un crafteo con timer, ya estaban consumidos (PendingConsumedIngredients
    // fue rellenado en CraftItem). No hay que consumirlos de nuevo.

    // ── Critical success roll ─────────────────────────────────────────────────
    const bool bCritical = FMath::FRand() < ActiveRecipe->CriticalSuccessChance;
    Result.bWasCriticalSuccess = bCritical;

    int32 FinalQuantity = ActiveRecipe->ResultQuantity;

    if (bCritical)
    {
        // Intentar subir la rareza del item resultado
        UEMBERVEILItemBase* ResultAsset = ActiveRecipe->ResultItem.Get();
        if (ResultAsset)
        {
            const EEMBERVEILItemRarity CurrentRarity = ResultAsset->Rarity;
            // Solo subir rareza si no es ya Legendary o Divine
            if (CurrentRarity != EEMBERVEILItemRarity::Legendary &&
                CurrentRarity != EEMBERVEILItemRarity::Divine)
            {
                // La rareza sube un nivel. Como el sistema de items usa un DataAsset inmutable,
                // en la práctica el critical bonus se traduce en +1 de cantidad extra.
                // Si el pipeline del proyecto tiene items mutables por rareza, esto se puede
                // extender aquí. Por ahora siempre añade +1 unidad como bonus crítico.
                FinalQuantity += 1;
            }
            else
            {
                // Para Legendary/Divine, el critical da +1 de cantidad igualmente
                FinalQuantity += 1;
            }
        }
        else
        {
            FinalQuantity += 1;
        }
    }

    Result.CraftedQuantity = FinalQuantity;

    // ── Añadir resultado al inventario ────────────────────────────────────────
    if (InvComp)
    {
        UEMBERVEILItemBase* ResultAsset = ActiveRecipe->ResultItem.Get();
        if (ResultAsset)
        {
            const bool bAdded = InvComp->AddItem(ResultAsset, FinalQuantity);
            if (!bAdded)
            {
                // El inventario se llenó entre la validación y ahora (edge case en multijugador).
                // Reembolsamos los ingredientes consumidos.
                // PendingConsumedIngredients contiene los ingredientes si fue un crafteo con timer;
                // para crafteos instantáneos usamos directamente la lista de la receta.
                const TArray<FCraftingIngredient>& IngredientsToRefund =
                    PendingConsumedIngredients.Num() > 0
                    ? PendingConsumedIngredients
                    : ActiveRecipe->Ingredients;

                for (const FCraftingIngredient& Ingredient : IngredientsToRefund)
                {
                    UEMBERVEILItemBase* IngredientAsset = Ingredient.Item.Get();
                    if (IngredientAsset)
                    {
                        InvComp->AddItem(IngredientAsset, Ingredient.Quantity);
                    }
                }
                if (PendingConsumedCurrency > 0)
                {
                    InvComp->AddCurrency(PendingConsumedCurrency);
                }
                else if (ActiveRecipe->CraftingCurrencyCost > 0
                         && PendingConsumedIngredients.Num() == 0)
                {
                    // Crafteo instantáneo: el currency ya fue gastado arriba, devolverlo
                    InvComp->AddCurrency(ActiveRecipe->CraftingCurrencyCost);
                }

                Result.Result = ECraftingResult::Failed_InventoryFull;
                Result.CraftedQuantity = 0;
            }
            else
            {
                Result.Result = ECraftingResult::Success;
            }
        }
        else
        {
            // Asset no estaba cargado en memoria — fallo suave
            Result.Result = ECraftingResult::Failed_RecipeUnknown;
            Result.CraftedQuantity = 0;
        }
    }
    else
    {
        Result.Result = ECraftingResult::Failed_InventoryFull;
        Result.CraftedQuantity = 0;
    }

    // ── Limpiar estado ────────────────────────────────────────────────────────
    PendingConsumedIngredients.Empty();
    PendingConsumedCurrency = 0;

    const FName FinishedRecipeID = CurrentCraftingRecipeID;

    bIsCrafting = false;
    CurrentCraftingRecipeID = NAME_None;
    ActiveRecipe = nullptr;

    // ── Cachear para el retorno síncrono (crafteo instantáneo) ───────────────
    LastInstantResult = Result;

    // ── Notificar ─────────────────────────────────────────────────────────────
    OnCraftingCompleted.Broadcast(Result);
}

// ─── CANCEL ───────────────────────────────────────────────────────────────────

void UEMBERVEILCraftingComponent::CancelCrafting()
{
    if (!bIsCrafting)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (World)
    {
        World->GetTimerManager().ClearTimer(CraftingTimerHandle);
    }

    // Devolver ingredientes consumidos al inventario
    UEMBERVEILInventoryComponent* InvComp = GetInventoryComponent();
    if (InvComp)
    {
        for (const FCraftingIngredient& Ingredient : PendingConsumedIngredients)
        {
            UEMBERVEILItemBase* ItemAsset = Ingredient.Item.Get();
            if (ItemAsset)
            {
                InvComp->AddItem(ItemAsset, Ingredient.Quantity);
            }
        }

        if (PendingConsumedCurrency > 0)
        {
            InvComp->AddCurrency(PendingConsumedCurrency);
        }
    }

    PendingConsumedIngredients.Empty();
    PendingConsumedCurrency = 0;

    const FName CancelledID = CurrentCraftingRecipeID;

    bIsCrafting = false;
    CurrentCraftingRecipeID = NAME_None;
    ActiveRecipe = nullptr;

    OnCraftingCancelled.Broadcast(CancelledID);
}

// ─── QUERIES ──────────────────────────────────────────────────────────────────

TArray<UEMBERVEILRecipeDataAsset*> UEMBERVEILCraftingComponent::GetKnownRecipesByCategory(
    ECraftingCategory Category) const
{
    TArray<UEMBERVEILRecipeDataAsset*> Result;
    TArray<UEMBERVEILRecipeDataAsset*> AllKnown = GetAllKnownRecipesLoaded();

    for (UEMBERVEILRecipeDataAsset* Recipe : AllKnown)
    {
        if (Recipe && Recipe->Category == Category)
        {
            Result.Add(Recipe);
        }
    }

    return Result;
}

TArray<UEMBERVEILRecipeDataAsset*> UEMBERVEILCraftingComponent::GetCraftableRecipes() const
{
    TArray<UEMBERVEILRecipeDataAsset*> Result;
    TArray<UEMBERVEILRecipeDataAsset*> AllKnown = GetAllKnownRecipesLoaded();

    for (UEMBERVEILRecipeDataAsset* Recipe : AllKnown)
    {
        if (Recipe && CanCraft(Recipe) == ECraftingResult::Success)
        {
            Result.Add(Recipe);
        }
    }

    return Result;
}

// ─── PRIVATE HELPERS ──────────────────────────────────────────────────────────

UEMBERVEILInventoryComponent* UEMBERVEILCraftingComponent::GetInventoryComponent() const
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    return Owner->FindComponentByClass<UEMBERVEILInventoryComponent>();
}

UEMBERVEILSkillMasteryComponent* UEMBERVEILCraftingComponent::GetMasteryComponent() const
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    return Owner->FindComponentByClass<UEMBERVEILSkillMasteryComponent>();
}

UAbilitySystemComponent* UEMBERVEILCraftingComponent::GetASC() const
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    // PlayerState implementa IAbilitySystemInterface
    IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Owner);
    if (ASCInterface)
    {
        return ASCInterface->GetAbilitySystemComponent();
    }

    return nullptr;
}

int32 UEMBERVEILCraftingComponent::GetPlayerLevel() const
{
    UAbilitySystemComponent* ASC = GetASC();
    if (!ASC)
    {
        return 1;
    }

    // Leer el atributo Level directamente del AttributeSet
    AEMBERVEILPlayerState* PS = Cast<AEMBERVEILPlayerState>(GetOwner());
    if (PS)
    {
        return PS->GetPlayerLevel();
    }

    // Fallback: leer directamente del ASC con el getter generado por la macro ATTRIBUTE_ACCESSORS
    const UEMBERVEILAttributeSet* AttrSet =
        ASC->GetSet<UEMBERVEILAttributeSet>();
    if (AttrSet)
    {
        return FMath::FloorToInt(AttrSet->GetLevel());
    }

    return 1;
}

EEMBERVEILClass UEMBERVEILCraftingComponent::GetPlayerClass() const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return EEMBERVEILClass::Warrior;
    }

    UEMBERVEILGameInstance* GI = World->GetGameInstance<UEMBERVEILGameInstance>();
    if (!GI)
    {
        return EEMBERVEILClass::Warrior;
    }

    return GI->GetActivePlayerData().Class;
}

TArray<UEMBERVEILRecipeDataAsset*> UEMBERVEILCraftingComponent::GetAllKnownRecipesLoaded() const
{
    TArray<UEMBERVEILRecipeDataAsset*> Result;

    if (KnownRecipeIDs.Num() == 0)
    {
        return Result;
    }

    UAssetManager& AssetManager = UAssetManager::Get();

    for (const FName& RecipeID : KnownRecipeIDs)
    {
        const FPrimaryAssetId AssetId(FPrimaryAssetType("CraftingRecipe"), RecipeID);
        UEMBERVEILRecipeDataAsset* Recipe = Cast<UEMBERVEILRecipeDataAsset>(
            AssetManager.GetPrimaryAssetObject(AssetId));

        if (Recipe)
        {
            Result.Add(Recipe);
        }
    }

    return Result;
}
