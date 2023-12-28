// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Footer.generated.h"

class UImage;
class UOverlay;
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UFooter : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Image;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Main;

	UFUNCTION(BlueprintCallable,meta = (Category = "Theme"))
	void SetBackground(UTexture2D* MainTexture, UTexture2D* BackgroundTexture);
};
