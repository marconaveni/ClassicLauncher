// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoadingScreen.h"

#include "Components/TextBlock.h"

void ULoadingScreen::ShowMessage(FText NewMessage) const
{
	Message->SetText(NewMessage);
}
