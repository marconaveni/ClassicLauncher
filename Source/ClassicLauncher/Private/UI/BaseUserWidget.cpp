// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "UI/BaseUserWidget.h"

#include "ClassicFunctionLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"


UBaseUserWidget::UBaseUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bInputDelay(false)
	, TimerDelayInput(0.18f)
	, DefaultTimerDelayInput(0.18f)
	, FirstDelayInput(0.18f)
	, DefaultFirstDelayInput(0.18f)
{
}

void UBaseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MouseHide->SetVisibility(ESlateVisibility::Hidden);
	MouseHide->SetCursor(EMouseCursor::None);
}

void UBaseUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	MouseHide->SetRenderOpacity(0);
}

void UBaseUserWidget::PressedInput(const FKey InKey)
{
	NativePressedInput(InKey);
}

void UBaseUserWidget::ReleaseInput(const FKey InKey)
{
	NativeReleaseInput(InKey);
}

FReply UBaseUserWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	NativePressedInput(InKeyEvent.GetKey());
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UBaseUserWidget::NativePressedInput(const FKey& InKey)
{
	if (MouseHide->GetVisibility() == ESlateVisibility::Hidden && !InKey.IsMouseButton())
	{
		FVector2D MousePosition;
		if(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosition.X,MousePosition.Y))
		{
			UE_LOG(LogTemp, Warning, TEXT("Inside Viewport"));
			MouseHide->SetVisibility(ESlateVisibility::Visible);
			GetOwningPlayer()->SetMouseLocation(MousePosition.X + 1, MousePosition.Y);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutSide Viewport"));
		}
	}
}

FReply UBaseUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(UClassicFunctionLibrary::GetInputButton(InKeyEvent) == EButtonsGame::A)
	{
		NativeReleaseInput(InKeyEvent.GetKey());
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UBaseUserWidget::NativeReleaseInput(const FKey& InKey)
{
	CancelDelay();
	FirstDelayInput = DefaultFirstDelayInput;
	DefaultTimerDelayInput = TimerDelayInput;
	bInputDelay = false;
}

FReply UBaseUserWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UBaseUserWidget::OnAnimationStartedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationStartedPlaying(Player);
	//const UWidgetAnimation* AnimationGet = Player.GetAnimation();
}

void UBaseUserWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
}

FReply UBaseUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UBaseUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    const double CursorDelta = InMouseEvent.GetCursorDelta().Size();
	if(CursorDelta > 0)
	{
		MouseHide->SetVisibility(ESlateVisibility::Hidden);
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UBaseUserWidget::DelayInput(const float Delay)
{
	if (!bInputDelay)
	{
		bInputDelay = true;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, [&]()
		{
			bInputDelay = false;
		}
		, 0.016f, false, Delay);
	}
}

bool UBaseUserWidget::GetDelayInput() const
{
	return bInputDelay;
}

void UBaseUserWidget::CancelDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
	DelayHandle.Invalidate();
	bInputDelay = false;
}

bool UBaseUserWidget::GetMouseEnable()
{
	return MouseHide->GetVisibility() == ESlateVisibility::Hidden;
}

