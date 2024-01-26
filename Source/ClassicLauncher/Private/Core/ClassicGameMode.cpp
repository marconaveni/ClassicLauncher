// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Core/ClassicGameMode.h"
#include "Core/ScreenManager.h"
#include "Data/DataManager.h"
#include "Data/XMLDateTime.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"


void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	FString DateStringValue = TEXT("Ola string");
	DateStringValue = TEXT("Ola strings");
	FDateTime DateTime = FDateTime::Now();
	FXMLDateTime XMLTime = TEXT("Olá");
	UE_LOG(LogTemp, Error, TEXT("time %s"), *XMLTime.ToString());
	XMLTime = TEXT("Olá Novamente");
	UE_LOG(LogTemp, Error, TEXT("recebeu TEXT() %s"), *XMLTime.ToString());
	XMLTime = DateStringValue;
	UE_LOG(LogTemp, Error, TEXT("recebeu como string %s"), *XMLTime.ToString());
	XMLTime = DateTime;
	UE_LOG(LogTemp, Error, TEXT("recebeu DateTime %s"), *XMLTime.ToString());
	*/
	
	
	//XMLTime.StringToDateTime(TEXT(""));
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



