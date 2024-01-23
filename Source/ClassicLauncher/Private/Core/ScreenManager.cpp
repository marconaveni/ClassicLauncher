// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Core/ScreenManager.h"

#include "UI/Components/ButtonCommon.h"
#include "UI/Layout/OptionsLayout.h"
#include "Core/ClassicGameinstance.h"
#include "Audio/ClassicMediaPlayer.h"
#include "UI/Layout/GameListLayout.h"
#include "UI/Layout/Frame.h"
#include "UI/Screens/MainScreen.h"
#include "UI/Screens/LoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Data/DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Layout/Header.h"
#include "UI/Components/ScrollBoxEnhanced.h"

#define DELAY 1.0f
#define LOCTEXT_NAMESPACE "ButtonsSelection"

void UScreenManager::Init(TSubclassOf<UMainScreen> MainScreenClass, TSubclassOf<class ULoadingScreen> LoadingScreenClass)
{
	DataManager = GetWorld()->GetSubsystem<UDataManager>();
	DataManager->CreateWidgets(MainScreenClass, LoadingScreenClass);
	MessageShow.AddDynamic(this, &UScreenManager::Message);
}

void UScreenManager::LoadConfiguration()
{
	DataManager->GetLoadingScreenReference()->ShowMessage(FText::FromString(TEXT("Loading Configurations...")));

	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), DataManager->ConfigurationData);
		UGameplayStatics::SetEnableWorldRendering(this, DataManager->ConfigurationData.Rendering);
		DataManager->ConfigurationData.PathMedia = (DataManager->ConfigurationData.PathMedia != TEXT(""))
			                                           ? DataManager->ConfigurationData.PathMedia
			                                           : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");
		DataManager->GetMainScreenReference()->Options->SetSlide(DataManager->ConfigurationData);
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::LoadGameSystems, 0.1f, false, DELAY);
	}
	DataManager->GetClassicMediaPlayerReference()->SetMusics(TEXT(""));
	DataManager->GetClassicMediaPlayerReference()->PlaylistMusic(false);
}

void UScreenManager::LoadGameSystems()
{
	DataManager->GameSystems = DataManager->GetClassicGameInstance()->GetGameSystemSave();
	UClassicFunctionLibrary::CreateFolders(DataManager->ConfigurationData.PathMedia, DataManager->GameSystems);

	DataManager->GetLoadingScreenReference()->ShowMessage(LOCTEXT("Loading", "Loading Games Wait..."));
	if (DataManager->GameSystems.Num() > 0)
	{
		AddSystems();

		for (int32 i = 0; i < DataManager->GameSystems.Num(); i++)
		{
			if (FString::FromInt(i) == DataManager->ConfigurationData.DefaultStartSystem)
			{
				DataManager->IndexGameSystem = i;
				UE_LOG(LogTemp, Warning, TEXT("%s ConfigurationData"), *DataManager->ConfigurationData.DefaultStartSystem);
				break;
			}
		}

		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::AddMainInterfaceToViewPort, 0.1f, false, DELAY);
	}
	else
	{
		//create new list game and save GameSystems internal
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::CreateNewGameList, 0.1f, false, DELAY);
	}
}

void UScreenManager::AddSystems()
{
	DataManager->GetMainScreenReference()->GameList->ScrollBox->ClearAllChildrenContent();
	DataManager->GetMainScreenReference()->ButtonSystemReferences.Empty();

	if (DataManager->GetMainScreenReference()->ButtonSystemClass != nullptr)
	{
		UButtonCommon* ButtonSystem = nullptr;

		for (int32 i = 0; i < DataManager->GameSystems.Num(); i++)
		{
			ButtonSystem = CreateWidget<UButtonCommon>(DataManager->GetMainScreenReference()->GetOwningPlayer(), DataManager->GetMainScreenReference()->ButtonSystemClass);
			ButtonSystem->OnClickTrigger.AddDynamic(DataManager->GetMainScreenReference(), &UMainScreen::OnClickSystem);
			ButtonSystem->SetText((i == 0) ? LOCTEXT("Systems", "Show Systems") : FText::FromString(DataManager->GameSystems[i].SystemLabel));
			ButtonSystem->SetIndex(i);
			DataManager->GetMainScreenReference()->ButtonSystemReferences.Add(ButtonSystem);
			DataManager->GetMainScreenReference()->GameList->ScrollBox->SetContent(ButtonSystem);
		}
		//DataManager->GameSystems = DataManager->GameSystems;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("buttonSystemClass Not Found"));
	}
}

