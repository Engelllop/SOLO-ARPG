#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "WG_MainMenu.generated.h"

/**
 * MenÃº mÃ­nimo en Slate (sin Blueprint de diseÃ±o): nombre local + Jugar + Salir.
 * Sustituir o envolver por UMG diseÃ±ado cuando exista arte de UI.
 */
UCLASS()
class SOLOCORE_API UWG_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	FReply HandleStartClicked();
	FReply HandleQuitClicked();

	TSharedPtr<SEditableTextBox> DisplayNameBox;
	TSharedPtr<SEditableTextBox> PasswordBox;
};
