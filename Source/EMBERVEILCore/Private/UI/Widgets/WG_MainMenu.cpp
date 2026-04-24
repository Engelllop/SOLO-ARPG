#include "UI/Widgets/WG_MainMenu.h"
#include "Profile/EMBERVEILProfileSubsystem.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "EMBERVEILMainMenu"

TSharedRef<SWidget> UWG_MainMenu::RebuildWidget()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
			  .AutoHeight()
			  .Padding(8.f)
		[SAssignNew(DisplayNameBox, SEditableTextBox)
				.HintText(LOCTEXT("DisplayNameHint", "Nombre visible (máx. 24)"))
				.MaximumLength(24)]
		+ SVerticalBox::Slot()
			  .AutoHeight()
			  .Padding(8.f)
		[SAssignNew(PasswordBox, SEditableTextBox)
				.HintText(LOCTEXT("AdminPasswordHint", "Contraseña (solo Admin0..10, opcional)"))
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
		if (UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance()))
		{
			if (UEMBERVEILProfileSubsystem* Profile = GI->GetSubsystem<UEMBERVEILProfileSubsystem>())
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

	UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance());
	if (!GI)
	{
		return FReply::Handled();
	}

	UEMBERVEILProfileSubsystem* Profile = GI->GetSubsystem<UEMBERVEILProfileSubsystem>();
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
