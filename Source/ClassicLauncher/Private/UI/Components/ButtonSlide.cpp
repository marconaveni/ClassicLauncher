// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/ButtonSlide.h"

#include "Components/CanvasPanelSlot.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/Slider.h"


void UButtonSlide::MoveSlide()
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

void UButtonSlide::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	MoveSlide();
}

bool UButtonSlide::Initialize()
{
	const bool Success = Super::Initialize();
	if (SliderVolume)
	{
		SliderVolume->OnValueChanged.AddDynamic(this, &UButtonSlide::OnSlideValue);
	}

	return Success;
}

void UButtonSlide::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UButtonSlide::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	OnFocusLostTriggerSlide.Broadcast();
	Super::NativeOnFocusLost(InFocusEvent);
}

void UButtonSlide::NativeSetBackgroundAppearance(float CornerRadius, FString Color, float BorderWidth, FString BorderColor, float MarginLeft, float MarginTop, float MarginRight, float MarginBottom)
{
	Super::NativeSetBackgroundAppearance(CornerRadius, Color, BorderWidth, BorderColor, MarginLeft, MarginTop, MarginRight, MarginBottom);
	const UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(BackgroundFocus->Slot);
	UOverlaySlot* HorizontalBoxSlot = Cast<UOverlaySlot>(HorizontalBox->Slot);
	HorizontalBoxSlot->SetPadding(FMargin(OverlaySlot->GetPadding()));
}

void UButtonSlide::NativeSetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin, float Size)
{
	Super::NativeSetTextAppearance(NewTextStyle, bEnableImageText, Margin, Size);
	const UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(BackgroundFocus->Slot);
	UHorizontalBoxSlot* TextHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(Text->Slot);
	const float NewPaddingLeft = Margin - OverlaySlot->GetPadding().Left;
	TextHorizontalBoxSlot->SetPadding(FMargin(NewPaddingLeft, 0, 0, 0));
}

void UButtonSlide::OnSlideValue(float Value)
{
	OnSlideTrigger.Broadcast(FMath::Clamp(Value, 0, 100));
}

void UButtonSlide::SetSlideValue(float Value)
{
	SliderVolume->SetValue(FMath::Clamp(Value, 0, 100));
	OnSlideTrigger.Broadcast(FMath::Clamp(Value, 0, 100));
}

float UButtonSlide::GetSlideValue()
{
	return SliderVolume->GetValue();
}

void UButtonSlide::SetSlidePosition(FVector2D Size)
{
	UCanvasPanelSlot* CanvasSlideBoxSlot = Cast<UCanvasPanelSlot>(SliderVolume->Slot);
	CanvasSlideBoxSlot->SetSize(Size);
}

void UButtonSlide::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundSelect = NavigateSound;
}
