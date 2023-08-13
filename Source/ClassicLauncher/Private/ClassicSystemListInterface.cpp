// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicSystemListInterface.h"
#include "Components/Scrollbox.h"
#include "ClassicButtonSystem.h"
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
	if (Input == EButtonsGame::DOWN && CurrentOffset < ScrollCurrentOffSet + 11)
	{
		ScrollBoxSystems->SetScrollOffset(ScrollCurrentOffSet);
		return;
	}
	ScrollCurrentOffSet = CurrentOffset;
	ScrollBoxSystems->SetScrollOffset(ScrollCurrentOffSet);
	UE_LOG(LogTemp, Warning, TEXT("ScrollCurrentOffSet value: %f "), ScrollCurrentOffSet);
}

void UClassicSystemListInterface::SetIconArrow()
{
	if (ScrollBoxSystems->GetAllChildren().Num() <= 5)
	{
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		UE_LOG(LogTemp, Warning, TEXT("ScrollBoxSystems->GetAllChildren().Num() = %d"), ScrollBoxSystems->GetAllChildren().Num());
		return;
	}

	if (ScrollCurrentOffSet >= ScrollBoxSystems->GetScrollOffsetOfEnd() - 11.0f)
	{
		//set ArrowDown
		ArrowUP->SetBrushFromTexture(ArrowIcon);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		UE_LOG(LogTemp, Warning, TEXT("ArrowDown"));
		return;
	}
	if (ScrollCurrentOffSet == 0)
	{
		//set ArrowUP
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIcon);
		UE_LOG(LogTemp, Warning, TEXT("ArrowUP"));
		return;
	}

	ArrowUP->SetBrushFromTexture(ArrowIcon);
	ArrowDown->SetBrushFromTexture(ArrowIcon);

}
