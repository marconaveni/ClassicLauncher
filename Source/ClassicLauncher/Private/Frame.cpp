// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "Frame.h"
#include "Components/Image.h"

UFrame::UFrame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, FrameIndexCenter(1)
{
}

void UFrame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UFrame::SetFrame(const int32& IndexFrame, const EPositionY& Position)
{
	OnSetFrame(IndexFrame, Position);
}

void UFrame::SetFrameIndexTop(const EButtonsGame Input, const int32 IndexLimit)
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

void UFrame::SetDefaultValues(int32 MaxFrameRightLimit, float MaxSpeed)
{
	FrameIndexCenter = 1;
	SetFrameCenterPosition(1);
}

void UFrame::SetFrameCenterPosition(const int32 PositionCenter)
{
	FrameIndexCenter = PositionCenter;

	ImageFrameCenter->SetRenderScale(FVector2D(1.0f, 1.0f));
	ImageFrameCenter->SetRenderOpacity(1.0f);
	ImageFrameCenter->SetVisibility(ESlateVisibility::Visible);
	ImageFrameTop->SetVisibility(ESlateVisibility::Hidden);

	switch (PositionCenter)
	{
	case 1:
		ImageFrameCenter->SetRenderTranslation(FVector2D(0, 0));
		break;
	case 2:
		ImageFrameCenter->SetRenderTranslation(FVector2D(385, 0));
		break;
	case 3:
		ImageFrameCenter->SetRenderTranslation(FVector2D(770, 0));
		break;
	case 4:
		ImageFrameCenter->SetRenderTranslation(FVector2D(1155, 0));
		break;
	default:
		break;
	}

}


