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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImgBackGround;

	UFUNCTION(BlueprintCallable, category = "Cover|Functions")
	void LoadCoverImage(UTexture2D* texture, int32 width, int32 height);
	
};
