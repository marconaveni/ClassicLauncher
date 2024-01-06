// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
class UFooter;
class UHeader;
class UFooterDetails;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	bool bInputEnable;

protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MouseHide;
	
	UPROPERTY()
	FTimerHandle TickTimerHandle;

	UPROPERTY()
	FTimerHandle DelayHandle;

	UPROPERTY()
	FTimerHandle MouseHandle;

	UPROPERTY()
	bool bInputDelay;
	
	UBaseUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativePressedInput(const FKey& InKey);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeReleaseInput(const FKey& InKey);
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnAnimationStartedPlaying(UUMGSequencePlayer& Player) override;
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION(BlueprintCallable,  Category = "Input")
	void PressedInput(const FKey InKey);
	
	UFUNCTION(BlueprintCallable,  Category = "Input")
	void ReleaseInput(const FKey InKey);
	
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void DelayInput(const float Delay);

	UFUNCTION(BlueprintPure, Category = "User Interface")
	bool GetDelayInput() const;

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void CancelDelay();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float TimerDelayInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Defaults")
	float DefaultTimerDelayInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float FirstDelayInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Defaults")
	float DefaultFirstDelayInput;

public:

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetHideMouse() const;
	
	UFUNCTION(BlueprintPure, Category = "Input")
	bool GetMouseEnable();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInputEnable(const bool bEnable);

	UFUNCTION(BlueprintPure, Category = "Input")
	bool GetInputEnable();
};
