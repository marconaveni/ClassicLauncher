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
	  /*, LoopScrollReference(nullptr)
	  , FrameReference(nullptr)*/
	  , ClassicMediaPlayerReference(nullptr)
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

UClassicGameInstance* UDataManager::GetClassicGameInstance() const
{
	return ClassicGameInstance;
}

AClassicMediaPlayer* UDataManager::GetClassicMediaPlayerReference() const
{
	return ClassicMediaPlayerReference;
}

ULoadingScreen* UDataManager::GetLoadingScreenReference() const
{
	return LoadingScreenReference;
}

UMainScreen* UDataManager::GetMainScreenReference() const
{
	return MainScreenReference;
}

ULoopScroll* UDataManager::GetLoopScrollReference() const
{
	return MainScreenReference->LoopScroll;
}

UFrame* UDataManager::GetFrameReference() const
{
	return MainScreenReference->Frame;
}

void UDataManager::SetClassicGameInstance(UClassicGameInstance*& ClassicGameInstanceRef)
{
	ClassicGameInstance = ClassicGameInstanceRef;
}

void UDataManager::SetClassicMediaPlayerReference(AClassicMediaPlayer*& ClassicMediaPlayerRef)
{
	ClassicMediaPlayerReference = ClassicMediaPlayerRef;
}

void UDataManager::SetLoadingScreenReference(ULoadingScreen*& LoadingScreenRef)
{
	LoadingScreenReference = LoadingScreenRef;
}

void UDataManager::SetMainScreenReference(UMainScreen*& MainScreenRef)
{
	MainScreenReference = MainScreenRef;
}

void UDataManager::SetLoopScrollReference(ULoopScroll*& LoopScrollRef)
{
	MainScreenReference->LoopScroll = LoopScrollRef;
}

void UDataManager::SetFrameReference(UFrame*& FrameReferenceRef)
{
	MainScreenReference->Frame = FrameReferenceRef;
}
