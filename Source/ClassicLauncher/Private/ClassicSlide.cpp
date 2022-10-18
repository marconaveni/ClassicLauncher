// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicSlide.h"
#include "Components/Slider.h"
#include "Arrow.h"

void UClassicSlide::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (SliderVol != nullptr) {
		//equivale doonce blueprint
		if (SliderVol->HasKeyboardFocus()) {
			if (!Hover) 
			{			
				WBPArrow->SetVisibility(ESlateVisibility::Visible);
			}
			Hover = true;
		}
		else {
			if (Hover)
			{
				WBPArrow->SetVisibility(ESlateVisibility::Hidden);
			}
			Hover = false;
		}
	}
}

void UClassicSlide::SetFocusSlide()
{
	SliderVol->SetKeyboardFocus();
}


