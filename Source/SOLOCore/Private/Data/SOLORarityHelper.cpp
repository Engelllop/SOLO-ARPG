#include "Data/SOLORarityHelper.h"

float USOLORarityHelper::GetMultiplier(ESOLOItemRarity Rarity)
{
	switch (Rarity)
	{
	case ESOLOItemRarity::Common:    return 1.0f;
	case ESOLOItemRarity::Uncommon:  return 1.15f;
	case ESOLOItemRarity::Rare:      return 1.35f;
	case ESOLOItemRarity::Epic:      return 1.6f;
	case ESOLOItemRarity::Legendary: return 2.0f;
	case ESOLOItemRarity::Divine:    return 2.5f;
	default:                         return 1.0f;
	}
}

FLinearColor USOLORarityHelper::GetColor(ESOLOItemRarity Rarity)
{
	switch (Rarity)
	{
	case ESOLOItemRarity::Common:    return FLinearColor(0.5f, 0.5f, 0.5f);
	case ESOLOItemRarity::Uncommon:  return FLinearColor(0.2f, 0.8f, 0.2f);
	case ESOLOItemRarity::Rare:      return FLinearColor(0.2f, 0.4f, 1.0f);
	case ESOLOItemRarity::Epic:      return FLinearColor(0.6f, 0.1f, 0.9f);
	case ESOLOItemRarity::Legendary: return FLinearColor(1.0f, 0.6f, 0.0f);
	case ESOLOItemRarity::Divine:    return FLinearColor(0.9f, 0.1f, 0.1f);
	default:                         return FLinearColor::White;
	}
}

float USOLORarityHelper::GetDropWeight(ESOLOItemRarity Rarity)
{
	switch (Rarity)
	{
	case ESOLOItemRarity::Common:    return 70.0f;
	case ESOLOItemRarity::Uncommon:  return 20.0f;
	case ESOLOItemRarity::Rare:      return 8.0f;
	case ESOLOItemRarity::Epic:      return 1.8f;
	case ESOLOItemRarity::Legendary: return 0.18f;
	case ESOLOItemRarity::Divine:    return 0.02f;
	default:                         return 70.0f;
	}
}

ESOLOItemRarity USOLORarityHelper::RollRarity(float LuckBonus)
{
	float Roll = FMath::FRandRange(0.f, 100.f);
	Roll -= LuckBonus;
	if (Roll < 0.02f) return ESOLOItemRarity::Divine;
	if (Roll < 0.2f)  return ESOLOItemRarity::Legendary;
	if (Roll < 2.0f)  return ESOLOItemRarity::Epic;
	if (Roll < 10.0f) return ESOLOItemRarity::Rare;
	if (Roll < 30.0f) return ESOLOItemRarity::Uncommon;
	return ESOLOItemRarity::Common;
}

FText USOLORarityHelper::GetRarityDisplayName(ESOLOItemRarity Rarity)
{
	switch (Rarity)
	{
	case ESOLOItemRarity::Common:    return NSLOCTEXT("SOLO", "Rarity_Common", "Common");
	case ESOLOItemRarity::Uncommon:  return NSLOCTEXT("SOLO", "Rarity_Uncommon", "Uncommon");
	case ESOLOItemRarity::Rare:      return NSLOCTEXT("SOLO", "Rarity_Rare", "Rare");
	case ESOLOItemRarity::Epic:      return NSLOCTEXT("SOLO", "Rarity_Epic", "Epic");
	case ESOLOItemRarity::Legendary: return NSLOCTEXT("SOLO", "Rarity_Legendary", "Legendary");
	case ESOLOItemRarity::Divine:    return NSLOCTEXT("SOLO", "Rarity_Divine", "Divine");
	default:                         return FText::GetEmpty();
	}
}
