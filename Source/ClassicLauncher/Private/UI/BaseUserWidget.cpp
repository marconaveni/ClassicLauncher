// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/BaseUserWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Data/DataManager.h"



UBaseUserWidget::UBaseUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , bInputEnable(false)
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
	SetHideMouse();
	DataManager = GetWorld()->GetSubsystem<UDataManager>();
}

void UBaseUserWidget::PressInput(const FKey InKey)
{
	NativePressInput(InKey);
}

void UBaseUserWidget::ReleaseInput(const FKey InKey)
{
	NativeReleaseInput(InKey);
}

void UBaseUserWidget::NativePressInput(const FKey& InKey)
{
	if(GetInputEnable())
	{
		InputLastPressed = UClassicFunctionLibrary::GetInputButtonsGame(InKey);
		InputPressed = InputLastPressed;
		bKeyPressed = (InputLastPressed != EButtonsGame::NONE);
	}
	if (MouseHide->GetVisibility() == ESlateVisibility::Hidden && !InKey.IsMouseButton())
	{
		SetHideMouse();
	}
}

void UBaseUserWidget::NativeReleaseInput(const FKey& InKey)
{
	if(GetInputEnable())
	{
		InputLastPressed = UClassicFunctionLibrary::GetInputButtonsGame(InKey);
		InputPressed = EButtonsGame::NONE;
	}
	bKeyPressed = false;
	CancelDelay();
	FirstDelayInput = DefaultFirstDelayInput;
	DefaultTimerDelayInput = TimerDelayInput;
}

FReply UBaseUserWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	NativePressInput(InKeyEvent.GetKey());
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UBaseUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	NativeReleaseInput(InKeyEvent.GetKey());
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

FReply UBaseUserWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UBaseUserWidget::SetHideMouse() const
{
	FVector2D MousePosition;
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosition.X, MousePosition.Y))
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

FReply UBaseUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FKey Key = EKeys::Gamepad_FaceButton_Right;
		NativeReleaseInput(Key);
	}
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UBaseUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const double CursorDelta = InMouseEvent.GetCursorDelta().Size();
	if (CursorDelta > 0)
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
		GetWorld()->GetTimerManager()
		.SetTimer(DelayHandle, [&]()
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

void UBaseUserWidget::SetInputEnable(const bool bEnable)
{
	if(bInputEnable != bEnable)
	{
		bInputEnable = bEnable;
		SetVisibility(bEnable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::HitTestInvisible);
	}
}

bool UBaseUserWidget::GetInputEnable()
{
	return bInputEnable;
}
