// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/Modal.h"

#include "TextImageBlock.h"
#include "Components/Image.h"


void UModal::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Title != nullptr && Icon != nullptr && Background != nullptr)
	{
		SetTitleText(TitleText);
		SetIconTexture(IconTexture);
		SetBackgroundTexture(BackgroundTexture);
	}
}

void UModal::SetTitleText(FText NewTitle)
{
	TitleText = NewTitle;
	Title->SetText(NewTitle);
}

void UModal::SetIconTexture(UTexture2D* Texture)
{
	IconTexture = Texture;
	Icon->SetBrushFromTexture(Texture);
}

void UModal::SetBackgroundTexture(UTexture2D* Texture)
{
	BackgroundTexture = Texture;
	Background->SetBrushFromTexture(Texture);
}

UButtonsPrompt* UModal::GetPrompt() const
{
	return Prompt;
}

UTextImageBlock* UModal::GetTitle() const
{
	return Title;
}
