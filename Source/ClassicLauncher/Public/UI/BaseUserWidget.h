// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DoOnce.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UImage;
class UTextImageBlock;
class UCanvasPanel;
class UToolTip;
class UClassicButton;
class UFrame;
class UClassicInfoInterface;
class UClassicSystemListInterface;
class UClassicConfigurations;
class UClassicButtonsIcons;
class UMessageBalloon;
class UTextBlock;
class ULoopScrollBox;
class UTextBoxScroll;
class UClassicGameInstance;
class UClassicButtonSystem;
class AClassicMediaPlayer;
class AClassicLibretroTV;
class UCanvasPanelSlot;
class AClassicGameMode;
class UCard;
class UCover;
class UScrollBox;
class UHorizontalBox;
class UButton;
class UCanvasPanel;
class UScaleBox;
class UToolTipsLayout;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FTimerHandle TickTimerHandle;

	UPROPERTY()
	FTimerHandle DelayHandle;

	UPROPERTY()
	FTimerHandle MouseHandle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MouseHide;

	UBaseUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnAnimationStartedPlaying(UUMGSequencePlayer& Player) override;
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void TimeTick();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface")
	void OnTimeTick();

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void DelayInput(const float Delay);

	UFUNCTION(BlueprintPure, Category = "User Interface")
	bool GetDelayInput() const;

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void CancelDelay();

	UPROPERTY()
	FDoOnce DoOnceHideCursor;

	UPROPERTY()
	bool bInputDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float TimerDelayInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Defaults")
	float DefaultTimerDelayInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float FirstDelayInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Defaults")
	float DefaultFirstDelayInput;

};
