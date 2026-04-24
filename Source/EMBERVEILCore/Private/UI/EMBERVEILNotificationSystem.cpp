#include "UI/EMBERVEILNotificationSystem.h"
#include "Inventory/EMBERVEILItemBase.h"
#include "GameFramework/PlayerController.h"
#include "Player/EMBERVEILPlayerController.h"
#include "Engine/GameInstance.h"

#define LOCTEXT_NAMESPACE "EMBERVEILNotification"

void UEMBERVEILNotificationSystem::EmitLocalPayload(const FEMBERVEILNotification& Notification)
{
	OnNotificationPushed.Broadcast(Notification);
}

void UEMBERVEILNotificationSystem::BroadcastLocal(const FEMBERVEILNotification& Notification)
{
	EmitLocalPayload(Notification);
}

void UEMBERVEILNotificationSystem::PushNotification(FText Message, ENotificationType Type, float Duration)
{
	FEMBERVEILNotification N;
	N.Message = MoveTemp(Message);
	N.Type = Type;
	N.Duration = Duration;
	EmitLocalPayload(N);
}

void UEMBERVEILNotificationSystem::PushItemPickup(UEMBERVEILItemBase* Item, int32 Quantity)
{
	if (!IsValid(Item) || Quantity <= 0)
	{
		return;
	}

	const FText Msg = FText::Format(
		LOCTEXT("ItemPickupFmt", "Obtenido: {0} x{1}"),
		Item->ItemName,
		FText::AsNumber(Quantity));

	FEMBERVEILNotification N;
	N.Message = Msg;
	N.Type = ENotificationType::ItemPickup;
	N.Duration = 3.f;
	EmitLocalPayload(N);
}

void UEMBERVEILNotificationSystem::PushLevelUp(int32 NewLevel)
{
	FEMBERVEILNotification N;
	N.Message = FText::Format(LOCTEXT("LevelUpFmt", "¡Nivel {0} alcanzado!"), FText::AsNumber(NewLevel));
	N.Type = ENotificationType::LevelUp;
	N.Duration = 4.f;
	EmitLocalPayload(N);
}

void UEMBERVEILNotificationSystem::NotifyLevelUpOwningClient(APlayerController* OwningPlayer, int32 NewLevel)
{
	FEMBERVEILNotification N;
	N.Message = FText::Format(LOCTEXT("LevelUpFmtNet", "¡Nivel {0} alcanzado!"), FText::AsNumber(NewLevel));
	N.Type = ENotificationType::LevelUp;
	N.Duration = 4.f;
	NotifyOwningClient(OwningPlayer, N);
}

void UEMBERVEILNotificationSystem::PushQuestUpdate(FText QuestName, FText ObjectiveText)
{
	const FText Msg = FText::Format(
		LOCTEXT("QuestUpdateFmt", "Misión: {0} — {1}"),
		QuestName,
		ObjectiveText);

	FEMBERVEILNotification N;
	N.Message = Msg;
	N.Type = ENotificationType::QuestUpdate;
	N.Duration = 4.f;
	EmitLocalPayload(N);
}

void UEMBERVEILNotificationSystem::PushSaveConfirmation()
{
	FEMBERVEILNotification N;
	N.Message = LOCTEXT("SaveOk", "Partida guardada");
	N.Type = ENotificationType::Success;
	N.Duration = 2.5f;
	EmitLocalPayload(N);
}

void UEMBERVEILNotificationSystem::NotifyOwningClient(APlayerController* OwningPlayer, const FEMBERVEILNotification& Payload)
{
	if (!IsValid(OwningPlayer))
	{
		return;
	}

	if (OwningPlayer->IsLocalController())
	{
		if (UGameInstance* GI = OwningPlayer->GetGameInstance())
		{
			if (UEMBERVEILNotificationSystem* Sys = GI->GetSubsystem<UEMBERVEILNotificationSystem>())
			{
				Sys->BroadcastLocal(Payload);
			}
		}
		return;
	}

	if (OwningPlayer->GetLocalRole() == ROLE_Authority)
	{
		if (AEMBERVEILPlayerController* EPC = Cast<AEMBERVEILPlayerController>(OwningPlayer))
		{
			EPC->ClientReceiveNotification(Payload.Message, static_cast<uint8>(Payload.Type), Payload.Duration);
		}
	}
}

#undef LOCTEXT_NAMESPACE
