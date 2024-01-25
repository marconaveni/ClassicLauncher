// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Core/ClassicGameMode.h"
#include "Core/ScreenManager.h"
#include "Data/DataManager.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"


void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AClassicGameMode::Init()
{
	UClassicFunctionLibrary::GameSettingsInit();
	UDataManager* DataManager = GetWorld()->GetSubsystem<UDataManager>();
	DataManager->GetScreenManager()->Init(MainInterfaceClass, LoadingScreenClass);
	DataManager->LoadConfiguration();
	DataManager->InitGameData();
}



