// Copyright 2023 Marco Naveni. All Rights Reserved.


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
		if (BtButton->HasKeyboardFocus()) 
		{
			if (!hover) 
			{
				SetFocusButton(true);			
			}
			hover = true;
		}
		else 
		{
			if (hover) 
			{
				SetFocusButton(false);			
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

void UClassicButton::SetFocusButton(bool Focus)
{
	if(Focus)
	{
		//OnFocusTrigger.Broadcast();
		BtButton->SetKeyboardFocus();
		BgImage->SetVisibility(ESlateVisibility::Visible);	
		UGameplayStatics::PlaySound2D(this, SoundNavigation);
		PlayAnimationForward(FocusButton);
	}
	else
	{
		BgImage->SetVisibility(ESlateVisibility::Hidden);
		PlayAnimationReverse(FocusButton);
		//OnFocusLostTrigger.Broadcast();
	}
}

void UClassicButton::ButtonClick()
{
	OnClickTrigger.Broadcast();
}

void UClassicButton::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundNavigation = NavigateSound;
}
