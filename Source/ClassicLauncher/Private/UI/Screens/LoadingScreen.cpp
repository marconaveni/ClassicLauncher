// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Screens/LoadingScreen.h"

#include "Components/TextBlock.h"
#include "Data/DataManager.h"

void ULoadingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UDataManager* DataManager = GetWorld()->GetSubsystem<UDataManager>();
	DataManager->OnMessageShow.AddDynamic(this , &ULoadingScreen::ShowMessage);
}

void ULoadingScreen::ShowMessage(const FText& NewMessage)
{
	Message->SetText(NewMessage);
}
