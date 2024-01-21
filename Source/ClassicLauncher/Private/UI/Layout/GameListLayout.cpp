// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/GameListLayout.h"
#include "UI/Components/ScrollBoxEnhanced.h"

void UGameListLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply UGameListLayout::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UGameListLayout::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UGameListLayout::SetFocusItem(const EButtonsGame Input, int32& Index)
{
	
	if (Input == EButtonsGame::UP)
	{
		/*Index =*/ ScrollBox->SetFocusScroll(EScrollTo::UP);
	}
	else if (Input == EButtonsGame::DOWN)
	{
		/*Index =*/ ScrollBox->SetFocusScroll(EScrollTo::DOWN);
	}
	else
	{
		/*Index =*/ ScrollBox->SetFocusScroll(EScrollTo::NONE);
	}
}
