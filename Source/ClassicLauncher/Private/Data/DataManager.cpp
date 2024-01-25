// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Data/DataManager.h"

#include "Audio/ClassicMediaPlayer.h"
#include "Core/ClassicGameInstance.h"
#include "Core/ClassicGameMode.h"
#include "Core/ScreenManager.h"
#include "Data/GameDataFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Layout/LoopScroll.h"
#include "UI/Layout/OptionsLayout.h"
#include "UI/Screens/LoadingScreen.h"
#include "UI/Screens/MainScreen.h"

#define LOCTEXT_NAMESPACE "ButtonsSelection"
#define DELAY 1.0f


UDataManager::UDataManager()
	: MainScreenReference(nullptr)
	  , LoadingScreenReference(nullptr)
	  , ClassicMediaPlayerReference(nullptr)
	  , ScreenManager(nullptr)
	  , GameDataStatus(EGameDataStatus::None)
	  , ConfigurationStatus(EConfigurationStatus::None)
	  , IndexGameData(0)
	  , IndexGameSystem(0)
{
}

void UDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ScreenManager = NewObject<UScreenManager>(this, UScreenManager::StaticClass(), FName("Data"));
}

void UDataManager::Deinitialize()
{
	Super::Deinitialize();
}

bool UDataManager::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return Super::DoesSupportWorldType(WorldType);
}

////////

void UDataManager::InitGameData()
{
	FTimerHandle DelayTimerHandle;
	if (GameDataStatus == EGameDataStatus::Done)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, ScreenManager, &UScreenManager::ShowMainScreenToViewPort, 0.1f, false, DELAY);
	}
	else if (GameDataStatus == EGameDataStatus::Failed)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UDataManager::CreateNewGameList, 0.1f, false, DELAY);
	}
	GameDataStatus = EGameDataStatus::None;
}

void UDataManager::LoadConfiguration()
{
	if (OnMessageShow.IsBound())
	{
		OnMessageShow.Broadcast((LOCTEXT("Loading", "Loading...")));
	}

	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UEasyXMLElement* ElementXML = UGameDataFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config"));
		UGameDataFunctionLibrary::SetConfig(ElementXML, ConfigurationData);
		UGameplayStatics::SetEnableWorldRendering(this, ConfigurationData.Rendering);
		ConfigurationData.PathMedia = (ConfigurationData.PathMedia != TEXT("")) ? ConfigurationData.PathMedia : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");
		GetMainScreenReference()->Options->SetSlide(ConfigurationData);
		LoadGameSystems();
		ConfigurationStatus = EConfigurationStatus::Done;
	}
	ConfigurationStatus = EConfigurationStatus::Failed;
}

void UDataManager::LoadGameSystems()
{
	GameSystems = GetClassicGameInstance()->GetGameSystemSave();
	GameDataStatus = (GameSystems.Num() > 0) ? EGameDataStatus::Done : EGameDataStatus::Failed;
	IndexGameSystem = 0;

	for (int32 i = 0; i < GameSystems.Num(); i++)
	{
		if (FString::FromInt(i) == ConfigurationData.DefaultStartSystem)
		{
			IndexGameSystem = i;
			UE_LOG(LogTemp, Warning, TEXT("%s ConfigurationData"), *ConfigurationData.DefaultStartSystem);
			break;
		}
	}
	if (GameDataStatus == EGameDataStatus::Done)
	{
		UGameDataFunctionLibrary::CreateFolders(ConfigurationData.PathMedia, GameSystems);
	}
}

void UDataManager::CreateNewGameList()
{
	AsyncTask(ENamedThreads::AnyThread, [this]()
	{
		GameSystems.Empty();

		FString ConfigSysXMLFile;
		const FString PathConfigSys = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\configsys.xml");

		if (UClassicFunctionLibrary::LoadStringFile(ConfigSysXMLFile, PathConfigSys))
		{
			// check if is empty to avoid error in UEasyXMLElement*
			if (ConfigSysXMLFile.IsEmpty())
			{
				AsyncTask(ENamedThreads::GameThread, [this]()
				{
					if (OnMessageShow.IsBound())
					{
						OnMessageShow.Broadcast(LOCTEXT("configsysempty", "configsys.xml is Empty"));
					}
				});
				return;
			}

			TArray<FGameSystem> TempGameSystems;
			TArray<UEasyXMLElement*> Elements = UGameDataFunctionLibrary::LoadXML(ConfigSysXMLFile, TEXT("config.system"));
			UGameDataFunctionLibrary::SetGameSystem(Elements, TempGameSystems);
			UGameDataFunctionLibrary::SortConfigSystem(TempGameSystems);

			GameSystems.Add(UGameDataFunctionLibrary::SetSystemToGameData(TempGameSystems));

			for (int32 i = 0; i < TempGameSystems.Num(); i++)
			{
				const FString PathGameList = TempGameSystems[i].RomPath + TEXT("\\gamelist.xml");
				if (UClassicFunctionLibrary::LoadStringFile(ConfigSysXMLFile, PathGameList))
				{
					if (ConfigSysXMLFile.IsEmpty()) continue; // check if is empty to avoid error in UEasyXMLElement*
					FString Label = TempGameSystems[i].SystemLabel;
					AsyncTask(ENamedThreads::GameThread, [this,i,Label]()
					{
						if (OnMessageShow.IsBound())
						{
							const FText Loading = FText::Format(LOCTEXT("AddSystem", "Add System {0}"), FText::FromString(Label));
							OnMessageShow.Broadcast(Loading);
						}
					});

					Elements = UGameDataFunctionLibrary::LoadXML(ConfigSysXMLFile, TEXT("gameList.game"));
					UGameDataFunctionLibrary::SetGameData(Elements, TempGameSystems[i].GameDatas, nullptr);
					if (TempGameSystems[i].GameDatas.Num() > 0)
					{
						GameSystems.Add(TempGameSystems[i]);
					}
				}
			}
			if (TempGameSystems.Num() == 0)
			{
				AsyncTask(ENamedThreads::GameThread, [this]()
				{
					if (OnMessageShow.IsBound())
					{
						OnMessageShow.Broadcast(LOCTEXT("sysnotfound", "Systems not found"));
					}
				});
				return;
			}
			TempGameSystems.Empty();
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				if (OnMessageShow.IsBound())
				{
					OnMessageShow.Broadcast(LOCTEXT("configsysnotfound", "configsys.xml not found"));
				}
			});
			return;
		}
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			PrepareToSaveNewGameList();
		});
	});
}

