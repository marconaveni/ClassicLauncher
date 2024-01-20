// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StarRating.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UStarRating : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Star0;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Star1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Star2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Star3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Star4;

	virtual void NativePreConstruct() override;

	UStarRating(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintCallable, Category = "Star")
	void SetTextureStar(UTexture2D* TStar0, UTexture2D* TStar1, UTexture2D* TStar2, UTexture2D* TStar3, UTexture2D* TStar4);


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star|Variables" , meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Rating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star|Variables")
	UTexture2D* ImageNull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star|Variables")
	UTexture2D* ImageHalfStar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star|Variables")
	UTexture2D* ImageStar;

	UFUNCTION(BlueprintCallable, Category = "Star")
	void SetUpdateRatingStar(float NewValue);
	
};
