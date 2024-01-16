// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "ClassicSlide.h"

#include "ClassicFunctionLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/Slider.h"


void UClassicSlide::MoveSlide()
{
	if (IsFocusable() && (Input == EButtonsGame::LEFT || Input == EButtonsGame::RIGHT))
	{
		const float SlideValue = GetSlideValue();
		const float SlideTo = 60.0f * GetWorld()->GetDeltaSeconds();
		if (Input == EButtonsGame::LEFT)
		{
			SetSlideValue(SlideValue - SlideTo);
		}
		else if (Input == EButtonsGame::RIGHT)
		{
			SetSlideValue(SlideValue + SlideTo);
		}
		UE_LOG(LogTemp, Warning, TEXT("slide %f"), SlideValue + SlideTo);
	}
}

void UClassicSlide::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	MoveSlide();
}

bool UClassicSlide::Initialize()
{
	const bool Success = Super::Initialize();
	if (SliderVolume)
	{
		SliderVolume->OnValueChanged.AddDynamic(this, &UClassicSlide::OnSlideValue);
	}

	return Success;
}

void UClassicSlide::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UClassicSlide::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	OnFocusLostTriggerSlide.Broadcast();
	Super::NativeOnFocusLost(InFocusEvent);
}

void UClassicSlide::OnSlideValue(float Value)
{
	OnSlideTrigger.Broadcast(FMath::Clamp(Value, 0, 100));
}

void UClassicSlide::SetSlideValue(float Value)
{
	SliderVolume->SetValue(FMath::Clamp(Value, 0, 100));
	OnSlideTrigger.Broadcast(FMath::Clamp(Value, 0, 100));
}

float UClassicSlide::GetSlideValue()
{
	return SliderVolume->GetValue();
}

void UClassicSlide::SetSlidePosition(float MarginLeft)
{
	UOverlaySlot* HorizontalBoxSlot = Cast<UOverlaySlot>(HorizontalBox->Slot);
	const UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(BackgroundFocus->Slot);
	HorizontalBoxSlot->SetPadding(FMargin(OverlaySlot->GetPadding()));
	UHorizontalBoxSlot* TextHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(Text->Slot);
	const float NewPaddingLeft = MarginLeft - OverlaySlot->GetPadding().Left;
	TextHorizontalBoxSlot->SetPadding(FMargin(NewPaddingLeft, 0, 0, 0));
	/*if (TextHorizontalBoxSlot != nullptr)
	{*/
	/*}*/
}

void UClassicSlide::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundSelect = NavigateSound;
}
