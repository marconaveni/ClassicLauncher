// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "MessageBalloon.h"
#include "TextImageBlock.h"
#include "Engine/World.h"


void UMessageBalloon::ShowMessage(FText Message, float InRate)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	PlayAnimationForward(AnimationShowMessage);
	TextMessage->SetText(Message);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMessageBalloon::EndMessage, InRate, false, -1);
}

void UMessageBalloon::EndMessage()
{
	PlayAnimationReverse(AnimationShowMessage);
}
