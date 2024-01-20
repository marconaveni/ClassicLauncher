// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Screens/LoadingScreen.h"

#include "Components/TextBlock.h"

void ULoadingScreen::ShowMessage(FText NewMessage) const
{
	Message->SetText(NewMessage);
}
