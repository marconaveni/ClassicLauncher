// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "MessageBalloon.h"

#include "AnimationUI.h"
#include "AnimationUILoader.h"
#include "TextImageBlock.h"
#include "Components/CanvasPanel.h"
#include "Engine/World.h"


void UMessageBalloon::ShowMessage(const FText Message, const float Time)
{

	TextMessage->SetText(Message);
	InRate = Time;
	CanvasMessage->SetRenderTransform(FWidgetTransform());

	GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UMessageBalloon::StartMessage, GetWorld()->GetDeltaSeconds(), false, -1);
}

void UMessageBalloon::StartMessage()
{
	UAnimationUILoader* AnimationLoaderUISubSystem = GetWorld()->GetSubsystem<UAnimationUILoader>();

	CanvasMessage->SetRenderTransform(FWidgetTransform());
	FVector2D CanvasSize;
	CanvasSize.X = CanvasMessage->GetDesiredSize().X * -1;
	CanvasSize.Y = 0;
	const FAnimationUICurves Curves;
	const FWidgetTransform ToPosition(
		CanvasSize,
		FVector2D(1.0f),
		FVector2D(0),
		0.0f);
	AnimationLoaderUISubSystem->PlayAnimation(CanvasMessage, 0.5f, ToPosition, 1.0f, false, EEasingFunc::EaseOut, Curves, TEXT("MessageShow"));


	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMessageBalloon::EndMessage, InRate, false, -1);
}

void UMessageBalloon::EndMessage()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	UAnimationUILoader* AnimationLoaderUISubSystem = GetWorld()->GetSubsystem<UAnimationUILoader>();
	const FAnimationUICurves Curves;
	AnimationLoaderUISubSystem->PlayAnimation(CanvasMessage, 0.5f, FWidgetTransform(), 1.0f, false, EEasingFunc::EaseOut, Curves, TEXT("MessageShow"));

}
