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

void UFrame::SetFramePosition(int32 PositionCenter, EFocusTop FocusTop)
{
	if (FocusTop == EFocusTop::NONE)
	{
		ChangeTexture(false);
		ImageFrame->SetRenderScale(FVector2D(1.0f, 1.0f));
		switch (PositionCenter)
		{
		case 1:
			ImageFrame->SetRenderTranslation(FVector2D(0, 0));
			break;
		case 2:
			ImageFrame->SetRenderTranslation(FVector2D(385, 0));
			break;
		case 3:
			ImageFrame->SetRenderTranslation(FVector2D(770, 0));
			break;
		case 4:
			ImageFrame->SetRenderTranslation(FVector2D(1155, 0));
			break;
		}
		return;
	}

	ChangeTexture(true);
	ImageFrame->SetRenderScale(FVector2D(0.352f, 0.288f));
	switch (FocusTop)
	{
	case EFocusTop::SYSTEM:
		ImageFrame->SetRenderTranslation(FVector2D(379.0f, -442.0f));
		break;
	case EFocusTop::CONFIG:
		ImageFrame->SetRenderTranslation(FVector2D(522.599976f, -442.0f));
		break;
	case EFocusTop::FAVORITE:
		ImageFrame->SetRenderTranslation(FVector2D(662.0f, -442.0f));
		break;
	case EFocusTop::INFO:
		ImageFrame->SetRenderTranslation(FVector2D(798.0f, -442.0f));
		break;
	}
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
	}
}

void UFrame::AnimationFrameToTop(UWidgetAnimation* Animation1, UWidgetAnimation* Animation2, UWidgetAnimation* Animation3, UWidgetAnimation* Animation4, bool Reverse)
{
	if (Reverse)
	{
		switch (FrameIndexCenter)
		{
		case 1:
			UUserWidget::PlayAnimationReverse(Animation1, PlaybackSpeed);
			break;
		case 2:
			UUserWidget::PlayAnimationReverse(Animation2, PlaybackSpeed);
			break;
		case 3:
			UUserWidget::PlayAnimationReverse(Animation3, PlaybackSpeed);
			break;
		case 4:
			UUserWidget::PlayAnimationReverse(Animation4, PlaybackSpeed);
			break;
		}
	}
	else
	{
		switch (FrameIndexCenter)
		{
		case 1:
			UUserWidget::PlayAnimationForward(Animation1, PlaybackSpeed);
			break;
		case 2:
			UUserWidget::PlayAnimationForward(Animation2, PlaybackSpeed);
			break;
		case 3:
			UUserWidget::PlayAnimationForward(Animation3, PlaybackSpeed);
			break;
		case 4:
			UUserWidget::PlayAnimationForward(Animation4, PlaybackSpeed);
			break;
		}
	}
}


