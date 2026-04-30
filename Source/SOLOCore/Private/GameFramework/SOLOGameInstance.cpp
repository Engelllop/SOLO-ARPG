#include "GameFramework/SOLOGameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

USOLOGameInstance::USOLOGameInstance()
{
}

void USOLOGameInstance::Init()
{
	Super::Init();
	ClearAdminSession();
	UE_LOG(LogTemp, Log, TEXT("SOLO GameInstance initialized."));
}

void USOLOGameInstance::Shutdown()
{
	ClearAdminSession();
	Super::Shutdown();
}

void USOLOGameInstance::ClearAdminSession()
{
	bAdminUnrestrictedAccess = false;
	AdminSessionAccountName.Reset();
}

void USOLOGameInstance::SetAdminSession(bool bEnabled, const FString& CanonicalAdminName)
{
	bAdminUnrestrictedAccess = bEnabled;
	AdminSessionAccountName = bEnabled ? CanonicalAdminName : FString();
}

void USOLOGameInstance::TravelToMap(const FString& MapPath, bool bAbsolute)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!PC) return;

	if (bAbsolute)
		PC->ClientTravel(MapPath, TRAVEL_Absolute);
	else
		GetWorld()->ServerTravel(MapPath);
}

void USOLOGameInstance::TravelToGameplayMap()
{
	if (GameplayMapPackage.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayMapPackage vacÃ­o: configurÃ¡ el mapa en GameInstance."));
		return;
	}

	// Asegura GameMode de gameplay aunque el mapa aÃºn no tenga override en World Settings.
	const FString Options = TEXT("?Game=/Script/SOLOCore.SOLOGameMode");
	UGameplayStatics::OpenLevel(this, FName(*GameplayMapPackage), true, Options);
}

FString USOLOGameInstance::GetLocalUserId() const
{
	if (const ULocalPlayer* LP = GetFirstGamePlayer())
	{
		if (const FUniqueNetIdPtr NetId = LP->GetPreferredUniqueNetId().GetUniqueNetId())
			return NetId->ToString();
	}
	return TEXT("LocalPlayer_0");
}
