// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/ButtonIcon.h"
#include "Components/Button.h"
#include "Components/Image.h"

UButtonIcon::UButtonIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UButtonIcon::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Button != nullptr)
	{
		Button->SetStyle(StyleButton);
	}
}

FReply UButtonIcon::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UButtonIcon::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

FReply UButtonIcon::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UButtonIcon::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
}

bool UButtonIcon::Initialize()
{
	const bool Success = Super::Initialize();
	return Success;
}

void UButtonIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UButtonIcon::SetTheme(UTexture2D* TextureIcon, FSlateBrush BackgroundColor)
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

void UButtonIcon::EnableButton(const bool bEnable)
{
	SetIsEnabled(bEnable);
	const float Alpha = (bEnable) ? 1 : 0.5f; 
	SetColorAndOpacity(FLinearColor(1, 1, 1, Alpha));
}

void UButtonIcon::SetFocusButton(bool bEnable)
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

