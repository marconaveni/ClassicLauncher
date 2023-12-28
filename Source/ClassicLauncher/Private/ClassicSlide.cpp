// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicSlide.h"
#include "Components/Slider.h"
#include "Components/Image.h"
#include "Arrow.h"
#include "TextImageBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UClassicSlide::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (SliderVol != nullptr) {
		//equivale doonce blueprint
		if (Click->HasKeyboardFocus()) {
			if (!Hover)
			{
				BgBackground->SetVisibility(ESlateVisibility::Visible);
				WBPArrow->SetVisibility(ESlateVisibility::Visible);
				OnFocusTriggerSlide.Broadcast();
				UGameplayStatics::PlaySound2D(this, SoundNavigation);
			}
			Hover = true;
		}
		else {
			if (Hover)
			{
				BgBackground->SetVisibility(ESlateVisibility::Hidden);
				WBPArrow->SetVisibility(ESlateVisibility::Hidden);
				OnFocusLostTriggerSlide.Broadcast();
			}
			Hover = false;
		}
	}
}

bool UClassicSlide::Initialize()
{
	bool Success = Super::Initialize();
	if (SliderVol)
	{
		SliderVol->OnValueChanged.AddDynamic(this, &UClassicSlide::OnSlideValue);
	}

	return Success;
}

void UClassicSlide::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetTextLabel(TxtTextLabel);
}

void UClassicSlide::SetFocusSlide()
{
	Click->SetKeyboardFocus();
}

void UClassicSlide::OnSlideValue(float Value)
{
	OnSlide.Broadcast(FMath::Clamp(Value, 0, 100));
	SlideValue = FMath::Clamp(Value, 0, 100);
}

void UClassicSlide::SetSlideValue(float Value)
{
	OnSlide.Broadcast(FMath::Clamp(Value, 0, 100));
	SliderVol->SetValue(FMath::Clamp(Value, 0, 100));
	SlideValue = FMath::Clamp(Value, 0, 100);
}

void UClassicSlide::SetTextLabel(FText NewText)
{
	TxtLabel->SetText(NewText);
}

void UClassicSlide::SetTextAppearance(FTextStyle NewTextStyle)
{
	TxtLabel->SetTextStyle(NewTextStyle);
}

void UClassicSlide::AlternateToTextImage(bool bEnable, float Size)
{
	TxtLabel->SetTextImageSize(Size);
	TxtLabel->DefaultToImageText(bEnable, true);
}

void UClassicSlide::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundNavigation = NavigateSound;
}


