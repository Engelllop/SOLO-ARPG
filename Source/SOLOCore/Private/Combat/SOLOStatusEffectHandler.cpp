#include "Combat/SOLOStatusEffectHandler.h"
#include "AbilitySystemComponent.h"

const TArray<FStatusEffectInteraction>& USOLOStatusEffectHandler::GetInteractions()
{
	static TArray<FStatusEffectInteraction> Interactions;
	if (Interactions.Num() == 0)
	{
		auto Add = [&](const FName& A, const FName& B, const FName& Name)
		{
			FStatusEffectInteraction I;
			I.StatusA = FGameplayTag::RequestGameplayTag(A);
			I.StatusB = FGameplayTag::RequestGameplayTag(B);
			I.InteractionName = FText::FromName(Name);
			I.bConsumeBoth = true;
			Interactions.Add(I);
		};

		Add("Status.Burn",   "Status.Poison",  "Combustion");
		Add("Status.Wet",    "Status.Freeze",   "InstantFreeze");
		Add("Status.Wet",    "Status.Shock",    "Electrocution");
		Add("Status.Freeze", "Status.Shock",    "CrystalExplosion");
	}
	return Interactions;
}

void USOLOStatusEffectHandler::CheckStatusInteractions(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;

	FGameplayTagContainer OwnedTags;
	ASC->GetOwnedGameplayTags(OwnedTags);

	for (const auto& Interaction : GetInteractions())
	{
		if (OwnedTags.HasTag(Interaction.StatusA) && OwnedTags.HasTag(Interaction.StatusB))
		{
			if (Interaction.ResultEffect)
			{
				FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(Interaction.ResultEffect, 1.f, Ctx);
				if (Spec.IsValid())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
				}
			}

			if (Interaction.bConsumeBoth)
			{
				ASC->RemoveLooseGameplayTag(Interaction.StatusA);
				ASC->RemoveLooseGameplayTag(Interaction.StatusB);
			}
		}
	}
}

void USOLOStatusEffectHandler::ApplyStatusImmunity(UAbilitySystemComponent* ASC, FGameplayTag StatusTag, float Duration)
{
	if (!ASC) return;

	FName ImmunityName = FName(*FString::Printf(TEXT("Immunity.%s"), *StatusTag.GetTagName().ToString()));
	FGameplayTag ImmTag = FGameplayTag::RequestGameplayTag(ImmunityName, false);
	if (!ImmTag.IsValid()) return;

	ASC->AddLooseGameplayTag(ImmTag);

	FTimerHandle Handle;
	FTimerDelegate Delegate;
	Delegate.BindLambda([ASC, ImmTag]()
	{
		if (ASC) ASC->RemoveLooseGameplayTag(ImmTag);
	});
	ASC->GetOwner()->GetWorldTimerManager().SetTimer(Handle, Delegate, Duration, false);
}

bool USOLOStatusEffectHandler::IsImmuneToStatus(UAbilitySystemComponent* ASC, FGameplayTag StatusTag)
{
	if (!ASC) return false;

	FName ImmunityName = FName(*FString::Printf(TEXT("Immunity.%s"), *StatusTag.GetTagName().ToString()));
	FGameplayTag ImmTag = FGameplayTag::RequestGameplayTag(ImmunityName, false);
	if (!ImmTag.IsValid()) return false;

	return ASC->HasMatchingGameplayTag(ImmTag);
}

float USOLOStatusEffectHandler::GetImmunityDuration(FGameplayTag StatusTag, bool bIsBoss)
{
	float Base = 5.f;
	FName TagName = StatusTag.GetTagName();

	if (TagName == FName("Status.Stun"))   Base = 10.f;
	else if (TagName == FName("Status.Fear"))   Base = 15.f;
	else if (TagName == FName("Status.Freeze")) Base = 8.f;

	return bIsBoss ? Base * 0.4f : Base;
}
