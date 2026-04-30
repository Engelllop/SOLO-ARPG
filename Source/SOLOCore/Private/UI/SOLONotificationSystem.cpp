#include "UI/SOLONotificationSystem.h"
#include "Inventory/SOLOItemBase.h"
#include "GameFramework/PlayerController.h"
#include "Player/SOLOPlayerController.h"
#include "Engine/GameInstance.h"

#define LOCTEXT_NAMESPACE "SOLONotification"

void USOLONotificationSystem::EmitLocalPayload(const FSOLONotification& Notification)
{
	OnNotificationPushed.Broadcast(Notification);
}

void USOLONotificationSystem::BroadcastLocal(const FSOLONotification& Notification)
{
	EmitLocalPayload(Notification);
}

void USOLONotificationSystem::PushNotification(FText Message, ENotificationType Type, float Duration)
{
	FSOLONotification N;
	N.Message = MoveTemp(Message);
	N.Type = Type;
	N.Duration = Duration;
	EmitLocalPayload(N);
}

void USOLONotificationSystem::PushItemPickup(USOLOItemBase* Item, int32 Quantity)
{
	if (!IsValid(Item) || Quantity <= 0)
	{
		return;
	}

	const FText Msg = FText::Format(
		LOCTEXT("ItemPickupFmt", "Obtenido: {0} x{1}"),
		Item->ItemName,
		FText::AsNumber(Quantity));

	FSOLONotification N;
	N.Message = Msg;
	N.Type = ENotificationType::ItemPickup;
	N.Duration = 3.f;
	EmitLocalPayload(N);
}

void USOLONotificationSystem::PushLevelUp(int32 NewLevel)
{
	FSOLONotification N;
	N.Message = FText::Format(LOCTEXT("LevelUpFmt", "Â¡Nivel {0} alcanzado!"), FText::AsNumber(NewLevel));
	N.Type = ENotificationType::LevelUp;
	N.Duration = 4.f;
	EmitLocalPayload(N);
}

void USOLONotificationSystem::NotifyLevelUpOwningClient(APlayerController* OwningPlayer, int32 NewLevel)
{
	FSOLONotification N;
	N.Message = FText::Format(LOCTEXT("LevelUpFmtNet", "Â¡Nivel {0} alcanzado!"), FText::AsNumber(NewLevel));
	N.Type = ENotificationType::LevelUp;
	N.Duration = 4.f;
	NotifyOwningClient(OwningPlayer, N);
}

void USOLONotificationSystem::PushQuestUpdate(FText QuestName, FText ObjectiveText)
{
	const FText Msg = FText::Format(
		LOCTEXT("QuestUpdateFmt", "MisiÃ³n: {0} â€” {1}"),
		QuestName,
		ObjectiveText);

	FSOLONotification N;
	N.Message = Msg;
	N.Type = ENotificationType::QuestUpdate;
	N.Duration = 4.f;
	EmitLocalPayload(N);
}

void USOLONotificationSystem::PushSaveConfirmation()
{
	FSOLONotification N;
	N.Message = LOCTEXT("SaveOk", "Partida guardada");
	N.Type = ENotificationType::Success;
	N.Duration = 2.5f;
	EmitLocalPayload(N);
}

void USOLONotificationSystem::NotifyOwningClient(APlayerController* OwningPlayer, const FSOLONotification& Payload)
{
	if (!IsValid(OwningPlayer))
	{
		return;
	}

	if (OwningPlayer->IsLocalController())
	{
		if (UGameInstance* GI = OwningPlayer->GetGameInstance())
		{
			if (USOLONotificationSystem* Sys = GI->GetSubsystem<USOLONotificationSystem>())
			{
				Sys->BroadcastLocal(Payload);
			}
		}
		return;
	}

	if (OwningPlayer->GetLocalRole() == ROLE_Authority)
	{
		if (ASOLOPlayerController* EPC = Cast<ASOLOPlayerController>(OwningPlayer))
		{
			EPC->ClientReceiveNotification(Payload.Message, static_cast<uint8>(Payload.Type), Payload.Duration);
		}
	}
}

#undef LOCTEXT_NAMESPACE
