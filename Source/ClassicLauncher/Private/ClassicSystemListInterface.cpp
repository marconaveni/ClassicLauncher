// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicSystemListInterface.h"
#include "Components/Scrollbox.h"
#include "ClassicButtonSystem.h"
#include "Components/ScrollBox.h"

void UClassicSystemListInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ScrollBoxSystems->OnUserScrolled.AddDynamic(this, &UClassicSystemListInterface::OnUserScrolled);
}

FReply UClassicSystemListInterface::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
	SetScrollArrowIcons(Input);
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UClassicSystemListInterface::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	SetScrollArrowIcons(Input);
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UClassicSystemListInterface::SetScrollArrowIcons(const EButtonsGame Navigate)
{

	if (Navigate == EButtonsGame::DOWN)
	{
		if (ScrollCurrentOffSet >= ScrollBoxSystems->GetScrollOffsetOfEnd() - 11.0f)
		{
			//set ArrowDown
			UE_LOG(LogTemp, Warning, TEXT("ArrowDown"));
		}
	}
	else if (Navigate == EButtonsGame::UP)
	{
		if (ScrollCurrentOffSet == 0)
		{
			//set ArrowUP
			UE_LOG(LogTemp, Warning, TEXT("ArrowUP"));
		}
	}

}

void UClassicSystemListInterface::OnUserScrolled(float CurrentOffset)
{

	if (Input == EButtonsGame::DOWN && CurrentOffset < ScrollCurrentOffSet + 11)
	{
		ScrollBoxSystems->SetScrollOffset(ScrollCurrentOffSet);
		return;
	}
	ScrollCurrentOffSet = CurrentOffset;
	ScrollBoxSystems->SetScrollOffset(ScrollCurrentOffSet);
	UE_LOG(LogTemp, Warning, TEXT("ScrollCurrentOffSet value: %f ") , ScrollCurrentOffSet);
}
