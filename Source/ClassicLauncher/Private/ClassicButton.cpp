// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicButton.h"

#include "ClassicFunctionLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"

UClassicButton::UClassicButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Index(0)
	, bFocus(false)
{
}

void UClassicButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	BtButton->SetStyle(StyleButton);
}

FReply UClassicButton::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
	if(Input == EButtonsGame::A)
	{
		ButtonClick();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UClassicButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	SetFocus();
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

FReply UClassicButton::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if(!HasFocusButton() && GetIsEnabled())
	{
		SetFocusButton(true);
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UClassicButton::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	SetFocusButton(false);
	Super::NativeOnFocusLost(InFocusEvent);
}

bool UClassicButton::Initialize()
{
	const bool Success = Super::Initialize();
	SetIsFocusable(true);
	if (BtButton)
	{
		BtButton->OnReleased.AddDynamic(this, &UClassicButton::ButtonClick);
	}
	return Success;
}

void UClassicButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UClassicButton::SetFocusButton(bool bEnable)
{
	bFocus = bEnable;
	
	if (bEnable)
	{
		OnFocusTrigger.Broadcast(Index);
		BgImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayAnimationForward(FocusButton);
	}
	else
	{
		OnFocusLostTrigger.Broadcast(Index);
		BgImage->SetVisibility(ESlateVisibility::Hidden);
		PlayAnimationReverse(FocusButton);
	}
}

void UClassicButton::ButtonClick()
{
	OnClickTrigger.Broadcast(Index);
}

bool UClassicButton::HasFocusButton()
{
	return bFocus;
}

void UClassicButton::SetTheme(UTexture2D* TextureIcon, FSlateBrush BackgroundColor)
{
	BgImage->SetBrush(BackgroundColor);

	FSlateBrush Icon;
	Icon.SetResourceObject(TextureIcon);
	FButtonStyle ButtonStyle;
	ButtonStyle.SetNormal(Icon);
	ButtonStyle.SetHovered(Icon);
	ButtonStyle.SetPressed(Icon);
	ButtonStyle.SetDisabled(Icon);
	BtButton->SetStyle(ButtonStyle);
}

void UClassicButton::EnableButton(const bool bEnable)
{
	SetIsEnabled(bEnable);
	const float Alpha = (bEnable) ? 1 : 0.5f; 
	SetColorAndOpacity(FLinearColor(1, 1, 1, Alpha));
}

