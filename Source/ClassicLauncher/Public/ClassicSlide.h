// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MusicInterface.h"
#include "Blueprint/UserWidget.h"
#include "TextImageBlock.h"
#include "ClassicSlide.generated.h"

class UButton;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusSlide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusLostSlide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSlide, int32, value);
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicSlide : public UUserWidget, public IMusicInterface
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusSlide OnFocusTriggerSlide;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusLostSlide OnFocusLostTriggerSlide;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateSlide OnSlide;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* SliderVol;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UArrow* WBPArrow;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtLabel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Click;

	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetFocusSlide();
	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void OnSlideValue(float Value);
	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetSlideValue(float Value);
	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetTextLabel(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void AlternateToTextImage(bool bEnable, float Size = 24);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSlide|Variables")
	class USoundBase* SoundNavigation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSlide|Variables")
	FText TxtTextLabel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSlide|Variables")
	float SlideValue;


private:

	UPROPERTY()
	bool Hover;

public:

	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;
};