void UScreenManager::CreateNewGameList()
{
	AsyncTask(ENamedThreads::AnyThread, [this]()
	{
		DataManager->GameSystems.Empty();

		FString ConfigSysXMLFile;
		const FString PathConfigSys = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\configsys.xml");

		if (UClassicFunctionLibrary::LoadStringFile(ConfigSysXMLFile, PathConfigSys))
		{
			// check if is empty to avoid error in UEasyXMLElement*
			if (ConfigSysXMLFile.IsEmpty())
			{
				AsyncTask(ENamedThreads::GameThread, [this]()
				{
					MessageShow.Broadcast(LOCTEXT("configsysempty", "configsys.xml is Empty"));
				});
				return;
			}

			TArray<FGameSystem> TempGameSystems;
			TArray<UEasyXMLElement*> Elements = UClassicFunctionLibrary::LoadXML(ConfigSysXMLFile, TEXT("config.system"));
			UClassicFunctionLibrary::SetGameSystem(Elements, TempGameSystems);
			UClassicFunctionLibrary::SortConfigSystem(TempGameSystems);

			DataManager->GameSystems.Add(UClassicFunctionLibrary::SetSystemToGameData(TempGameSystems));

			for (int32 i = 0; i < TempGameSystems.Num(); i++)
			{
				const FString PathGameList = TempGameSystems[i].RomPath + TEXT("\\gamelist.xml");
				if (UClassicFunctionLibrary::LoadStringFile(ConfigSysXMLFile, PathGameList))
				{
					if (ConfigSysXMLFile.IsEmpty()) continue; // check if is empty to avoid error in UEasyXMLElement*
					FString Label = TempGameSystems[i].SystemLabel;
					AsyncTask(ENamedThreads::GameThread, [this,i,Label]()
					{
						const FText Loading = FText::Format(LOCTEXT("AddSystem", "Add System {0}"), FText::FromString(Label));
						MessageShow.Broadcast(Loading);
					});

					Elements = UClassicFunctionLibrary::LoadXML(ConfigSysXMLFile, TEXT("gameList.game"));
					UClassicFunctionLibrary::SetGameData(Elements, TempGameSystems[i].GameDatas, nullptr);
					if (TempGameSystems[i].GameDatas.Num() > 0)
					{
						DataManager->GameSystems.Add(TempGameSystems[i]);
					}
				}
			}
			if (TempGameSystems.Num() == 0)
			{
				AsyncTask(ENamedThreads::GameThread, [this]()
				{
					MessageShow.Broadcast(LOCTEXT("sysnotfound", "Systems not found"));
				});
				return;
			}
			TempGameSystems.Empty();
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				MessageShow.Broadcast(LOCTEXT("configsysnotfound", "configsys.xml not found"));
			});
			return;
		}
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			PrepareToSaveNewGameList();
		});
	});
}

void UScreenManager::PrepareToSaveNewGameList()
{
	AsyncTask(ENamedThreads::AnyThread, [this]()
	{
		for (int32 i = 1; i < DataManager->GameSystems.Num(); i++)
		{
			FString GameResult;
			FString GameRoot = DataManager->GameSystems[i].RomPath + TEXT("\\gamelist.xml");

			UClassicFunctionLibrary::SortGameData(DataManager->GameSystems[i].GameDatas, false);
			UClassicFunctionLibrary::FormatGameData(DataManager->GameSystems[i], DataManager->ConfigurationData);
			AsyncTask(ENamedThreads::GameThread, [this,i]()
			{
				const FText Loading = FText::Format(LOCTEXT("loading", "Loading {0}"), FText::FromString(DataManager->GameSystems[i].SystemLabel));
				MessageShow.Broadcast(Loading);
			});
		}

		if (DataManager->GetClassicGameInstance()->SaveGameSystem(DataManager->GameSystems))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved"));
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				MessageShow.Broadcast(LOCTEXT("LogSuccessfullyGameList", "Game list update successfully wait..."));
				GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::LoadGameSystems, 0.1f, false, DELAY + 3.0f);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				MessageShow.Broadcast(LOCTEXT("LogErrorGameList", "Game list not save"));
			});
		}
	});
}

void UScreenManager::AddMainInterfaceToViewPort()
{
	if (DataManager->GetMainScreenReference() != nullptr)
	{
		DataManager->GetMainScreenReference()->SetVisibility(ESlateVisibility::Visible);
		//Loading Game list in main interface
		DataManager->GetMainScreenReference()->LoadGamesList();
		//Show the Cursor.
		DataManager->GetGameplayStatics()->bShowMouseCursor = true;

		//Input mode settings. 
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		//set input mode
		DataManager->GetGameplayStatics()->SetInputMode(InputMode);

		//Mouse focus mode 
		UGameplayStatics::SetViewportMouseCaptureMode(DataManager->GetGameplayStatics(), EMouseCaptureMode::NoCapture);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference Not Found or Null"));
	}
}

void UScreenManager::AddLoadingScreenToViewPort()
{
	if (DataManager->GetLoadingScreenReference() != nullptr)
	{
		DataManager->GetLoadingScreenReference()->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingScreenReference Not Found or Null"));
	}
}

void UScreenManager::RemoveLoadingScreenToParent()
{
	if (DataManager->GetLoadingScreenReference() != nullptr)
	{
		DataManager->GetLoadingScreenReference()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UScreenManager::SetToRestartWidgets()
{
	DataManager->GetMainScreenReference()->Clear();
	DataManager->GetMainScreenReference()->SetPlayAnimation(TEXT("AnimationShowConfigurationReverse"));
	//MainInterfaceReference->SetPlayAnimation(TEXT("LoadListGameReverse"));
	DataManager->GetMainScreenReference()->Header->SetFocusButton();
	DataManager->GetMainScreenReference()->Frame->SetFramePositionWithoutAnimation(1);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::RestartWidgets, 0.1f, false, DELAY + 1.0f);
	AddLoadingScreenToViewPort();
	const FText Message = LOCTEXT("UpdateGame", "Update game wait");
	DataManager->GetLoadingScreenReference()->ShowMessage(Message);
}

void UScreenManager::RestartWidgets()
{
	CreateNewGameList();
}

void UScreenManager::Message(FText Message)
{
	DataManager->GetLoadingScreenReference()->ShowMessage(Message);
}

#undef LOCTEXT_NAMESPACE
