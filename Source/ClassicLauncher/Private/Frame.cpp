// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "Frame.h"
#include "Components/Image.h"

UFrame::UFrame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Clear();
}

void UFrame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UFrame::OnAnimationStartedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationStartedPlaying(Player);
}

void UFrame::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
}

void UFrame::Clear()
{
	MaxFrameLimit = 1;
	PlaybackSpeed = 1.0f;
	FrameIndexCenter = 1;
	bIsNotAnimated = true;
	bAtRight = false;
	bAtLeft = true;
	EndAtTime = 0.0f;
}

void UFrame::SetDefaultValues(int32 MaxFrameRightLimit, float MaxSpeed)
{
	MaxFrameLimit = FMath::Clamp(MaxFrameRightLimit, 1, 4);
	PlaybackSpeed = FMath::Clamp(MaxSpeed, 0.5f, 3.0f);
	FrameIndexCenter = 1;
	bAtRight = false;
	bAtLeft = true;
	EndAtTime = 0.0f;
	SetFramePosition(1, EFocusTop::NONE);
}


void UFrame::SetFramePosition(int32 PositionCenter, EFocusTop FocusTop)
{
	if (FocusTop == EFocusTop::NONE)
	{
		ImageFrameCenter->SetRenderScale(FVector2D(1.0f, 1.0f));
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
		return;
	}

	ImageFrameCenter->SetRenderScale(FVector2D(0.352f, 0.288f));
	ImageFrameCenter->SetVisibility(ESlateVisibility::Hidden);
	ImageFrameTop->SetVisibility(ESlateVisibility::Visible);

}

void UFrame::DirectionRight(int32 Frame, int32 Limit)
{
	OnDirectionRight(Frame, Limit);
}

void UFrame::DirectionLeft(int32 Frame, int32 Limit)
{
	OnDirectionLeft(Frame, Limit);
}

void UFrame::AnimationToTopDown(EFocusTop Focus, bool Forward)
{
	switch (Focus)
	{
	case EFocusTop::SYSTEM:
		AnimationFrameToTop(FrameAnimationY1ToSystems, FrameAnimationY2ToSystems, FrameAnimationY3ToSystems, FrameAnimationY4ToSystems, Forward);
		break;
	case EFocusTop::CONFIG:
		AnimationFrameToTop(FrameAnimationY1ToConfig, FrameAnimationY2ToConfig, FrameAnimationY3ToConfig, FrameAnimationY4ToConfig, Forward);
		break;
	case EFocusTop::FAVORITE:
		AnimationFrameToTop(FrameAnimationY1ToFavorite, FrameAnimationY2ToFavorite, FrameAnimationY3ToFavorite, FrameAnimationY4ToFavorite, Forward);
		break;
	case EFocusTop::INFO:
		AnimationFrameToTop(FrameAnimationY1ToInfo, FrameAnimationY2ToInfo, FrameAnimationY3ToInfo, FrameAnimationY4ToInfo, Forward);
		break;
	default: 
		break;
	}
}

void UFrame::AnimationFrameToTop(UWidgetAnimation* Animation1, UWidgetAnimation* Animation2, UWidgetAnimation* Animation3, UWidgetAnimation* Animation4, bool Reverse)
{
	if (Reverse)
	{
		switch (FrameIndexCenter)
		{
		case 1:
			PlayAnimationReverse(Animation1, PlaybackSpeed);
			break;
		case 2:
			PlayAnimationReverse(Animation2, PlaybackSpeed);
			break;
		case 3:
			PlayAnimationReverse(Animation3, PlaybackSpeed);
			break;
		case 4:
			PlayAnimationReverse(Animation4, PlaybackSpeed);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (FrameIndexCenter)
		{
		case 1:
			PlayAnimationForward(Animation1, PlaybackSpeed);
			break;
		case 2:
			PlayAnimationForward(Animation2, PlaybackSpeed);
			break;
		case 3:
			PlayAnimationForward(Animation3, PlaybackSpeed);
			break;
		case 4:
			PlayAnimationForward(Animation4, PlaybackSpeed);
			break;
		default:
			break;
		}
	}
}


