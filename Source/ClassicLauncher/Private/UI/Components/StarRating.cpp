// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/StarRating.h"
#include "Components/Image.h"

void UStarRating::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetUpdateRatingStar(Rating);
}

UStarRating::UStarRating(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UStarRating::SetTextureStar(UTexture2D* TStar0, UTexture2D* TStar1 , UTexture2D* TStar2, UTexture2D* TStar3, UTexture2D* TStar4 ) {
	Star0->SetBrushFromTexture(TStar0);
	Star1->SetBrushFromTexture(TStar1);
	Star2->SetBrushFromTexture(TStar2);
	Star3->SetBrushFromTexture(TStar3);
	Star4->SetBrushFromTexture(TStar4);
}

void UStarRating::SetUpdateRatingStar(float NewValue)
{
	const int32 Value = NewValue * 100;

	if (Value == 0) {
		SetTextureStar(ImageNull, ImageNull, ImageNull, ImageNull, ImageNull);
	}
	else if (Value <= 10) {
		SetTextureStar(ImageHalfStar, ImageNull, ImageNull, ImageNull, ImageNull);	
	}
	else if (Value  <= 20 ) {
		SetTextureStar(ImageStar, ImageNull, ImageNull, ImageNull, ImageNull);
	}
	else if (Value <= 30) {
		SetTextureStar(ImageStar, ImageHalfStar, ImageNull, ImageNull, ImageNull);
	}
	else if (Value <= 40) {
		SetTextureStar(ImageStar, ImageStar, ImageNull, ImageNull, ImageNull);
	}
	else if (Value <= 50) {
		SetTextureStar(ImageStar, ImageStar, ImageHalfStar, ImageNull, ImageNull);
	}
	else if (Value <= 60) {
		SetTextureStar(ImageStar, ImageStar, ImageStar, ImageNull, ImageNull);
	}
	else if (Value <= 70) {
		SetTextureStar(ImageStar, ImageStar, ImageStar, ImageHalfStar, ImageNull);
	}
	else if (Value <= 80) {
		SetTextureStar(ImageStar, ImageStar, ImageStar, ImageStar, ImageNull);
	}
	else if (Value <= 90) {
		SetTextureStar(ImageStar, ImageStar, ImageStar, ImageStar, ImageHalfStar);
	}
	else{
		SetTextureStar(ImageStar, ImageStar, ImageStar, ImageStar, ImageStar);
	}

}
