// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

UClassicButton::UClassicButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    hover = false;
}

void UClassicButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
	if (BtButton != nullptr) {
		//equivale doonce blueprint
		if (BtButton->HasKeyboardFocus()) {
			if (!hover) {
				BgImage->SetVisibility(ESlateVisibility::Visible);
				OnFocusTrigger.Broadcast();
				UGameplayStatics::PlaySound2D(this, SoundSelect);
				UUserWidget::PlayAnimationForward(FocusButton);
			}
			hover = true;
		}
		else {
			if (hover) {
				BgImage->SetVisibility(ESlateVisibility::Hidden);
				OnFocusLostTrigger.Broadcast();
				UUserWidget::PlayAnimationReverse(FocusButton);
			}
			hover = false;
		}
	}

}

bool UClassicButton::Initialize()
{
    bool Success = Super::Initialize();
	if (BtButton)
	{
		BtButton->OnClicked.AddDynamic(this, &UClassicButton::ButtonClick);
		
	}
    return false;
}

void UClassicButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	BtButton->SetStyle(StyleButton);
}

void UClassicButton::ButtonClick()
{
	OnClickTrigger.Broadcast();
}
