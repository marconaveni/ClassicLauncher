// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Data/DataManager.h"

#include "Core/ClassicGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Layout/LoopScroll.h"
#include "UI/Screens/LoadingScreen.h"
#include "UI/Screens/MainScreen.h"

UDataManager::UDataManager()
	: MainScreenReference(nullptr)
	  , LoadingScreenReference(nullptr)
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

FGameData UDataManager::GetGameData(const int32 Index) const
{
	const int32 Id = (Index != -1) ? Index : IndexGameData;
	if (GameData.IsValidIndex(Id))
	{
		return GameData[Id];
	}
	return FGameData();
}

FGameSystem UDataManager::GetGameSystem(const int32 Index) const
{
	const int32 Id = (Index != -1) ? Index : IndexGameSystem;
	if (GameSystems.IsValidIndex(Id))
	{
		return GameSystems[Id];
	}
	return FGameSystem();
}

FGameData UDataManager::GetGameSystemGameData(const int32 Index) const
{
	const int32 Id = (Index != -1) ? Index : IndexGameData;
	if (GameSystems[IndexGameSystem].GameDatas.IsValidIndex(Id))
	{
		return GameSystems[IndexGameSystem].GameDatas[Id];
	}
	return FGameData();
}

bool UDataManager::Save()
{
	FString Path = GetGameSystem().RomPath;
	UClassicFunctionLibrary::SaveGameListXML(Path, GameSystems[IndexGameSystem].GameDatas);
	return GetClassicGameInstance()->SaveGameSystem(GameSystems);
}

void UDataManager::SetCountPlayerToSave()
{
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this]
	{
		int32 Find;
		const FString CurrentDateNow = UClassicFunctionLibrary::FormatDateToXml();
		if (UClassicFunctionLibrary::FindGameData(GetGameSystem().GameDatas, GetGameData(), Find))
		{
			GameSystems[IndexGameSystem].GameDatas[Find].playcount++;
			GameSystems[IndexGameSystem].GameDatas[Find].lastplayed = CurrentDateNow;
			GameData[IndexGameData].playcount++;
			GameData[IndexGameData].lastplayed = CurrentDateNow;
			Save();
		}
	});
}

#define LOCTEXT_NAMESPACE "ButtonsSelection"

void UDataManager::SetFavoriteToSave()
{
	if (!GetGameSystem().SystemName.Equals(TEXT("${System}")))
	{
		AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this]()
		{
			int32 Find;
			if (UClassicFunctionLibrary::FindGameData(GetGameSystem().GameDatas, GetGameData(), Find))
			{
				const bool ToggleFavorite = !GetGameSystem().GameDatas[Find].favorite;
				GameSystems[IndexGameSystem].GameDatas[Find].favorite = ToggleFavorite;
				GameData[IndexGameData].favorite = ToggleFavorite;

				if (Save())
				{
					AsyncTask(ENamedThreads::GameThread, [this, ToggleFavorite]()
					{
						GetLoopScrollReference()->SetCardFavorite(ToggleFavorite);
						GetMainScreenReference()->SetButtonsIconInterfaces(EPositionY::CENTER);
						GetMainScreenReference()->ShowMessage((ToggleFavorite) ? LOCTEXT("MessageAddFavorite", "Add game to favorite") : LOCTEXT("RemoveFavorite", "Remove game to favorite"), 3.5f);
					});
				}
			}
		});
	}
}

#undef LOCTEXT_NAMESPACE

void UDataManager::CreateWidgets(TSubclassOf<UMainScreen> MainScreenClass, TSubclassOf<class ULoadingScreen> LoadingScreenClass)
{
	if (MainScreenClass != nullptr && LoadingScreenClass != nullptr) // Check if the Asset is assigned.
	{
		// Create the widget and store it.
		SetMainScreenReference(CreateWidget<UMainScreen>(GetGameplayStatics(), MainScreenClass));
		SetLoadingScreenReference(CreateWidget<ULoadingScreen>(GetGameplayStatics(), LoadingScreenClass));

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

APlayerController* UDataManager::GetGameplayStatics() const
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

UClassicGameInstance* UDataManager::GetClassicGameInstance() const
{
	return Cast<UClassicGameInstance>(GetWorld()->GetGameInstance());;
}

AClassicMediaPlayer* UDataManager::GetClassicMediaPlayerReference() const
{
	if (ClassicMediaPlayerReference != nullptr)
	{
		return ClassicMediaPlayerReference;
	}
	return nullptr;
}

ULoadingScreen* UDataManager::GetLoadingScreenReference() const
{
	if (IsValid(LoadingScreenReference))
	{
		return LoadingScreenReference;
	}
	return nullptr;
}

UMainScreen* UDataManager::GetMainScreenReference() const
{
	if (IsValid(MainScreenReference))
	{
		return MainScreenReference;
	}
	return nullptr;
}

ULoopScroll* UDataManager::GetLoopScrollReference() const
{
	if (IsValid(MainScreenReference))
	{
		return MainScreenReference->LoopScroll;
	}
	return nullptr;
}

UFrame* UDataManager::GetFrameReference() const
{
	if (IsValid(MainScreenReference))
	{
		return MainScreenReference->Frame;
	}
	return nullptr;
}

void UDataManager::SetClassicMediaPlayerReference(AClassicMediaPlayer* ClassicMediaPlayerRef)
{
	ClassicMediaPlayerReference = ClassicMediaPlayerRef;
}

void UDataManager::SetLoadingScreenReference(ULoadingScreen* LoadingScreenRef)
{
	LoadingScreenReference = LoadingScreenRef;
}

void UDataManager::SetMainScreenReference(UMainScreen* MainScreenRef)
{
	MainScreenReference = MainScreenRef;
}
