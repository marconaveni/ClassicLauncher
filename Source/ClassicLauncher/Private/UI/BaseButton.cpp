// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/BaseButton.h"

#include "FunctionLibrary/ClassicFunctionLibrary.h"
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

FReply UBaseButton::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UBaseButton::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const EButtonsGame NewInput = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
	if (NewInput == EButtonsGame::A)
	{
		ButtonClick();
	}
	Input = EButtonsGame::NONE;
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

FReply UBaseButton::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
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

void UBaseButton::SetIndex(const int32 NewIndex)
{
	Index = NewIndex;
}
