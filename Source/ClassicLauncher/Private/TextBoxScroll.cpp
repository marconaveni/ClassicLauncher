// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "TextBoxScroll.h"
#include "Components/Scrollbox.h"
#include "Components/MultiLineEditableTextBox.h"

UTextBoxScroll::UTextBoxScroll(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UTextBoxScroll::NativePreConstruct()
{
	Super::NativePreConstruct();
	bAutoScroll = true;
	DelayWaitRestart = 2.5f;
	ScrollOffset = 0;
}

void UTextBoxScroll::StartScroll(float DelayStart)
{
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UTextBoxScroll::ScrollTick, 0.016f, true, DelayStart + DelayWaitRestart);
}

void UTextBoxScroll::ScrollTick()
{

	if (!bAutoScroll)
	{
		CancelAutoScroll();
	}

	const float GetScrollOffset = Scroll->GetScrollOffset();
	const float GetScrollOffsetOfEnd = Scroll->GetScrollOffsetOfEnd();

	if (GetScrollOffset <= GetScrollOffsetOfEnd)
	{
		ScrollOffset += SpeedScroll;
		Scroll->SetScrollOffset(FMath::Lerp(0, GetScrollOffsetOfEnd, ScrollOffset));
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle, this, &UTextBoxScroll::RestartScroll, 0.016f, false, DelayWaitRestart);
	}

}

void UTextBoxScroll::RestartScroll()
{
	Scroll->SetScrollOffset(0);
	ScrollOffset = 0;
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UTextBoxScroll::ScrollTick, 0.016f, false, DelayWaitRestart);
}

void UTextBoxScroll::CancelAutoScroll()
{
	bAutoScroll = true;
	ScrollOffset = 0;
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(RestartTimerHandle);
}

void UTextBoxScroll::SetText(FText NewText)
{
	Description->SetText(NewText);
}

void UTextBoxScroll::SetTextString(FString NewText)
{
	Description->SetText(FText::FromString(NewText));
}
