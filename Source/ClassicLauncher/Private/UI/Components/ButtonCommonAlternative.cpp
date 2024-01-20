// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/ButtonCommonAlternative.h"
#include "Components/Image.h"

void UButtonCommonAlternative::SetFocusButton(bool bEnable)
{
	if (bEnable)
	{
		Image->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	Super::SetFocusButton(bEnable);
}

bool UButtonCommonAlternative::Initialize()
{
	BackgroundDrawAs = ESlateBrushDrawType::Box;
	return Super::Initialize();
}

void UButtonCommonAlternative::NativeSetBackgroundAppearance(float CornerRadius, FString Color, float BorderWidth, FString BorderColor, float MarginLeft, float MarginTop, float MarginRight, float MarginBottom)
{
}

void UButtonCommonAlternative::NativeSetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin, float Size)
{
	Text->SetTextStyle(NewTextStyle);
	Text->SetTextImageSize(Size);
	Text->DefaultToImageText(bEnableImageText, true);
}

void UButtonCommonAlternative::NativeSetSize(float Width, float Height)
{
}

void UButtonCommonAlternative::NativeSetArrow(UTexture2D* Texture0, UTexture2D* Texture1, UTexture2D* Texture2, float Angle, float Margin)
{
}

void UButtonCommonAlternative::NativeSetIcon(UTexture2D* Texture, float Margin, bool bEnable)
{
}

void UButtonCommonAlternative::NativeSetImage(UTexture2D* Texture, float Margin, bool bEnable)
{
	Super::NativeSetImage(Texture, Margin, bEnable);
}

void UButtonCommonAlternative::SetButtonTextures(UTexture2D* TextureToFocus, UTexture2D* TextureNoFocus)
{
	Image->SetBrushFromTexture(TextureNoFocus);
	BackgroundFocus->SetBrushFromTexture(TextureToFocus);
}
