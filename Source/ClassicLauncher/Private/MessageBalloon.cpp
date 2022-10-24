// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "MessageBalloon.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/World.h"


void UMessageBalloon::ShowMessage(FString Message, float InRate)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	UUserWidget::PlayAnimationForward(AnimationShowMessage);
	TextMessage->SetText(FText::FromString(Message));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMessageBalloon::EndMessage, InRate, false, -1);
}

void UMessageBalloon::EndMessage()
{
	UUserWidget::PlayAnimationReverse(AnimationShowMessage);
}
