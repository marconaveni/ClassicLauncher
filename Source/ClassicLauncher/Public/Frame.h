// Copyright 2023 Marco Naveni. All Rights Reserved.

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	int32 FrameIndexCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	int32 FrameIndexTop = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	UTexture2D* TextureFrameTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	UTexture2D* TextureFrameCenter;

	UFUNCTION()
	void SetFrame(const int32& IndexFrame, const EPositionY& Position);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetFrame(const int32& IndexFrame, const EPositionY& Position);

	UFUNCTION()
	void SetFrameIndexTop(const EButtonsGame Input, const int32 IndexLimit);

	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetDefaultValues(int32 MaxFrameRightLimit, float MaxSpeed);

	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetFrameCenterPosition(int32 PositionCenter);

};
