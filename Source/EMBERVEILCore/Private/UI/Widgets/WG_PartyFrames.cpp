#include "UI/Widgets/WG_PartyFrames.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "Player/EMBERVEILPlayerState.h"
#include "Attributes/EMBERVEILAttributeSet.h"

void UWG_PartyFrames::NativeConstruct()
{
	Super::NativeConstruct();

	SetPartyVisible(false);
}

void UWG_PartyFrames::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	PollingAccumulator += InDeltaTime;
	if (PollingAccumulator >= PollingInterval)
	{
		PollingAccumulator = 0.f;
		PollMemberHealth();
	}
}

void UWG_PartyFrames::AddPartyMember(APlayerState* NewMember)
{
	if (!NewMember || PartyMembers.Num() >= MaxPartySize)
	{
		return;
	}

	FPartyMemberData Data;
	Data.MemberPlayerState = NewMember;
	// Clamp display name to MaxNameDisplayLength characters
	Data.PlayerName = NewMember->GetPlayerName().Left(MaxNameDisplayLength);
	Data.HealthPercent = 1.f;

	PartyMembers.Add(Data);
	SetPartyVisible(PartyMembers.Num() > 1);
	BP_RefreshPartyList();
}

void UWG_PartyFrames::RemovePartyMember(APlayerState* Member)
{
	const int32 Idx = PartyMembers.IndexOfByPredicate([Member](const FPartyMemberData& D)
	{
		return D.MemberPlayerState == Member;
	});

	if (Idx != INDEX_NONE)
	{
		PartyMembers.RemoveAt(Idx);
		SetPartyVisible(PartyMembers.Num() > 1);
		BP_RefreshPartyList();
	}
}

void UWG_PartyFrames::UpdateMemberHealth(int32 MemberIndex, float NewHealthPercent)
{
	if (!PartyMembers.IsValidIndex(MemberIndex))
	{
		return;
	}

	PartyMembers[MemberIndex].HealthPercent = FMath::Clamp(NewHealthPercent, 0.f, 1.f);
}

void UWG_PartyFrames::SetMemberDead(int32 MemberIndex, bool bDead)
{
	if (!PartyMembers.IsValidIndex(MemberIndex))
	{
		return;
	}

	PartyMembers[MemberIndex].bIsDead = bDead;
	BP_SetMemberDead(MemberIndex, bDead);
}

void UWG_PartyFrames::SetMemberOutOfRange(int32 MemberIndex, bool bOutOfRange)
{
	if (!PartyMembers.IsValidIndex(MemberIndex))
	{
		return;
	}

	PartyMembers[MemberIndex].bIsOutOfRange = bOutOfRange;
	BP_SetMemberOutOfRange(MemberIndex, bOutOfRange);
}

void UWG_PartyFrames::SetPartyVisible(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UWG_PartyFrames::PollMemberHealth()
{
	APlayerController* LocalPC = GetOwningPlayer();
	APawn* LocalPawn = LocalPC ? LocalPC->GetPawn() : nullptr;
	const FVector LocalLocation = LocalPawn ? LocalPawn->GetActorLocation() : FVector::ZeroVector;

	for (int32 i = 0; i < PartyMembers.Num(); ++i)
	{
		FPartyMemberData& Member = PartyMembers[i];
		if (!IsValid(Member.MemberPlayerState.Get()))
		{
			continue;
		}

		AEMBERVEILPlayerState* EPS = Cast<AEMBERVEILPlayerState>(Member.MemberPlayerState.Get());
		if (!EPS)
		{
			continue;
		}

		const float MaxHP = FMath::Max(EPS->GetMaxHealth(), 1.f);
		const float NewPercent = FMath::Clamp(EPS->GetHealth() / MaxHP, 0.f, 1.f);
		UpdateMemberHealth(i, NewPercent);

		const bool bDead = FMath::IsNearlyZero(NewPercent);
		if (bDead != Member.bIsDead)
		{
			SetMemberDead(i, bDead);
		}

		if (LocalPawn)
		{
			APawn* MemberPawn = EPS->GetPawn();
			if (MemberPawn)
			{
				const float DistSq = FVector::DistSquared(LocalLocation, MemberPawn->GetActorLocation());
				const bool bOutOfRange = DistSq > OutOfRangeDistanceSq;
				if (bOutOfRange != Member.bIsOutOfRange)
				{
					SetMemberOutOfRange(i, bOutOfRange);
				}
			}
		}
	}
}
