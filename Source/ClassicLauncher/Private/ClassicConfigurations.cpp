// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicConfigurations.h"
#include "MainInterface.h"
#include "EngineUtils.h"
#include "ClassicButton.h"


void UClassicConfigurations::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (TObjectIterator<UMainInterface> ObjectIterator ; ObjectIterator; ++ObjectIterator)
	{
		MainInterfaceReference = *ObjectIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference UMainInterface Founds: %s "), *MainInterfaceReference->GetName());
	}
}

FReply UClassicConfigurations::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);

	if (Input == EButtonsGame::B)
	{
		UUserWidget::PlayAnimationReverse(ShowConfigurations);
		MainInterfaceReference->BtnConfigurations->SetFocusButton(true);
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UClassicConfigurations::ShowConfiguration()
{
	UUserWidget::PlayAnimationForward(ShowConfigurations);
}
