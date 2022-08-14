// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Cover.h"
#include "Components/Image.h"

void UCover::LoadCoverImage(UTexture2D* texture, int32 width, int32 height)
{
	if (texture != nullptr) {
		ImgBackGround->SetDesiredSizeOverride(FVector2D(FMath::Clamp(width, 32, 363), FMath::Clamp(height, 32, 306)));
		ImgBackGround->SetBrushFromTexture(texture);
	}
}
