// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicSystemListInterface.h"
#include "Components/Scrollbox.h"
#include "ClassicButtonSystem.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UClassicSystemListInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ScrollBoxSystems->OnUserScrolled.AddDynamic(this, &UClassicSystemListInterface::OnUserScrolled);
	SetIconArrow();
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
	if (Navigate != EButtonsGame::DOWN && Navigate != EButtonsGame::UP) return;
	SetIconArrow();
}

void UClassicSystemListInterface::OnUserScrolled(float CurrentOffset)
{
	if (CurrentOffset >= ScrollBoxSystems->GetScrollOffsetOfEnd() - 2)
	{
		ScrollBoxSystems->SetScrollOffset(ScrollBoxSystems->GetScrollOffsetOfEnd() - 1);
	}
}

void UClassicSystemListInterface::SetIconArrow()
{
	ScrollCurrentOffSet = ScrollBoxSystems->GetScrollOffset();

	if (ScrollBoxSystems->GetAllChildren().Num() <= 5)
	{
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		return;
	}

	if (ScrollCurrentOffSet >= ScrollBoxSystems->GetScrollOffsetOfEnd() - 2)
	{
		//set ArrowDown
		ArrowUP->SetBrushFromTexture(ArrowIcon);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		return;
	}
	if (ScrollCurrentOffSet <= 1)
	{
		//set ArrowUP
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIcon);
		return;
	}

	ArrowUP->SetBrushFromTexture(ArrowIcon);
	ArrowDown->SetBrushFromTexture(ArrowIcon);

}

void UClassicSystemListInterface::SetFocusItem(const EButtonsGame Navigate, int32& Index, TArray<UClassicButtonSystem*> ButtonSystemReferences)
{
	const int32 NumChildren = ScrollBoxSystems->GetChildrenCount() - 1;
	if (Navigate == EButtonsGame::UP)
	{
		Index--;
		if (Index < 0)
		{
			Index = NumChildren;
		}
	}
	else if (Navigate == EButtonsGame::DOWN)
	{
		Index++;
		if (Index > NumChildren)
		{
			Index = 0;
			ScrollBoxSystems->SetScrollOffset(0);
		}
	}

	if (ButtonSystemReferences.IsValidIndex(Index))
	{
		ButtonSystemReferences[Index]->Click->SetKeyboardFocus();
		ScrollBoxSystems->ScrollWidgetIntoView(ButtonSystemReferences[Index], false, EDescendantScrollDestination::Center, 0);
	}
	IndexFocus = Index;
	GetWorld()->GetTimerManager().SetTimer(ArrowTimerHandle, this, &UClassicSystemListInterface::SetIconArrow, 0.016f, false, 0.1f);
}
