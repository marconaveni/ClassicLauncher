// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicSlide.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicSlide : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* SliderVol;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UArrow* WBPArrow;

	UFUNCTION(BlueprintCallable, Category = "ClassicSlide|Functions")
	void SetFocusSlide();



private:

	UPROPERTY()
	bool Hover;
};
