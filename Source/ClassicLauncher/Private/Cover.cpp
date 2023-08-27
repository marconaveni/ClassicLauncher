// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Cover.h"
#include "Components/Image.h"
#include "Arrow.h"

void UCover::FocusCover(bool Enable)
{
	if (Enable)
	{
		WBPFocusArrow->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WBPFocusArrow->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCover::SetCoverImage(UTexture2D* Texture, int32 Width, int32 Height)
{
	if (Texture != nullptr) 
	{
		FSlateBrush NewBrush;
		NewBrush.SetImageSize(FVector2D(Width, Height));
		NewBrush.SetResourceObject(Texture);
		ImgBackGround->SetBrush(NewBrush);
	}
}
