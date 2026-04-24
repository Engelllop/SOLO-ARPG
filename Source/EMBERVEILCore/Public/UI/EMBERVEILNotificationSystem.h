#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EMBERVEILNotificationSystem.generated.h"

class UEMBERVEILItemBase;
class APlayerController;

UENUM(BlueprintType)
enum class ENotificationType : uint8
{
	Info        UMETA(DisplayName = "Info"),
	Success     UMETA(DisplayName = "Success"),
	Warning     UMETA(DisplayName = "Warning"),
	Error       UMETA(DisplayName = "Error"),
	ItemPickup  UMETA(DisplayName = "Item Pickup"),
	LevelUp     UMETA(DisplayName = "Level Up"),
	QuestUpdate UMETA(DisplayName = "Quest Update")
};

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FEMBERVEILNotification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notification")
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notification")
	float Duration = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notification")
	ENotificationType Type = ENotificationType::Info;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationPushed, FEMBERVEILNotification, Notification);

/**
 * Cola ligera de notificaciones para la UI (toast, feed, etc.).
 * Emite OnNotificationPushed; el HUD/widget se suscribe en el cliente.
 *
 * Desde el servidor, usar NotifyOwningClient para llegar al cliente poseedor.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILNotificationSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Emite en el GameInstance local (cliente / PIE). No cruza red. */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void PushNotification(FText Message, ENotificationType Type, float Duration = 3.f);

	UFUNCTION(BlueprintCallable, Category = "Notification")
	void PushItemPickup(UEMBERVEILItemBase* Item, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Notification")
	void PushLevelUp(int32 NewLevel);

	/** Servidor → cliente: mismo mensaje que PushLevelUp pero usando la cola de NotifyOwningClient. */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	static void NotifyLevelUpOwningClient(APlayerController* OwningPlayer, int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = "Notification")
	void PushQuestUpdate(FText QuestName, FText ObjectiveText);

	UFUNCTION(BlueprintCallable, Category = "Notification")
	void PushSaveConfirmation();

	/** Servidor → cliente poseedor del PlayerController. Seguro llamar desde HasAuthority. */
	static void NotifyOwningClient(APlayerController* OwningPlayer, const FEMBERVEILNotification& Payload);

	UPROPERTY(BlueprintAssignable, Category = "Notification")
	FOnNotificationPushed OnNotificationPushed;

	/** Emite OnNotificationPushed en el GameInstance actual (uso desde Client RPC o lógica local). */
	void BroadcastLocal(const FEMBERVEILNotification& Notification);

private:
	void EmitLocalPayload(const FEMBERVEILNotification& Notification);
};
