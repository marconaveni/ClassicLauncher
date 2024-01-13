// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Cover.h"
#include "Components/Image.h"
#include "Arrow.h"
#include "Blueprint/SlateBlueprintLibrary.h"

void UCover::FocusCover(bool bEnable)
{
	if (bEnable)
	{
		WBPFocusArrow->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WBPFocusArrow->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCover::SetCoverImage(UTexture2D* Texture)
{
	if (Texture != nullptr) 
	{
		const int32 Width = Texture->GetSizeX();
		const int32 Height = Texture->GetSizeY();
		FSlateBrush NewBrush;
		NewBrush.SetImageSize(FVector2D(Width, Height));
		NewBrush.SetResourceObject(Texture);
		ImgBackGround->SetBrush(NewBrush);
	}
}

bool UCover::GetPositionCover()
{
	const FGeometry& Geometry = GetCachedGeometry();
	FVector2D PixelPosition, ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(this, Geometry, FVector2D(0,0), PixelPosition,ViewportPosition);
	if(PixelPosition.X > 90 && PixelPosition.X < 1800)
	{
		return true;
	}
	return false;
}
