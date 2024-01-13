// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassicFunctionLibrary.h"
#include "MainInterface.h"
#include "Blueprint/UserWidget.h"
#include "Frame.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateAnimationStartFrame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateAnimationEndFrame);
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UFrame : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFrame(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateAnimationStartFrame OnAnimationStart;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateAnimationEndFrame OnAnimationEnd;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageFrameCenter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageFrameTop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* PaddingImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* PaddingOverlay;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	FVector2D FrameCenter2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	FVector2D FrameTop2D;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	int32 FrameIndexCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	int32 FrameIndexTop = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	FName ImgCenterAnimationLeftRightTop = TEXT("ImgCenterAnimationLeftRightTop");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	FName ImgCenterAnimationLeftRight = TEXT("ImgCenterAnimationLeftRight");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	EPositionY Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	class ULoopScrollBox* LoopScrollReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	UTexture2D* TextureFrameTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	UTexture2D* TextureFrameCenter;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetFrame();

	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetIndexTop(const EButtonsGame Input, const int32 IndexLimit);

	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetFramePositionWithAnimation(const EPositionY& NewPosition);
	
	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetFramePositionWithoutAnimation(int32 PositionCenter);

};
