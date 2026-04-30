#include "Skills/SOLOSwordSkillDataAsset.h"

USOLOSwordSkillDataAsset::USOLOSwordSkillDataAsset()
{
	// Los DataAssets no necesitan inicializaciÃ³n especial en el constructor;
	// todos los valores por defecto estÃ¡n declarados en el header.
}

FPrimaryAssetId USOLOSwordSkillDataAsset::GetPrimaryAssetId() const
{
	// El tipo "SwordSkill" permite al AssetManager agrupar y cargar
	// todos los skills con: AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("SwordSkill"))
	return FPrimaryAssetId(FPrimaryAssetType("SwordSkill"), GetFName());
}
