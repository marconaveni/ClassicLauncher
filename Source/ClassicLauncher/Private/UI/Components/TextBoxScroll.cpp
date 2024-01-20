// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/TextBoxScroll.h"

#include "TextImageBlock.h"
#include "TimerManager.h"
#include "Components/Scrollbox.h"

UTextBoxScroll::UTextBoxScroll(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutoScroll = true;
	DelayWaitRestart = 5.0f;
	DelayTextAppear = 0.35f;
	ScrollOffset = 0;
	SpeedScroll = 0.00015f;
}

void UTextBoxScroll::NativePreConstruct()
{
	Super::NativePreConstruct();
	DescriptionEnchanted->SetText(FText::FromString(Text));
	AlternateToTextImage(false, 17);
	TextStyle.bWrap = true;
	SetTextAppearance(TextStyle);
}

void UTextBoxScroll::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTextBoxScroll::StartScroll(float DelayStart)
{
	DescriptionEnchanted->UpdateRender();
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
		const float Min = FMath::Clamp(GetScrollOffsetOfEnd, 2000, GetScrollOffsetOfEnd + 2000);
		ScrollOffset += SpeedScroll;
		Scroll->SetScrollOffset(FMath::Lerp(0, Min, ScrollOffset));
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
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UTextBoxScroll::ScrollTick, 0.016f, true, DelayWaitRestart);
}

void UTextBoxScroll::CancelAutoScroll()
{
	Scroll->SetScrollOffset(0);
	bAutoScroll = true;
	ScrollOffset = 0;
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(RestartTimerHandle);
}

void UTextBoxScroll::PauseAutoScroll()
{
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(RestartTimerHandle);
}

void UTextBoxScroll::SetText(FString NewText)
{
	Text = NewText;
	GetWorld()->GetTimerManager().ClearTimer(DelaySetText);
	GetWorld()->GetTimerManager().SetTimer(DelaySetText, this, &UTextBoxScroll::RenderText, 0.016f, false, DelayTextAppear);
}

void UTextBoxScroll::RenderText()
{
	DescriptionEnchanted->SetText(FText::FromString(Text));
}

void UTextBoxScroll::SetTextAppearance(FTextStyle NewTextStyle)
{
	DescriptionEnchanted->SetTextStyle(NewTextStyle);
}

void UTextBoxScroll::AlternateToTextImage(bool bEnable, float Size)
{
	DescriptionEnchanted->SetTextImageSize(Size);
	DescriptionEnchanted->DefaultToImageText(bEnable,true);
}

void UTextBoxScroll::SetNewScroll(EButtonsGame Input, float NewScroll)
{
	if (Input == EButtonsGame::SCROLLUP || Input == EButtonsGame::SCROLLDOWN)
	{
		PauseAutoScroll();
		StartScroll(5.0f);

		const float GetScrollOffset = Scroll->GetScrollOffset();
		const float GetScrollOffsetOfEnd = Scroll->GetScrollOffsetOfEnd();
		const float Min = FMath::Clamp(GetScrollOffsetOfEnd, 2000, GetScrollOffsetOfEnd + 2000);

		if (Input == EButtonsGame::SCROLLUP && GetScrollOffset > 0)
		{
			ScrollOffset -= NewScroll;
			Scroll->SetScrollOffset(FMath::Lerp(0, Min, ScrollOffset));
		}
		else if (Input == EButtonsGame::SCROLLDOWN && GetScrollOffset <= GetScrollOffsetOfEnd)
		{
			ScrollOffset += NewScroll;
			Scroll->SetScrollOffset(FMath::Lerp(0, Min, ScrollOffset));
		}
	}
}
