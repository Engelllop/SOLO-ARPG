#include "Crafting/SOLOCraftingComponent.h"

#include "Crafting/SOLORecipeDataAsset.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Inventory/SOLOItemBase.h"
#include "Inventory/SOLOItemInstance.h"
#include "Inventory/SOLOItemTypes.h"
#include "Skills/SOLOSkillMasteryComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Player/SOLOPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// â”€â”€â”€ CONSTRUCTOR â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

USOLOCraftingComponent::USOLOCraftingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// â”€â”€â”€ LIFECYCLE â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOCraftingComponent::BeginPlay()
{
    Super::BeginPlay();

    // Solo el servidor registra las recetas por defecto para evitar duplicados
    if (!GetOwner()->HasAuthority())
    {
        return;
    }

    // Cargamos sÃ­ncronamente todos los CraftingRecipe DataAssets que estÃ©n en memoria
    // y que tengan bIsDiscoveredByDefault == true, aÃ±adiÃ©ndolos al libro del jugador.
    UAssetManager& AssetManager = UAssetManager::Get();
    TArray<FPrimaryAssetId> AssetIds;
    AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("CraftingRecipe"), AssetIds);

    for (const FPrimaryAssetId& AssetId : AssetIds)
    {
        USOLORecipeDataAsset* Recipe = Cast<USOLORecipeDataAsset>(
            AssetManager.GetPrimaryAssetObject(AssetId));

        if (Recipe && Recipe->bIsDiscoveredByDefault && !Recipe->RecipeID.IsNone())
        {
            KnownRecipeIDs.AddUnique(Recipe->RecipeID);
        }
    }
}

// â”€â”€â”€ REPLICATION â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOCraftingComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USOLOCraftingComponent, KnownRecipeIDs);
    DOREPLIFETIME(USOLOCraftingComponent, bIsCrafting);
    DOREPLIFETIME(USOLOCraftingComponent, CurrentCraftingRecipeID);
}

// â”€â”€â”€ BOOK OF RECIPES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

bool USOLOCraftingComponent::LearnRecipe(FName RecipeID)
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

bool USOLOCraftingComponent::KnowsRecipe(FName RecipeID) const
{
    return KnownRecipeIDs.Contains(RecipeID);
}

// â”€â”€â”€ VALIDATION â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

ECraftingResult USOLOCraftingComponent::CanCraft(USOLORecipeDataAsset* Recipe) const
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

    // 3. MaestrÃ­a (solo si la receta requiere un tipo y nivel especÃ­ficos)
    if (Recipe->RequiredMasteryLevel > 0 && Recipe->RequiredMasteryType != EWeaponType::None)
    {
        USOLOSkillMasteryComponent* MasteryComp = GetMasteryComponent();
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
        const ESOLOClass PlayerClass = GetPlayerClass();
        if (!Recipe->AllowedClasses.Contains(PlayerClass))
        {
            return ECraftingResult::Failed_WrongClass;
        }
    }

    // 5. Ingredientes
    USOLOInventoryComponent* InvComp = GetInventoryComponent();
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
    //    Comprobamos si hay un slot con espacio o un slot vacÃ­o disponible.
    //    Cargamos el DataAsset del resultado para obtener MaxStackSize.
    //    Si no estÃ¡ cargado aÃºn, asumimos que se necesita un slot libre.
    {
        USOLOItemBase* ResultAsset = Recipe->ResultItem.Get();
        bool bHasSpace = false;

        if (ResultAsset)
        {
            // Si el item tiene MaxStackSize > 1, puede apilarse en un slot existente
            if (ResultAsset->MaxStackSize > 1)
            {
                const int32 CurrentQty = InvComp->GetItemQuantity(ResultAsset->ItemID);
                // Calculamos cuÃ¡ntos items caben en los stacks actuales
                // (cantidad total en slots existentes vs. mÃ¡ximo si todos estuvieran llenos)
                const int32 SlotsUsedCount = [&]() -> int32
                {
                    int32 Count = 0;
                    for (const FInventorySlot& Slot : InvComp->InventorySlots)
                    {
                        if (!Slot.IsEmpty() &&
                            Slot.ItemInstance &&
                            Slot.ItemInstance->GetClass())  // instancia vÃ¡lida
                        {
                            Count++;
                        }
                    }
                    return Count;
                }();

                // Si la cantidad actual en todos los stacks no llena un stack completo,
                // todavÃ­a hay espacio. O si hay slots vacÃ­os, tambiÃ©n podemos aÃ±adir.
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
                    // Verificar si hay un slot vacÃ­o
                    const int32 OccupiedSlots = SlotsUsedCount;
                    bHasSpace = OccupiedSlots < InvComp->MaxInventorySlots;
                }
            }
            else
            {
                // MaxStackSize == 1: necesita slot vacÃ­o por cada unidad
                int32 EmptySlots = 0;
                for (const FInventorySlot& Slot : InvComp->InventorySlots)
                {
                    if (Slot.IsEmpty())
                    {
                        EmptySlots++;
                    }
                }
                // TambiÃ©n contar slots que aÃºn no existen en el array (por debajo de MaxInventorySlots)
                const int32 UnallocatedSlots =
                    InvComp->MaxInventorySlots - InvComp->InventorySlots.Num();
                EmptySlots += UnallocatedSlots;

                bHasSpace = EmptySlots >= Recipe->ResultQuantity;
            }
        }
        else
        {
            // Asset no cargado â€” verificar si hay al menos un slot libre
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

// â”€â”€â”€ CRAFTING ENTRY POINT â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

FCraftingAttemptResult USOLOCraftingComponent::CraftItem(FName RecipeID)
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
    USOLORecipeDataAsset* Recipe = Cast<USOLORecipeDataAsset>(
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

    USOLOInventoryComponent* InvComp = GetInventoryComponent();
    if (!InvComp)
    {
        FailResult.Result = ECraftingResult::Failed_MissingIngredients;
        return FailResult;
    }

    if (Recipe->CraftingTime > 0.f)
    {
        // â”€â”€ Crafteo con timer â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
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
                &USOLOCraftingComponent::FinishCrafting,
                Recipe->CraftingTime,
                false);
        }

        OnCraftingStarted.Broadcast(RecipeID, Recipe->CraftingTime);

        // Devolvemos Success con Quantity == 0 para indicar que estÃ¡ en progreso
        FCraftingAttemptResult PendingResult;
        PendingResult.Result = ECraftingResult::Success;
        PendingResult.CraftedItem = Recipe->ResultItem;
        PendingResult.CraftedQuantity = 0;
        return PendingResult;
    }
    else
    {
        // â”€â”€ Crafteo instantÃ¡neo â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
        ActiveRecipe = Recipe;
        bIsCrafting = true;
        CurrentCraftingRecipeID = RecipeID;
        PendingConsumedIngredients.Empty();
        PendingConsumedCurrency = 0;
        LastInstantResult = FCraftingAttemptResult(); // Reset before FinishCrafting fills it

        FinishCrafting();

        // FinishCrafting wrote into LastInstantResult before broadcasting â€” return it directly.
        return LastInstantResult;
    }
}

