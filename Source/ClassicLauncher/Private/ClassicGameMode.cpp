// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicGameMode.h"
#include "LoadingGameData.h"
#include "GameFramework/GameUserSettings.h"


void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AClassicGameMode::Init()
{
	LoadingGameData = NewObject<ULoadingGameData>(this, ULoadingGameData::StaticClass(), FName("Data"));
	LoadingGameData->Init(MainInterfaceClass, LoadingScreenClass);
	LoadingGameData->CreateWidgets();
	LoadingGameData->AddLoadingScreenToViewPort();
	LoadingGameData->LoadConfiguration();
	GameSettingsInit();
}

void AClassicGameMode::GameSettingsInit()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(60.0f);
	Settings->SetVSyncEnabled(true);
	Settings->SetResolutionScaleValueEx(25);
	Settings->SetViewDistanceQuality(0);
	Settings->SetAntiAliasingQuality(0);
	Settings->SetPostProcessingQuality(0);
	Settings->SetShadowQuality(0);
	Settings->SetGlobalIlluminationQuality(0);
	Settings->SetReflectionQuality(0);
	Settings->SetTextureQuality(0);
	Settings->SetVisualEffectQuality(0);
	Settings->SetFoliageQuality(0);
	Settings->SetShadingQuality(0);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}

void AClassicGameMode::GameSettingsRunningInternal()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(60.0f);
	Settings->SetResolutionScaleValueEx(100);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}

void AClassicGameMode::GameSettingsRunning()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(1.0f);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}