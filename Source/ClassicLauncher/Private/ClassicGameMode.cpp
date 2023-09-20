// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicGameMode.h"
#include "LoadingGameData.h"


void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AClassicGameMode::Init()
{
	Data = NewObject<ULoadingGameData>(this, ULoadingGameData::StaticClass(), FName("Data"));
	Data->Init(MainInterfaceClass, LoadingScreenClass);
	Data->CreateWidgets();
	Data->AddLoadingScreenToViewPort();
	Data->LoadConfiguration();
}
