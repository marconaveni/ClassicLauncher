// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassicFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Frame.generated.h"

UENUM(BlueprintType, Category = "Navigation")
enum class EFocusTop : uint8
{
	NONE		  UMETA(DisplayName = "None"),
	SYSTEM        UMETA(DisplayName = "System"),
	CONFIG        UMETA(DisplayName = "Configuration"),
	FAVORITE      UMETA(DisplayName = "Favorite"),
	INFO          UMETA(DisplayName = "Information")
};

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
	virtual void OnAnimationStartedPlaying(UUMGSequencePlayer& Player) override;
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateAnimationStartFrame OnAnimationStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateAnimationEndFrame OnAnimationEnd;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* MoveLeftRightCenter;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* MoveLeftRightTop1;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* MoveLeftRightTop2;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* MoveLeftRightTop3;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImageFrameCenter;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImageFrameTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	int32 FrameIndexCenter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	int32 FrameIndexTop = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	int32 MaxFrameLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	bool bIsNotAnimated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	bool bAtRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	bool bAtLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	float EndAtTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	float StartTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	float PlaybackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	UTexture2D* TextureFrameTop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame|Variables")
	UTexture2D* TextureFrameCenter;




	UFUNCTION(BlueprintImplementableEvent)
	void OnDirectionLeft(const int32& Frame, const int32& Limit);
	UFUNCTION(BlueprintImplementableEvent)
	void OnDirectionRight(const int32& Frame, const int32& Limit);

	UFUNCTION()
	void TopFocus();
	UFUNCTION()
	void CenterFocus();
	UFUNCTION()
	void DirectionRightLeftTop(EButtonsGame Input, int32 IndexLimit);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTopFocus();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCenterFocus();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDirectionRightLeftTop();

	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void Clear();
	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetDefaultValues(int32 MaxFrameRightLimit, float MaxSpeed);
	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetFrameCenterPosition(int32 PositionCenter);
	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void SetFrameTopPosition(EFocusTop FocusTop);
	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void DirectionRight(int32 Frame, int32 Limit);
	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void DirectionLeft(int32 Frame, int32 Limit);
	UFUNCTION(BlueprintCallable, Category = "Frame|Functions")
	void AnimationToTopDown(EFocusTop Focus, bool Forward);
	UFUNCTION(BlueprintCallable, Category = "Frame|Animations")
	void AnimationFrameToTop(UWidgetAnimation* Animation1, UWidgetAnimation* Animation2, UWidgetAnimation* Animation3, UWidgetAnimation* Animation4, bool Reverse);

	
};
