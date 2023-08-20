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
	PlaybackSpeed = 1.4f;
	bIsLoopFrame = false;
	FrameIndexCenter = 1;
	bIsNotAnimated = true;
}

void UFrame::ChangeTexture(bool ToUp)
{
	if (ToUp)
	{
		ImageFrame->SetBrushFromTexture(TextureFrameTop);
	}
	else
	{
		ImageFrame->SetBrushFromTexture(TextureFrameCenter);
	}
}

void UFrame::SettingParameters(float AnimationSpeed, int32 MaxFrame)
{
	MaxFrameLimit = MaxFrame;
	PlaybackSpeed = AnimationSpeed;
	bIsLoopFrame = (MaxFrame > 4);
}

void UFrame::DirectionRight()
{
	OnDirectionRight();
}

void UFrame::DirectionLeft()
{
	OnDirectionLeft();
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
	}
}

void UFrame::AnimationFrameToTop(UWidgetAnimation* Animation1, UWidgetAnimation* Animation2, UWidgetAnimation* Animation3, UWidgetAnimation* Animation4, bool Reverse)
{
	if (Reverse)
	{
		switch (FrameIndexCenter)
		{
		case 1: UUserWidget::PlayAnimationReverse(Animation1, PlaybackSpeed); break;
		case 2: UUserWidget::PlayAnimationReverse(Animation2, PlaybackSpeed); break;
		case 3: UUserWidget::PlayAnimationReverse(Animation3, PlaybackSpeed); break;
		case 4: UUserWidget::PlayAnimationReverse(Animation4, PlaybackSpeed); break;
		}
	}
	else
	{
		switch (FrameIndexCenter)
		{
		case 1: UUserWidget::PlayAnimationForward(Animation1, PlaybackSpeed); break;
		case 2:	UUserWidget::PlayAnimationForward(Animation2, PlaybackSpeed); break;
		case 3: UUserWidget::PlayAnimationForward(Animation3, PlaybackSpeed); break;
		case 4: UUserWidget::PlayAnimationForward(Animation4, PlaybackSpeed); break;
		}
	}
}


