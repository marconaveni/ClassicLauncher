// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/BaseButton.h"

#include "ClassicFunctionLibrary.h"
#include "Components/Button.h"

UBaseButton::UBaseButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , Index(0)
	  , bFocus(false)
{
}

void UBaseButton::NativePreConstruct()
{
	Super::NativePreConstruct();
}

FReply UBaseButton::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
	if (Input == EButtonsGame::A)
	{
		ButtonClick();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UBaseButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetFocus();
}

FReply UBaseButton::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (!HasFocusButton() && GetIsEnabled())
	{
		SetFocusButton(true);
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UBaseButton::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	SetFocusButton(false);
}

bool UBaseButton::Initialize()
{
	const bool Success = Super::Initialize();
	if (Button != nullptr)
	{
		Button->OnReleased.AddDynamic(this, &UBaseButton::ButtonClick);
	}
	SetIsFocusable(true);
	return Success;
}

void UBaseButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBaseButton::ButtonClick()
{
	OnClickTrigger.Broadcast(Index);
}

bool UBaseButton::HasFocusButton()
{
	return bFocus;
}

void UBaseButton::SetFocusButton(bool bEnable)
{
	bFocus = bEnable;
	
	if (bEnable)
	{
		OnFocusTrigger.Broadcast(Index);
	}
	else
	{
		OnFocusLostTrigger.Broadcast(Index);
	}
}
