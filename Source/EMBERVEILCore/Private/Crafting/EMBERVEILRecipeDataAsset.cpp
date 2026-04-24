#include "Crafting/EMBERVEILRecipeDataAsset.h"

UEMBERVEILRecipeDataAsset::UEMBERVEILRecipeDataAsset()
{
}

FPrimaryAssetId UEMBERVEILRecipeDataAsset::GetPrimaryAssetId() const
{
    // El AssetManager identifica estas recetas bajo el tipo "CraftingRecipe".
    // En DefaultGame.ini añadir:
    //   [/Script/Engine.AssetManagerSettings]
    //   +PrimaryAssetTypesToScan=(PrimaryAssetType="CraftingRecipe",
    //       AssetBaseClass=/Script/EMBERVEILCore.EMBERVEILRecipeDataAsset,
    //       bHasBlueprintClasses=False, bIsEditorOnly=False,
    //       Directories=((Path="/Game/Data/Recipes")))
    return FPrimaryAssetId(FPrimaryAssetType("CraftingRecipe"), GetFName());
}
