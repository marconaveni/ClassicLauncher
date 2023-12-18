// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ToolTip.h"
#include "Components/Image.h"
#include "Components/Overlay.h"


UToolTip::UToolTip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UToolTip::NativePreConstruct()
{
	TextBlock->SetText(Text);

	const FString NewName = FString::Printf(TEXT("tooltip_%d"), Index);
	IndexName = FName(*NewName);
	Super::NativePreConstruct();
}

void UToolTip::NativeConstruct()
{
	Super::NativeConstruct();
}

void UToolTip::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UToolTip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UToolTip::SetFocusToolTip(bool bEnable)
{
	bFocus = bEnable;
	ShowToolTip(bFocus);
}

bool UToolTip::GetToolTipFocus()
{
	return bFocus;
}

void UToolTip::SetTextAppearance(FTextStyle NewTextStyle)
{
	TextBlock->SetTextStyle(NewTextStyle);
}

void UToolTip::AlternateToTextImage(bool bEnable, float Size)
{
	TextBlock->SetTextImageSize(Size);
	TextBlock->DefaultToImageText(bEnable, true);
}

void UToolTip::SetToolTipThemes(UTexture2D* TextureBackground, UTexture2D* TextureDetail, bool TextImage, float Size, FTextStyle NewTextStyle)
{
	AlternateToTextImage(TextImage, Size);
	SetTextAppearance(NewTextStyle);
	BgImage->SetBrushFromTexture(TextureBackground);
	BgImageTop->SetBrushFromTexture(TextureDetail);
}
