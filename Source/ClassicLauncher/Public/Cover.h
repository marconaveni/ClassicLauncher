// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cover.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UCover : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "Classic Launcher|Components")
	class UImage* ImgBackGround;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "Classic Launcher|Components")
	class UArrow* WBPFocusArrow;

	UFUNCTION(BlueprintCallable, category = "Classic Launcher|Functions")
	void FocusCover(bool bEnable);

	UFUNCTION(BlueprintCallable, category = "Classic Launcher|Functions")
	void SetCoverImage(UTexture2D* Texture, int32 Width, int32 Height);

	UFUNCTION(BlueprintCallable, category = "Classic Launcher|Functions")
	bool GetPositionCover();

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim), category = "Classic Launcher|Animations")
	UWidgetAnimation* FadeInImage;
};
