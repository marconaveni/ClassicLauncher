// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "ClassicButtonSystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Arrow.h"
#include "ClassicFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

UClassicButtonSystem::UClassicButtonSystem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bMouseFocus(false)
{
}

void UClassicButtonSystem::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text->SetText(ButtonText);
}

FReply UClassicButtonSystem::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	//const EButtonsGame NewInput = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UClassicButtonSystem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMouseFocus = true;
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

FReply UClassicButtonSystem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UClassicButtonSystem::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	bMouseFocus = false;
}

FReply UClassicButtonSystem::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

bool UClassicButtonSystem::Initialize()
{
	bool Success = Super::Initialize();
	return Success;
}

void UClassicButtonSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UClassicButtonSystem::SetText(FText NewText)
{
	Text->SetText(NewText);
	ButtonText = NewText;
}

void UClassicButtonSystem::SetFocusButton(bool bEnable)
{
	if (bEnable)
	{
		BgBackground->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WBPArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if(!bMouseFocus)
		{
			UGameplayStatics::PlaySound2D(this, SoundSelect);
		}
	}
	else
	{
		BgBackground->SetVisibility(ESlateVisibility::Hidden);
		WBPArrow->SetVisibility(ESlateVisibility::Hidden);
	}
	Super::SetFocusButton(bEnable);
}

void UClassicButtonSystem::ButtonClick()
{
	if (HasAnyUserFocus())
	{
		UGameplayStatics::PlaySound2D(this, SoundClick);
		//FSlateApplication::Get().SetAllUserFocusToGameViewport();
	}
	Super::ButtonClick();
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

void UClassicButtonSystem::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundClick = SelectSound;
	SoundSelect = NavigateSound;
}
