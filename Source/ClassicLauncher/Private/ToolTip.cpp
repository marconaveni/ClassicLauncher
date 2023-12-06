// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ToolTip.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Components/Overlay.h"


UToolTip::UToolTip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UToolTip::NativePreConstruct()
{
	TextBlock->SetText(Text);
	Super::NativePreConstruct();
}

void UToolTip::NativeConstruct()
{
	Super::NativeConstruct();
}

void UToolTip::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UToolTip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bFocus)
	{
		if (!bEnableVisibility)
		{
			PlayAnimationForward(FadeInFadeOutAnimation);
		}
		bEnableVisibility = true;
	}
	else
	{
		if (bEnableVisibility && Overlay->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayAnimationReverse(FadeInFadeOutAnimation);
		}
		bEnableVisibility = false;
	}

}

void UToolTip::SetToolTipVisibility(ESlateVisibility Visible)
{
	bFocus = (Visible == ESlateVisibility::Visible);
}

void UToolTip::SetTextAppearance(FTextStyle NewTextStyle)
{
	TextBlock->SetTextStyle(NewTextStyle);
}

void UToolTip::AlternateToTextImage(bool bEnable, float Size)
{
	TextBlock->SetTextImageSize(Size);
	TextBlock->DefaultToImageText(bEnable, true);
}
