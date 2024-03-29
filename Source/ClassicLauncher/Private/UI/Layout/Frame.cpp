// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/Frame.h"

#include "UI/Layout/LoopScroll.h"
#include "UI/Screens/MainScreen.h"
#include "Components/Image.h"
#include "Data/DataManager.h"

UFrame::UFrame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , FrameIndexCenter(1)
	  , Position(EPositionY::CENTER)
{
}

void UFrame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DataManager = GetWorld()->GetSubsystem<UDataManager>();
}


void UFrame::SetIndexTop(const EButtonsGame Input, const int32 IndexLimit)
{

	const int32 Max = (IndexLimit != 0) ? 4 : 2;

	if (Input == EButtonsGame::LEFT)
	{
		if (FrameIndexTop - 1 < 1)
		{
			FrameIndexTop = Max + 1;
		}
		FrameIndexTop = FMath::Clamp(FrameIndexTop - 1, 1, Max);
	}
	else if (Input == EButtonsGame::RIGHT)
	{
		if (FrameIndexTop + 1 > Max)
		{
			FrameIndexTop = 0;
		}
		FrameIndexTop = FMath::Clamp(FrameIndexTop + 1, 1, Max);
	}

}

void UFrame::SetFramePositionWithoutAnimation(const int32 PositionCenter)
{
	FrameIndexCenter = PositionCenter;

	ImageFrameCenter->SetRenderScale(FVector2D(1.0f, 1.0f));
	ImageFrameCenter->SetRenderOpacity(1.0f);
	ImageFrameCenter->SetVisibility(ESlateVisibility::Visible);
	ImageFrameTop->SetVisibility(ESlateVisibility::Hidden);


	if(DataManager->GetLoopScrollReference() != nullptr)
	{
		const int32 PositionX = DataManager->GetLoopScrollReference()->GetScrollConfiguration().CardSize * (PositionCenter - 1);
		ImageFrameCenter->SetRenderTranslation(FVector2D(PositionX, 0));
	}
}

void UFrame::SetFramePositionWithAnimation(const EPositionY& NewPosition)
{
	Position = NewPosition;
	OnSetFrame();
}

