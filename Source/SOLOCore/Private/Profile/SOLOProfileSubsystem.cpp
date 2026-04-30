#include "Profile/SOLOProfileSubsystem.h"
#include "Profile/SOLOLocalProfileSave.h"
#include "Profile/SOLOAdminAuth.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSOLOProfile, Log, All);

const TCHAR* USOLOProfileSubsystem::SaveSlotName = TEXT("SOLO_LocalProfile");

void USOLOProfileSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadFromDisk();

	if (USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance()))
	{
		if (!CachedDisplayName.IsEmpty())
		{
			GI->ActivePlayerData.CharacterName = CachedDisplayName;
		}
	}
}

bool USOLOProfileSubsystem::HasValidDisplayName() const
{
	FString Copy = CachedDisplayName;
	Copy.TrimStartAndEndInline();
	return !Copy.IsEmpty();
}

void USOLOProfileSubsystem::SetDisplayNameAndPersist(const FString& InName)
{
	CachedDisplayName = SanitizeDisplayName(InName);

	if (USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance()))
	{
		GI->ActivePlayerData.CharacterName = CachedDisplayName;
	}

	SaveToDisk();
	UE_LOG(LogSOLOProfile, Log, TEXT("Perfil local actualizado: \"%s\""), *CachedDisplayName);
}

bool USOLOProfileSubsystem::TryCompleteMainMenuLogin(const FString& DisplayName, const FString& OptionalPassword, FString& OutError)
{
	OutError.Reset();

	USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance());
	if (!GI)
	{
		OutError = TEXT("GameInstance no disponible.");
		return false;
	}

	FString Name = DisplayName;
	Name.TrimStartAndEndInline();

	if (Name.IsEmpty())
	{
		OutError = TEXT("EscribÃ­ un nombre antes de jugar.");
		return false;
	}

	FString Password = OptionalPassword;
	Password.TrimStartAndEndInline();

	if (Password.IsEmpty())
	{
		GI->ClearAdminSession();
		SetDisplayNameAndPersist(Name);
		return true;
	}

	FString CanonicalAdmin;
	if (!SOLOAdminAuth::IsAdminUsername(Name, CanonicalAdmin))
	{
		OutError = TEXT("La contraseÃ±a solo aplica a usuarios Admin0..Admin10.");
		return false;
	}

	if (!SOLOAdminAuth::VerifyAdminPassword(Password))
	{
		OutError = TEXT("ContraseÃ±a incorrecta.");
		return false;
	}

	GI->SetAdminSession(true, CanonicalAdmin);
	SetDisplayNameAndPersist(CanonicalAdmin);
	UE_LOG(LogSOLOProfile, Log, TEXT("SesiÃ³n admin: %s"), *CanonicalAdmin);
	return true;
}

void USOLOProfileSubsystem::LoadFromDisk()
{
	CachedDisplayName.Reset();

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		if (USOLOLocalProfileSave* Loaded =
		        Cast<USOLOLocalProfileSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex)))
		{
			CachedDisplayName = Loaded->DisplayName;
		}
	}

	CachedDisplayName = SanitizeDisplayName(CachedDisplayName);
}

void USOLOProfileSubsystem::SaveToDisk()
{
	USOLOLocalProfileSave* Save = Cast<USOLOLocalProfileSave>(UGameplayStatics::CreateSaveGameObject(USOLOLocalProfileSave::StaticClass()));
	if (!Save)
	{
		return;
	}

	Save->DisplayName = CachedDisplayName;
	UGameplayStatics::SaveGameToSlot(Save, SaveSlotName, SaveUserIndex);
}

FString USOLOProfileSubsystem::SanitizeDisplayName(const FString& Raw) const
{
	FString Out = Raw;
	Out.TrimStartAndEndInline();

	if (Out.Len() > 24)
	{
		Out.LeftInline(24, EAllowShrinking::No);
		Out.TrimEndInline();
	}

	return Out;
}
