// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "ClassicSystemListInterface.h"
#include "UI/Layout/Components/ScrollBoxEnhanced.h"

void UClassicSystemListInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply UClassicSystemListInterface::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UClassicSystemListInterface::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UClassicSystemListInterface::SetFocusItem(const EButtonsGame Input, int32& Index)
{
	
	if (Input == EButtonsGame::UP)
	{
		Index = ScrollBox->SetFocusScroll(EScrollTo::UP);
	}
	else if (Input == EButtonsGame::DOWN)
	{
		Index = ScrollBox->SetFocusScroll(EScrollTo::DOWN);
	}
	else
	{
		Index = ScrollBox->SetFocusScroll(EScrollTo::NONE);
	}
}
