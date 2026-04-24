#include "GameFramework/EMBERVEILGameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UEMBERVEILGameInstance::UEMBERVEILGameInstance()
{
}

void UEMBERVEILGameInstance::Init()
{
	Super::Init();
	ClearAdminSession();
	UE_LOG(LogTemp, Log, TEXT("EMBERVEIL GameInstance initialized."));
}

void UEMBERVEILGameInstance::Shutdown()
{
	ClearAdminSession();
	Super::Shutdown();
}

void UEMBERVEILGameInstance::ClearAdminSession()
{
	bAdminUnrestrictedAccess = false;
	AdminSessionAccountName.Reset();
}

void UEMBERVEILGameInstance::SetAdminSession(bool bEnabled, const FString& CanonicalAdminName)
{
	bAdminUnrestrictedAccess = bEnabled;
	AdminSessionAccountName = bEnabled ? CanonicalAdminName : FString();
}

void UEMBERVEILGameInstance::TravelToMap(const FString& MapPath, bool bAbsolute)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!PC) return;

	if (bAbsolute)
		PC->ClientTravel(MapPath, TRAVEL_Absolute);
	else
		GetWorld()->ServerTravel(MapPath);
}

void UEMBERVEILGameInstance::TravelToGameplayMap()
{
	if (GameplayMapPackage.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayMapPackage vacío: configurá el mapa en GameInstance."));
		return;
	}

	// Asegura GameMode de gameplay aunque el mapa aún no tenga override en World Settings.
	const FString Options = TEXT("?Game=/Script/EMBERVEILCore.EMBERVEILGameMode");
	UGameplayStatics::OpenLevel(this, FName(*GameplayMapPackage), true, Options);
}

FString UEMBERVEILGameInstance::GetLocalUserId() const
{
	if (const ULocalPlayer* LP = GetFirstGamePlayer())
	{
		if (const FUniqueNetIdPtr NetId = LP->GetPreferredUniqueNetId().GetUniqueNetId())
			return NetId->ToString();
	}
	return TEXT("LocalPlayer_0");
}
