// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "TextBoxScroll.h"
#include "Components/Scrollbox.h"
#include "Components/MultiLineEditableTextBox.h"

UTextBoxScroll::UTextBoxScroll(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutoScroll = true;
	DelayWaitRestart = 5.0f;
	ScrollOffset = 0;
	SpeedScroll = 0.00015f;
	TextColor = FLinearColor(1.0f, 1.0f, 1.0f);
	TextSize = 17;
}

void UTextBoxScroll::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetText(Text);
	SetTextAppearance(TextColor, TextSize);
}

void UTextBoxScroll::NativeConstruct()
{
	Super::NativeConstruct();
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

void UTextBoxScroll::SetText(FText NewText)
{
	Description->SetText(NewText);
}

void UTextBoxScroll::SetTextString(FString NewText)
{
	Description->SetText(FText::FromString(NewText));
}

void UTextBoxScroll::SetTextAppearance(FLinearColor NewTextColor, int32 NewTextSize)
{
	const uint16 Size = FMath::Clamp(NewTextSize, 0, 1000);
	FTextBlockStyle Style;
	Style.SetFont(Description->TextStyle.Font);
	Style.SetColorAndOpacity(FSlateColor(NewTextColor));
	Style.SetFontSize(Size);
	Description->SetTextStyle(Style);
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
