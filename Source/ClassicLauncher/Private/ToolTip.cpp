// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ToolTip.h"
#include "Components/Image.h"


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

void UToolTip::SetToolTipVisibility(ESlateVisibility Visible)
{
	if (Visible == ESlateVisibility::Visible) 
	{
		PlayAnimationForward(FadeInFadeOutAnimation);
	}
	else 
	{
		PlayAnimationReverse(FadeInFadeOutAnimation);
	}
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
