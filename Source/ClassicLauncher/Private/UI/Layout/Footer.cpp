// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "UI/Layout/Footer.h"
#include "Components/Image.h"

void UFooter::SetBackground(UTexture2D* MainTexture, UTexture2D* BackgroundTexture)
{
	Main->SetBrushFromTexture(MainTexture);
	Background->SetBrushFromTexture(BackgroundTexture);
}
