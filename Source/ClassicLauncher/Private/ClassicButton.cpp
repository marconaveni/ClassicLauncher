// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "ClassicButton.h"

#include "ClassicFunctionLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"

UClassicButton::UClassicButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UClassicButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Button != nullptr)
	{
		Button->SetStyle(StyleButton);
	}
}

FReply UClassicButton::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UClassicButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

FReply UClassicButton::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UClassicButton::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
}

bool UClassicButton::Initialize()
{
	const bool Success = Super::Initialize();
	return Success;
}

void UClassicButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
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
	Button->SetStyle(ButtonStyle);
}

void UClassicButton::EnableButton(const bool bEnable)
{
	SetIsEnabled(bEnable);
	const float Alpha = (bEnable) ? 1 : 0.5f; 
	SetColorAndOpacity(FLinearColor(1, 1, 1, Alpha));
}

void UClassicButton::SetFocusButton(bool bEnable)
{
	Super::SetFocusButton(bEnable);
	
	if (bEnable)
	{
		BgImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayAnimationForward(FocusButton);
	}
	else
	{
		BgImage->SetVisibility(ESlateVisibility::Hidden);
		PlayAnimationReverse(FocusButton);
	}
}

