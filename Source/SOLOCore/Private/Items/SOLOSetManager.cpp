#include "Items/SOLOSetManager.h"
#include "AbilitySystemComponent.h"

void USOLOSetManager::RefreshSetBonuses(UAbilitySystemComponent* ASC, const TArray<FName>& EquippedItemIDs)
{
	if (!ASC || !SetDataTable) return;

	// Remove all existing set bonuses for this ASC
	if (TArray<FActiveGameplayEffectHandle>* Handles = ActiveBonuses.Find(ASC))
	{
		for (const FActiveGameplayEffectHandle& H : *Handles)
			ASC->RemoveActiveGameplayEffect(H);
		Handles->Empty();
	}

	TArray<FSOLOSetData*> AllSets;
	SetDataTable->GetAllRows<FSOLOSetData>(TEXT("SetManager"), AllSets);

	TArray<FActiveGameplayEffectHandle>& NewHandles = ActiveBonuses.FindOrAdd(ASC);

	for (const FSOLOSetData* SetData : AllSets)
	{
		if (!SetData) continue;
		int32 Pieces = CountEquippedPieces(*SetData, EquippedItemIDs);

		auto ApplyBonus = [&](TSubclassOf<UGameplayEffect> BonusClass)
		{
			if (!BonusClass) return;
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(BonusClass, 1, Ctx);
			if (Spec.IsValid())
				NewHandles.Add(ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get()));
		};

		if (Pieces >= 4 && SetData->Bonus4Pieces) ApplyBonus(SetData->Bonus4Pieces);
		else if (Pieces >= 2 && SetData->Bonus2Pieces) ApplyBonus(SetData->Bonus2Pieces);
	}
}

int32 USOLOSetManager::CountEquippedPieces(const FSOLOSetData& SetData, const TArray<FName>& EquippedIDs) const
{
	int32 Count = 0;
	for (const FName& PieceID : SetData.PieceIDs)
		if (EquippedIDs.Contains(PieceID)) Count++;
	return Count;
}
