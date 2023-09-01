// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ToolTip.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


UToolTip::UToolTip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UToolTip::NativePreConstruct()
{
	Super::NativePreConstruct();
	TextBlock->SetText(Text);
}

void UToolTip::NativeConstruct()
{
	Super::NativeConstruct();
}

void UToolTip::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BgImage->SetVisibility(ESlateVisibility::Hidden);
	TextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UToolTip::SetToolTipVisibility(ESlateVisibility Visible)
{
	if (Visible == ESlateVisibility::Visible) 
	{
		PlayAnimationForward(AnimationSetToVisibility);
	}
	else 
	{
		PlayAnimationForward(AnimationSetToInvisibility);
	}
}