// â”€â”€â”€ FINISH CRAFTING â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOCraftingComponent::FinishCrafting()
{
    if (!ActiveRecipe)
    {
        bIsCrafting = false;
        CurrentCraftingRecipeID = NAME_None;
        return;
    }

    USOLOInventoryComponent* InvComp = GetInventoryComponent();

    FCraftingAttemptResult Result;
    Result.CraftedItem = ActiveRecipe->ResultItem;

    // Si los ingredientes aÃºn no fueron consumidos (crafteo instantÃ¡neo), consumirlos ahora
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

    // â”€â”€ Critical success roll â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    const bool bCritical = FMath::FRand() < ActiveRecipe->CriticalSuccessChance;
    Result.bWasCriticalSuccess = bCritical;

    int32 FinalQuantity = ActiveRecipe->ResultQuantity;

    if (bCritical)
    {
        // Intentar subir la rareza del item resultado
        USOLOItemBase* ResultAsset = ActiveRecipe->ResultItem.Get();
        if (ResultAsset)
        {
            const ESOLOItemRarity CurrentRarity = ResultAsset->Rarity;
            // Solo subir rareza si no es ya Legendary o Divine
            if (CurrentRarity != ESOLOItemRarity::Legendary &&
                CurrentRarity != ESOLOItemRarity::Divine)
            {
                // La rareza sube un nivel. Como el sistema de items usa un DataAsset inmutable,
                // en la prÃ¡ctica el critical bonus se traduce en +1 de cantidad extra.
                // Si el pipeline del proyecto tiene items mutables por rareza, esto se puede
                // extender aquÃ­. Por ahora siempre aÃ±ade +1 unidad como bonus crÃ­tico.
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

    // â”€â”€ AÃ±adir resultado al inventario â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    if (InvComp)
    {
        USOLOItemBase* ResultAsset = ActiveRecipe->ResultItem.Get();
        if (ResultAsset)
        {
            const bool bAdded = InvComp->AddItem(ResultAsset, FinalQuantity);
            if (!bAdded)
            {
                // El inventario se llenÃ³ entre la validaciÃ³n y ahora (edge case en multijugador).
                // Reembolsamos los ingredientes consumidos.
                // PendingConsumedIngredients contiene los ingredientes si fue un crafteo con timer;
                // para crafteos instantÃ¡neos usamos directamente la lista de la receta.
                const TArray<FCraftingIngredient>& IngredientsToRefund =
                    PendingConsumedIngredients.Num() > 0
                    ? PendingConsumedIngredients
                    : ActiveRecipe->Ingredients;

                for (const FCraftingIngredient& Ingredient : IngredientsToRefund)
                {
                    USOLOItemBase* IngredientAsset = Ingredient.Item.Get();
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
                    // Crafteo instantÃ¡neo: el currency ya fue gastado arriba, devolverlo
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
            // Asset no estaba cargado en memoria â€” fallo suave
            Result.Result = ECraftingResult::Failed_RecipeUnknown;
            Result.CraftedQuantity = 0;
        }
    }
    else
    {
        Result.Result = ECraftingResult::Failed_InventoryFull;
        Result.CraftedQuantity = 0;
    }

    // â”€â”€ Limpiar estado â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    PendingConsumedIngredients.Empty();
    PendingConsumedCurrency = 0;

    const FName FinishedRecipeID = CurrentCraftingRecipeID;

    bIsCrafting = false;
    CurrentCraftingRecipeID = NAME_None;
    ActiveRecipe = nullptr;

    // â”€â”€ Cachear para el retorno sÃ­ncrono (crafteo instantÃ¡neo) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    LastInstantResult = Result;

    // â”€â”€ Notificar â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    OnCraftingCompleted.Broadcast(Result);
}

// â”€â”€â”€ CANCEL â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOCraftingComponent::CancelCrafting()
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
    USOLOInventoryComponent* InvComp = GetInventoryComponent();
    if (InvComp)
    {
        for (const FCraftingIngredient& Ingredient : PendingConsumedIngredients)
        {
            USOLOItemBase* ItemAsset = Ingredient.Item.Get();
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

// â”€â”€â”€ QUERIES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

TArray<USOLORecipeDataAsset*> USOLOCraftingComponent::GetKnownRecipesByCategory(
    ECraftingCategory Category) const
{
    TArray<USOLORecipeDataAsset*> Result;
    TArray<USOLORecipeDataAsset*> AllKnown = GetAllKnownRecipesLoaded();

    for (USOLORecipeDataAsset* Recipe : AllKnown)
    {
        if (Recipe && Recipe->Category == Category)
        {
            Result.Add(Recipe);
        }
    }

    return Result;
}

TArray<USOLORecipeDataAsset*> USOLOCraftingComponent::GetCraftableRecipes() const
{
    TArray<USOLORecipeDataAsset*> Result;
    TArray<USOLORecipeDataAsset*> AllKnown = GetAllKnownRecipesLoaded();

    for (USOLORecipeDataAsset* Recipe : AllKnown)
    {
        if (Recipe && CanCraft(Recipe) == ECraftingResult::Success)
        {
            Result.Add(Recipe);
        }
    }

    return Result;
}

// â”€â”€â”€ PRIVATE HELPERS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

USOLOInventoryComponent* USOLOCraftingComponent::GetInventoryComponent() const
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    return Owner->FindComponentByClass<USOLOInventoryComponent>();
}

USOLOSkillMasteryComponent* USOLOCraftingComponent::GetMasteryComponent() const
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    return Owner->FindComponentByClass<USOLOSkillMasteryComponent>();
}

UAbilitySystemComponent* USOLOCraftingComponent::GetASC() const
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

int32 USOLOCraftingComponent::GetPlayerLevel() const
{
    UAbilitySystemComponent* ASC = GetASC();
    if (!ASC)
    {
        return 1;
    }

    // Leer el atributo Level directamente del AttributeSet
    ASOLOPlayerState* PS = Cast<ASOLOPlayerState>(GetOwner());
    if (PS)
    {
        return PS->GetPlayerLevel();
    }

    // Fallback: leer directamente del ASC con el getter generado por la macro ATTRIBUTE_ACCESSORS
    const USOLOAttributeSet* AttrSet =
        ASC->GetSet<USOLOAttributeSet>();
    if (AttrSet)
    {
        return FMath::FloorToInt(AttrSet->GetLevel());
    }

    return 1;
}

ESOLOClass USOLOCraftingComponent::GetPlayerClass() const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return ESOLOClass::Warrior;
    }

    USOLOGameInstance* GI = World->GetGameInstance<USOLOGameInstance>();
    if (!GI)
    {
        return ESOLOClass::Warrior;
    }

    return GI->GetActivePlayerData().Class;
}

TArray<USOLORecipeDataAsset*> USOLOCraftingComponent::GetAllKnownRecipesLoaded() const
{
    TArray<USOLORecipeDataAsset*> Result;

    if (KnownRecipeIDs.Num() == 0)
    {
        return Result;
    }

    UAssetManager& AssetManager = UAssetManager::Get();

    for (const FName& RecipeID : KnownRecipeIDs)
    {
        const FPrimaryAssetId AssetId(FPrimaryAssetType("CraftingRecipe"), RecipeID);
        USOLORecipeDataAsset* Recipe = Cast<USOLORecipeDataAsset>(
            AssetManager.GetPrimaryAssetObject(AssetId));

        if (Recipe)
        {
            Result.Add(Recipe);
        }
    }

    return Result;
}
