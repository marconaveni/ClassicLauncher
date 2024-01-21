// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Data/DataManager.h"

#include "Core/ClassicGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Screens/LoadingScreen.h"
#include "UI/Screens/MainScreen.h"

UDataManager::UDataManager()
	: GameplayStatics(nullptr)
	  , ClassicGameInstance(nullptr)
	  , MainScreenReference(nullptr)
	  , LoadingScreenReference(nullptr)
	  , LoopScrollReference(nullptr)
	  , FrameReference(nullptr)
	  , IndexGameData(0)
	  , IndexGameSystem(0)
{
}

void UDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDataManager::Deinitialize()
{
	Super::Deinitialize();
}

bool UDataManager::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return Super::DoesSupportWorldType(WorldType);
}

void UDataManager::CreateWidgets()
{
	GameplayStatics = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ClassicGameInstance = Cast<UClassicGameInstance>(GetWorld()->GetGameInstance());
	
	if (MainScreenClass != nullptr && LoadingScreenClass != nullptr) // Check if the Asset is assigned.
	{
		// Create the widget and store it.
		MainScreenReference = CreateWidget<UMainScreen>(GameplayStatics, MainScreenClass);
		LoadingScreenReference = CreateWidget<ULoadingScreen>(GameplayStatics, LoadingScreenClass);

		if (MainScreenReference != nullptr)
		{
			//add it to the view port
			MainScreenReference->AddToViewport(0);
		}
		if (LoadingScreenReference != nullptr)
		{
			//add it to the view port
			LoadingScreenReference->AddToViewport(1);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceClass or LoadingScreenClass the Assets is not assigned"));
	}
}
