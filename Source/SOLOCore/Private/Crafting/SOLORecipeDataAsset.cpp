#include "Crafting/SOLORecipeDataAsset.h"

USOLORecipeDataAsset::USOLORecipeDataAsset()
{
}

FPrimaryAssetId USOLORecipeDataAsset::GetPrimaryAssetId() const
{
    // El AssetManager identifica estas recetas bajo el tipo "CraftingRecipe".
    // En DefaultGame.ini aÃ±adir:
    //   [/Script/Engine.AssetManagerSettings]
    //   +PrimaryAssetTypesToScan=(PrimaryAssetType="CraftingRecipe",
    //       AssetBaseClass=/Script/SOLOCore.SOLORecipeDataAsset,
    //       bHasBlueprintClasses=False, bIsEditorOnly=False,
    //       Directories=((Path="/Game/Data/Recipes")))
    return FPrimaryAssetId(FPrimaryAssetType("CraftingRecipe"), GetFName());
}
