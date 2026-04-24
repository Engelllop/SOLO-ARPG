#include "Profile/EMBERVEILProfileSubsystem.h"
#include "Profile/EMBERVEILLocalProfileSave.h"
#include "Profile/EMBERVEILAdminAuth.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogEMBERVEILProfile, Log, All);

const TCHAR* UEMBERVEILProfileSubsystem::SaveSlotName = TEXT("EMBERVEIL_LocalProfile");

void UEMBERVEILProfileSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadFromDisk();

	if (UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance()))
	{
		if (!CachedDisplayName.IsEmpty())
		{
			GI->ActivePlayerData.CharacterName = CachedDisplayName;
		}
	}
}

bool UEMBERVEILProfileSubsystem::HasValidDisplayName() const
{
	FString Copy = CachedDisplayName;
	Copy.TrimStartAndEndInline();
	return !Copy.IsEmpty();
}

void UEMBERVEILProfileSubsystem::SetDisplayNameAndPersist(const FString& InName)
{
	CachedDisplayName = SanitizeDisplayName(InName);

	if (UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance()))
	{
		GI->ActivePlayerData.CharacterName = CachedDisplayName;
	}

	SaveToDisk();
	UE_LOG(LogEMBERVEILProfile, Log, TEXT("Perfil local actualizado: \"%s\""), *CachedDisplayName);
}

bool UEMBERVEILProfileSubsystem::TryCompleteMainMenuLogin(const FString& DisplayName, const FString& OptionalPassword, FString& OutError)
{
	OutError.Reset();

	UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance());
	if (!GI)
	{
		OutError = TEXT("GameInstance no disponible.");
		return false;
	}

	FString Name = DisplayName;
	Name.TrimStartAndEndInline();

	if (Name.IsEmpty())
	{
		OutError = TEXT("Escribí un nombre antes de jugar.");
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
	if (!EMBERVEILAdminAuth::IsAdminUsername(Name, CanonicalAdmin))
	{
		OutError = TEXT("La contraseña solo aplica a usuarios Admin0..Admin10.");
		return false;
	}

	if (!EMBERVEILAdminAuth::VerifyAdminPassword(Password))
	{
		OutError = TEXT("Contraseña incorrecta.");
		return false;
	}

	GI->SetAdminSession(true, CanonicalAdmin);
	SetDisplayNameAndPersist(CanonicalAdmin);
	UE_LOG(LogEMBERVEILProfile, Log, TEXT("Sesión admin: %s"), *CanonicalAdmin);
	return true;
}

void UEMBERVEILProfileSubsystem::LoadFromDisk()
{
	CachedDisplayName.Reset();

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		if (UEMBERVEILLocalProfileSave* Loaded =
		        Cast<UEMBERVEILLocalProfileSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex)))
		{
			CachedDisplayName = Loaded->DisplayName;
		}
	}

	CachedDisplayName = SanitizeDisplayName(CachedDisplayName);
}

void UEMBERVEILProfileSubsystem::SaveToDisk()
{
	UEMBERVEILLocalProfileSave* Save = Cast<UEMBERVEILLocalProfileSave>(UGameplayStatics::CreateSaveGameObject(UEMBERVEILLocalProfileSave::StaticClass()));
	if (!Save)
	{
		return;
	}

	Save->DisplayName = CachedDisplayName;
	UGameplayStatics::SaveGameToSlot(Save, SaveSlotName, SaveUserIndex);
}

FString UEMBERVEILProfileSubsystem::SanitizeDisplayName(const FString& Raw) const
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
