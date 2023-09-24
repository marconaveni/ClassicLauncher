// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicButtonSystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Arrow.h"
#include "Kismet/GameplayStatics.h"

UClassicButtonSystem::UClassicButtonSystem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CountSystem = 0;
	Hover = false;
}

void UClassicButtonSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Click != nullptr) {
		//equivale doonce blueprint
		if (Click->HasKeyboardFocus()) {
			if (!Hover) {
				BgBackground->SetVisibility(ESlateVisibility::Visible);
				WBPArrow->SetVisibility(ESlateVisibility::Visible);
				OnFocusTrigger.Broadcast();
				UGameplayStatics::PlaySound2D(this, SoundSelect);
			}
			Hover = true;
		}
		else {
			if (Hover) {
				BgBackground->SetVisibility(ESlateVisibility::Hidden);
				WBPArrow->SetVisibility(ESlateVisibility::Hidden);
				OnFocusLostTrigger.Broadcast();
			}
			Hover = false;
		}
	}
}

bool UClassicButtonSystem::Initialize()
{
	bool Success = Super::Initialize();
	if (Click)
	{
		Click->OnClicked.AddDynamic(this, &UClassicButtonSystem::ButtonClick);
	}
	
	return Success;
}

void UClassicButtonSystem::NativePreConstruct()
{
	Text->SetText(ButtonText);
	Super::NativePreConstruct();
}

void UClassicButtonSystem::SetText(FText NewText)
{
	Text->SetText(NewText);
	ButtonText = NewText;
}

void UClassicButtonSystem::SetCount(int32 NewValue)
{
	CountSystem = NewValue;
}

void UClassicButtonSystem::ButtonClick()
{
	OnClickTrigger.Broadcast(CountSystem);
	UGameplayStatics::PlaySound2D(this, SoundClick);
}

void UClassicButtonSystem::SetTextAppearance(FTextStyle NewTextStyle)
{
	Text->SetTextStyle(NewTextStyle);
}

void UClassicButtonSystem::AlternateToTextImage(bool bEnable, float Size)
{
	Text->SetTextImageSize(Size);
	Text->DefaultToImageText(bEnable, true);
}
