// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Components/ButtonCommon.h"
#include "ButtonSlide.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusLostSlide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSlide, int32, value);
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UButtonSlide : public UButtonCommon
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusLostSlide OnFocusLostTriggerSlide;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateSlide OnSlideTrigger;

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeSetBackgroundAppearance(float CornerRadius,FString Color,float BorderWidth,FString BorderColor,float MarginLeft,float MarginTop,float MarginRight,float MarginBottom) override;
	virtual void NativeSetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin, float Size) override;
	void MoveSlide();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* SliderVolume;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasSlide;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void OnSlideValue(float Value);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetSlideValue(float Value);

	UFUNCTION(BlueprintPure, Category = "ClassicSlide|Functions")
	float GetSlideValue();

	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetSlidePosition(FVector2D Size);
	
	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;
};