void UDataManager::PrepareToSaveNewGameList()
{
	AsyncTask(ENamedThreads::AnyThread, [this]()
	{
		for (int32 i = 1; i < GameSystems.Num(); i++)
		{
			FString GameResult;
			FString GameRoot = GameSystems[i].RomPath + TEXT("\\gamelist.xml");

			UGameDataFunctionLibrary::SortGameData(GameSystems[i].GameDatas, true);
			UGameDataFunctionLibrary::FormatGameData(GameSystems[i], ConfigurationData);
			AsyncTask(ENamedThreads::GameThread, [this,i]()
			{
				if (OnMessageShow.IsBound())
				{
					const FText Loading = FText::Format(LOCTEXT("loading", "Loading {0}"), FText::FromString(GameSystems[i].SystemLabel));
					OnMessageShow.Broadcast(Loading);
				}
			});
		}

		if (GetClassicGameInstance()->SaveGameSystem(GameSystems))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved"));
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				if (OnMessageShow.IsBound())
				{
					OnMessageShow.Broadcast(LOCTEXT("LogSuccessfullyGameList", "Game list update successfully wait..."));
				}
				GameDataStatus = EGameDataStatus::Done;
				InitGameData();
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				if (OnMessageShow.IsBound())
				{
					OnMessageShow.Broadcast(LOCTEXT("LogErrorGameList", "Game list not save"));
				}
			});
		}
	});
}

void UDataManager::LoadGamesList()
{
	AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [&]()
	{
		GameData = UGameDataFunctionLibrary::FilterGameData(GetGameSystem().GameDatas, GameSystems[IndexGameSystem].Positions.OrderBy);

		if (GameData.Num() > 0)
		{
			ConfigurationData.DefaultStartSystem = FString::FromInt(IndexGameSystem);
			UGameDataFunctionLibrary::SaveConfigurationData(ConfigurationData);
			GetClassicGameInstance()->SaveGameSystem(GameSystems);
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				if (OnDoneGameListNative.IsBound())
				{
					OnDoneGameListNative.Broadcast();
				}
			});
		}
	});
}

//////////////////////////////////////////////////////////


UScreenManager* UDataManager::GetScreenManager() const
{
	return ScreenManager;
}

AClassicGameMode* UDataManager::GetGameMode() const
{
	return Cast<AClassicGameMode>(UGameplayStatics::GetGameMode(this));
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
	UGameDataFunctionLibrary::SaveGameListXML(Path, GameSystems[IndexGameSystem].GameDatas);
	return GetClassicGameInstance()->SaveGameSystem(GameSystems);
}

void UDataManager::SetCountPlayerToSave()
{
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this]
	{
		int32 Find;
		const FString CurrentDateNow = UClassicFunctionLibrary::FormatDateToXml();
		if (UGameDataFunctionLibrary::FindGameData(GetGameSystem().GameDatas, GetGameData(), Find))
		{
			GameSystems[IndexGameSystem].GameDatas[Find].playcount++;
			GameSystems[IndexGameSystem].GameDatas[Find].lastplayed = CurrentDateNow;
			GameData[IndexGameData].playcount++;
			GameData[IndexGameData].lastplayed = CurrentDateNow;
			Save();
		}
	});
}


void UDataManager::SetFavoriteToSave()
{
	if (!GetGameSystem().SystemName.Equals(TEXT("${System}")))
	{
		AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this]()
		{
			int32 Find;
			if (UGameDataFunctionLibrary::FindGameData(GetGameSystem().GameDatas, GetGameData(), Find))
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


#undef LOCTEXT_NAMESPACE
