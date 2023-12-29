// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "UI/BaseUserWidget.h"

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
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UBaseUserWidget::TimeTick, 0.01667f, true, -1);
}

FReply UBaseUserWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UBaseUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	CancelDelay();
	FirstDelayInput = DefaultFirstDelayInput;
	DefaultTimerDelayInput = TimerDelayInput;
	bInputDelay = false;
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
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
	const double Move = InMouseEvent.GetCursorDelta().Size();
	if(Move > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MouseHandle);
		DoOnceHideCursor.Reset();
		MouseHide->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Warning, TEXT("show mouse"));
	}
	else
	{
		if (DoOnceHideCursor.Execute())
		{
			GetWorld()->GetTimerManager().SetTimer(MouseHandle, [&]()
			{
				MouseHide->SetVisibility(ESlateVisibility::Visible);
				FVector2f Position;
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(Position.X,Position.Y);
				GetOwningPlayer()->SetMouseLocation(Position.X + 1, Position.Y);
				//UE_LOG(LogTemp, Warning, TEXT("hide mouse after 5 seconds"));
			}
			, 5, false);

			//UE_LOG(LogTemp, Warning, TEXT("mouse will hide "));
		}
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UBaseUserWidget::TimeTick()
{
	OnTimeTick();
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

