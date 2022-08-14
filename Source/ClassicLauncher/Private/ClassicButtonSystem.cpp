// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicButtonSystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Arrow.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

UClassicButtonSystem::UClassicButtonSystem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CountSystem = 0;
	hover = false;
}

void UClassicButtonSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Click != nullptr) {
		//equivale doonce blueprint
		if (Click->HasKeyboardFocus()) {
			if (!hover) {
				BgBackground->SetVisibility(ESlateVisibility::Visible);
				WBPArrow->SetVisibility(ESlateVisibility::Visible);
				OnFocusTrigger.Broadcast();
				UGameplayStatics::PlaySound2D(this, SoundSelect);
			}
			hover = true;
		}
		else {
			if (hover) {
				BgBackground->SetVisibility(ESlateVisibility::Hidden);
				WBPArrow->SetVisibility(ESlateVisibility::Hidden);
				OnFocusLostTrigger.Broadcast();
			}
			hover = false;
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

	return false;
}

void UClassicButtonSystem::SetText(FString NewText, int32 CountSystem_)
{
	Text->SetText(FText::FromString(NewText));
	CountSystem = CountSystem_;
}

void UClassicButtonSystem::ButtonClick()
{
	OnClickTrigger.Broadcast(CountSystem);
	UGameplayStatics::PlaySound2D(this, SoundClick);
}
