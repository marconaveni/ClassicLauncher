// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/GameListLayout.h"

#include "UI/Components/ButtonCommon.h"
#include "UI/Components/ScrollBoxEnhanced.h"
#include "UI/Screens/MainScreen.h"

#define LOCTEXT_NAMESPACE "ButtonsSelection"

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
void UGameListLayout::AddSystems(TArray<FGameSystem> GameSystem, UMainScreen* MainScreenRef)
{
	ScrollBox->ClearAllChildrenContent();

	if (ButtonCommonClass != nullptr)
	{
		for (int32 i = 0; i < GameSystem.Num(); i++)
		{
			UButtonCommon* ButtonSystem = CreateWidget<UButtonCommon>(GetOwningPlayer(), ButtonCommonClass);
			ButtonSystem->OnClickTrigger.AddDynamic(MainScreenRef, &UMainScreen::OnClickSystem);
			ButtonSystem->SetText((i == 0) ? LOCTEXT("Systems", "Show Systems") : FText::FromString(GameSystem[i].SystemLabel));
			ButtonSystem->SetIndex(i);
			ScrollBox->SetContent(ButtonSystem);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("buttonSystemClass Not Found"));
	}
}

void UGameListLayout::SetFocusItem(const EButtonsGame Input)
{
	
	if (Input == EButtonsGame::UP)
	{
		ScrollBox->SetFocusScroll(EScrollTo::UP);
	}
	else if (Input == EButtonsGame::DOWN)
	{
		ScrollBox->SetFocusScroll(EScrollTo::DOWN);
	}
	else
	{
		ScrollBox->SetFocusScroll(EScrollTo::NONE);
	}
}


#undef LOCTEXT_NAMESPACE