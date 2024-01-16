// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Components/ButtonCommon.h"
#include "ButtonSlide.generated.h"



/*
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusSlide);*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusLostSlide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSlide, int32, value);
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UButtonSlide : public UButtonCommon/*, public IMusicInterface*/
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
	void MoveSlide();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* SliderVolume;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBox;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void OnSlideValue(float Value);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetSlideValue(float Value);

	UFUNCTION(BlueprintPure, Category = "ClassicSlide|Functions")
	float GetSlideValue();

	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetSlidePosition(float MarginLeft);
	
	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;
};
