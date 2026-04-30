#include "UI/Widgets/WG_MainMenu.h"
#include "Profile/SOLOProfileSubsystem.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "SOLOMainMenu"

TSharedRef<SWidget> UWG_MainMenu::RebuildWidget()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
			  .AutoHeight()
			  .Padding(8.f)
		[SAssignNew(DisplayNameBox, SEditableTextBox)
				.HintText(LOCTEXT("DisplayNameHint", "Nombre visible (mÃ¡x. 24)"))
				.MaximumLength(24)]
		+ SVerticalBox::Slot()
			  .AutoHeight()
			  .Padding(8.f)
		[SAssignNew(PasswordBox, SEditableTextBox)
				.HintText(LOCTEXT("AdminPasswordHint", "ContraseÃ±a (solo Admin0..10, opcional)"))
				.IsPassword(true)
				.MaximumLength(32)]
		+ SVerticalBox::Slot()
			  .AutoHeight()
			  .Padding(8.f)
		[SNew(SButton)
				.Text(LOCTEXT("StartGame", "Jugar"))
				.OnClicked(FOnClicked::CreateUObject(this, &UWG_MainMenu::HandleStartClicked))]
		+ SVerticalBox::Slot()
			  .AutoHeight()
			  .Padding(8.f)
		[SNew(SButton)
				.Text(LOCTEXT("QuitGame", "Salir"))
				.OnClicked(FOnClicked::CreateUObject(this, &UWG_MainMenu::HandleQuitClicked))];
}

void UWG_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (DisplayNameBox.IsValid())
	{
		if (USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance()))
		{
			if (USOLOProfileSubsystem* Profile = GI->GetSubsystem<USOLOProfileSubsystem>())
			{
				DisplayNameBox->SetText(FText::FromString(Profile->GetDisplayName()));
			}
		}

		FSlateApplication::Get().SetKeyboardFocus(DisplayNameBox);
	}
}

FReply UWG_MainMenu::HandleStartClicked()
{
	FString Raw;
	if (DisplayNameBox.IsValid())
	{
		Raw = DisplayNameBox->GetText().ToString();
	}

	FString Password;
	if (PasswordBox.IsValid())
	{
		Password = PasswordBox->GetText().ToString();
	}

	USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance());
	if (!GI)
	{
		return FReply::Handled();
	}

	USOLOProfileSubsystem* Profile = GI->GetSubsystem<USOLOProfileSubsystem>();
	if (!Profile)
	{
		return FReply::Handled();
	}

	FString Error;
	if (!Profile->TryCompleteMainMenuLogin(Raw, Password, Error))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage((uint64)(uintptr_t)this, 4.f, FColor::Red, Error);
		}
		return FReply::Handled();
	}

	GI->TravelToGameplayMap();
	return FReply::Handled();
}

FReply UWG_MainMenu::HandleQuitClicked()
{
	APlayerController* PC = GetOwningPlayer();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
