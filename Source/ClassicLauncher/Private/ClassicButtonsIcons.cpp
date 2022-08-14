// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicButtonsIcons.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UClassicButtonsIcons::UClassicButtonsIcons(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UClassicButtonsIcons::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtons(ButtonsIcons);

}

void UClassicButtonsIcons::SetButtons(TArray<FButtonsIcons> ButtonsIcon)
{

	TArray<UTexture2D*> BgTextures;
	TArray<UTexture2D*> Textures;
	TArray<FString> Texts;
	TArray<ESlateVisibility> EVisibility;
	if (ButtonsIcon.IsValidIndex(5)) {

		for (int32 i = 0; i < 6; i++)
		{
			BgTextures.Add(ButtonsIcon[i].BgButtonTexture);
			Textures.Add(ButtonsIcon[i].ButtonTexture);
			Texts.Add(ButtonsIcon[i].ButtonText);
			EVisibility.Add(ButtonsIcon[i].EVisibility);
		}

		SetButtonsStyle(BgTextures,Textures);
		SetButtonsText(Texts);
		SetButtonsVisibility(EVisibility);
		SetColorsText(TextColor);

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("The lenght of Array ButtonsIcon is: %d to set 6 values in editor"), ButtonsIcon.Num());
	}

	
}

void UClassicButtonsIcons::SetButtonsStyle(TArray<UTexture2D*> BgTextures, TArray<UTexture2D*> Textures)
{
	if (BgTextures.IsValidIndex(5) && Textures.IsValidIndex(5)) {

		IconB1->SetBrushFromTexture(BgTextures[0]);
		IconB2->SetBrushFromTexture(BgTextures[1]);
		IconB3->SetBrushFromTexture(BgTextures[2]);
		IconB4->SetBrushFromTexture(BgTextures[3]);
		IconB5->SetBrushFromTexture(BgTextures[4]);
		IconB6->SetBrushFromTexture(BgTextures[5]);

		Icon1->SetBrushFromTexture(Textures[0]);
		Icon2->SetBrushFromTexture(Textures[1]);
		Icon3->SetBrushFromTexture(Textures[2]);
		Icon4->SetBrushFromTexture(Textures[3]);
		Icon5->SetBrushFromTexture(Textures[4]);
		Icon6->SetBrushFromTexture(Textures[5]);
	}

}

void UClassicButtonsIcons::SetButtonsText(TArray<FString> ButtonsText)
{
	if (ButtonsText.IsValidIndex(5)) {
		TextBlock1->SetText(FText::FromString(ButtonsText[0]));
		TextBlock2->SetText(FText::FromString(ButtonsText[1]));
		TextBlock3->SetText(FText::FromString(ButtonsText[2]));
		TextBlock4->SetText(FText::FromString(ButtonsText[3]));
		TextBlock5->SetText(FText::FromString(ButtonsText[4]));
		TextBlock6->SetText(FText::FromString(ButtonsText[5]));
	}

}

void UClassicButtonsIcons::SetColorsText(FSlateColor newColor)
{
	TextBlock1->SetColorAndOpacity(newColor);
	TextBlock2->SetColorAndOpacity(newColor);
	TextBlock3->SetColorAndOpacity(newColor);
	TextBlock4->SetColorAndOpacity(newColor);
	TextBlock5->SetColorAndOpacity(newColor);
	TextBlock6->SetColorAndOpacity(newColor);
}


void UClassicButtonsIcons::SetButtonsVisibility(TArray<ESlateVisibility> EVisibility)
{
	if (EVisibility.IsValidIndex(5)) {
		IconB1->SetVisibility(EVisibility[0]);
		IconB2->SetVisibility(EVisibility[1]);
		IconB3->SetVisibility(EVisibility[2]);
		IconB4->SetVisibility(EVisibility[3]);
		IconB5->SetVisibility(EVisibility[4]);
		IconB6->SetVisibility(EVisibility[5]);

		Icon1->SetVisibility(EVisibility[0]);
		Icon2->SetVisibility(EVisibility[1]);
		Icon3->SetVisibility(EVisibility[2]);
		Icon4->SetVisibility(EVisibility[3]);
		Icon5->SetVisibility(EVisibility[4]);
		Icon6->SetVisibility(EVisibility[5]);

		TextBlock1->SetVisibility(EVisibility[0]);
		TextBlock2->SetVisibility(EVisibility[1]);
		TextBlock3->SetVisibility(EVisibility[2]);
		TextBlock4->SetVisibility(EVisibility[3]);
		TextBlock5->SetVisibility(EVisibility[4]);
		TextBlock6->SetVisibility(EVisibility[5]);
	}
}



