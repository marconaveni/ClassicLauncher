// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicButtonsIcons.h"
#include "Components/Image.h"
#include "TextImageBlock.h"


void UClassicButtonsIcons::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonsIcons(ButtonsIcons);

}

void UClassicButtonsIcons::SetComponents(TArray<FButtonsIcons>& ButtonsIcon)
{
	ButtonsIcons.SetNum(6);
	ButtonsIcons[0].IconReference = Icon1;
	ButtonsIcons[1].IconReference = Icon2;
	ButtonsIcons[2].IconReference = Icon3;
	ButtonsIcons[3].IconReference = Icon4;
	ButtonsIcons[4].IconReference = Icon5;
	ButtonsIcons[5].IconReference = Icon6;
	ButtonsIcons[0].IconBackgroundReference = IconB1;
	ButtonsIcons[1].IconBackgroundReference = IconB2;
	ButtonsIcons[2].IconBackgroundReference = IconB3;
	ButtonsIcons[3].IconBackgroundReference = IconB4;
	ButtonsIcons[4].IconBackgroundReference = IconB5;
	ButtonsIcons[5].IconBackgroundReference = IconB6;
	ButtonsIcons[0].TextImageBlockReference = TextBlock1;
	ButtonsIcons[1].TextImageBlockReference = TextBlock2;
	ButtonsIcons[2].TextImageBlockReference = TextBlock3;
	ButtonsIcons[3].TextImageBlockReference = TextBlock4;
	ButtonsIcons[4].TextImageBlockReference = TextBlock5;
	ButtonsIcons[5].TextImageBlockReference = TextBlock6;
}


void UClassicButtonsIcons::SetButtonsIcons(TArray<FButtonsIcons> ButtonsIcon)
{
	ButtonsIcons = ButtonsIcon;

	SetComponents(ButtonsIcons);

	for (int32 i = 0; i < ButtonsIcons.Num(); i++)
	{
		ButtonsIcons[i].IconBackgroundReference->SetBrushFromTexture(ButtonsIcons[i].BgButtonTexture);
		ButtonsIcons[i].IconReference->SetBrushFromTexture(ButtonsIcons[i].ButtonTexture);

		ButtonsIcons[i].TextImageBlockReference->SetFText(ButtonsIcons[i].ButtonText);
		ButtonsIcons[i].TextImageBlockReference->SetTextStyle(ButtonsIcons[i].TextStyle);

		ButtonsIcons[i].IconBackgroundReference->SetVisibility(ButtonsIcons[i].EVisibility);
		ButtonsIcons[i].IconReference->SetVisibility(ButtonsIcons[i].EVisibility);
		ButtonsIcons[i].TextImageBlockReference->SetVisibility(ButtonsIcons[i].EVisibility);

		ButtonsIcons[i].TextImageBlockReference->DefaultToImageText(ButtonsIcons[i].DefaultToImageText);
		ButtonsIcons[i].TextImageBlockReference->SetTextStyle(ButtonsIcons[i].TextStyle);

	}
}

void UClassicButtonsIcons::SetIconsStyle(TArray<UTexture2D*> BgTextures, TArray<UTexture2D*> Textures)
{
	for (int32 i = 0; i < ButtonsIcons.Num(); i++)
	{
		ButtonsIcons[i].IconBackgroundReference->SetBrushFromTexture(BgTextures[i]);
		ButtonsIcons[i].IconReference->SetBrushFromTexture(Textures[i]);
	}
}

void UClassicButtonsIcons::SetTexts(TArray<FText> ButtonsText)
{
	for (int32 i = 0; i < ButtonsIcons.Num(); i++)
	{
		ButtonsIcons[i].TextImageBlockReference->SetFText(ButtonsText[i]);
	}
}

void UClassicButtonsIcons::SetColorsText(FSlateColor NewColor)
{
	for (int32 i = 0; i < ButtonsIcons.Num(); i++)
	{
		FTextStyle Style = ButtonsIcons[i].TextStyle;
		Style.Color = NewColor;
		ButtonsIcons[i].TextImageBlockReference->SetTextStyle(Style);
	}
}

void UClassicButtonsIcons::SetTextDefaultStyle(FTextStyle TextStyle)
{
	for (int32 i = 0; i < ButtonsIcons.Num(); i++)
	{
		ButtonsIcons[i].TextImageBlockReference->SetTextStyle(TextStyle);
	}
}

void UClassicButtonsIcons::SetButtonsVisibility(TArray<ESlateVisibility> EVisibility)
{
	for (int32 i = 0; i < ButtonsIcons.Num(); i++)
	{
		ButtonsIcons[i].IconBackgroundReference->SetVisibility(EVisibility[i]);
		ButtonsIcons[i].IconReference->SetVisibility(EVisibility[i]);
		ButtonsIcons[i].TextImageBlockReference->SetVisibility(EVisibility[i]);
	}
}

void UClassicButtonsIcons::AlternateToTextImage(bool bEnable, float Size)
{
	for (int32 i = 0; i < ButtonsIcons.Num(); i++)
	{
		ButtonsIcons[i].TextImageBlockReference->DefaultToImageText(bEnable);
		ButtonsIcons[i].TextImageBlockReference->SetTextImageSize(Size);
		ButtonsIcons[i].TextImageBlockReference->UpdateText();
	}
}



