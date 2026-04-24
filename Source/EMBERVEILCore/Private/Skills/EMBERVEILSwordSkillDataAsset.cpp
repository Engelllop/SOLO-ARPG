#include "Skills/EMBERVEILSwordSkillDataAsset.h"

UEMBERVEILSwordSkillDataAsset::UEMBERVEILSwordSkillDataAsset()
{
	// Los DataAssets no necesitan inicialización especial en el constructor;
	// todos los valores por defecto están declarados en el header.
}

FPrimaryAssetId UEMBERVEILSwordSkillDataAsset::GetPrimaryAssetId() const
{
	// El tipo "SwordSkill" permite al AssetManager agrupar y cargar
	// todos los skills con: AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("SwordSkill"))
	return FPrimaryAssetId(FPrimaryAssetType("SwordSkill"), GetFName());
}
