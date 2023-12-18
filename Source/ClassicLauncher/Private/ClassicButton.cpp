// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicButton.h"
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

bool UClassicButton::Initialize()
{
	const bool Success = Super::Initialize();
	if (BtButton)
	{
		BtButton->OnClicked.AddDynamic(this, &UClassicButton::ButtonClick);
	}
	return Success;
}

void UClassicButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (BtButton != nullptr) {
		if (!BtButton->HasKeyboardFocus() && bFocus)
		{
			EnableFocusButton(false);
		}
	}
}

void UClassicButton::SetFocusButton()
{
	BtButton->SetKeyboardFocus();
	EnableFocusButton(true);
}

void UClassicButton::EnableFocusButton(bool bEnable)
{
	bFocus = bEnable;
	if (bEnable)
	{
		OnFocusTrigger.Broadcast(Index);
		BgImage->SetVisibility(ESlateVisibility::Visible);
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

