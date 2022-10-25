// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ToolTip.h"
#include "Animation/WidgetAnimation.h"
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
	UUserWidget::PlayAnimationForward(AnimationSetToInvisibility);
}

void UToolTip::SetToolTipVisibility(ESlateVisibility Visible)
{
	if (Visible == ESlateVisibility::Visible) {
		UUserWidget::PlayAnimationForward(AnimationSetToVisibility);
	}
	else {
		UUserWidget::PlayAnimationForward(AnimationSetToInvisibility);
	}
}
