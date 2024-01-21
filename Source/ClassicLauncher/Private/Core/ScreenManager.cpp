// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Core/ScreenManager.h"

#include "UI/Components/ButtonCommon.h"
#include "UI/Layout/OptionsLayout.h"
#include "Core/ClassicGameinstance.h"
#include "Core/ClassicSaveGame.h"
#include "Audio/ClassicMediaPlayer.h"
#include "UI/Components/ButtonSlide.h"
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

void UScreenManager::Init(TSubclassOf<UMainScreen> MainInterface, TSubclassOf<class ULoadingScreen> LoadingScreen)
{

	DataManager = GetWorld()->GetSubsystem<UDataManager>();
	DataManager->MainScreenClass = MainInterface;
	DataManager->LoadingScreenClass = LoadingScreen;
	DataManager->CreateWidgets();
	
	/*
	MainInterfaceClass = MainInterface;
	LoadingScreenClass = LoadingScreen;*/
	/*GameplayStatics = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ClassicGameInstance = Cast<UClassicGameInstance>(GetWorld()->GetGameInstance());*/
	MessageShow.AddDynamic(this, &UScreenManager::Message);
}

/*void UScreenManager::CreateWidgets()
{
	/*if (MainInterfaceClass != nullptr && LoadingScreenClass != nullptr) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		MainInterfaceReference = CreateWidget<UMainScreen>(GameplayStatics, MainInterfaceClass);
		LoadingScreenReference = CreateWidget<ULoadingScreen>(GameplayStatics, LoadingScreenClass);

		if (MainInterfaceReference != nullptr)
		{
			//let add it to the view port
			MainInterfaceReference->AddToViewport(0);
		}
		if (LoadingScreenReference != nullptr)
		{
			LoadingScreenReference->AddToViewport(1);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceClass or LoadingScreenClass the Assets is not assigned"));
	}#1#
}*/

void UScreenManager::LoadConfiguration()
{
	DataManager->LoadingScreenReference->ShowMessage(FText::FromString(TEXT("Loading Configurations...")));

	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), DataManager->ConfigurationData);
		UGameplayStatics::SetEnableWorldRendering(this, DataManager->ConfigurationData.Rendering);
		DataManager->ConfigurationData.PathMedia = (DataManager->ConfigurationData.PathMedia != TEXT("")) ? DataManager->ConfigurationData.PathMedia : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");
		DataManager->MainScreenReference->Options->SetSlide( DataManager->ConfigurationData);
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::LoadGameSystems, 0.1f, false, DELAY);
	}
	DataManager->MainScreenReference->ClassicMediaPlayerReference->SetMusics(TEXT(""));
	DataManager->MainScreenReference->ClassicMediaPlayerReference->PlaylistMusic(false);
}

void UScreenManager::LoadGameSystems()
{
	DataManager->GameSystems = DataManager->ClassicGameInstance->GetGameSystemSave();
	UClassicFunctionLibrary::CreateFolders(DataManager->ConfigurationData.PathMedia, DataManager->GameSystems);

	DataManager->LoadingScreenReference->ShowMessage(LOCTEXT("Loading", "Loading Games Wait..."));
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

		SetMainInterfaceData();
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
	DataManager->MainScreenReference->WBPSystemsList->ScrollBox->ClearAllChildrenContent();
	DataManager->MainScreenReference->GameSystems.Empty();
	DataManager->MainScreenReference->ButtonSystemReferences.Empty();

	if (DataManager->MainScreenReference->ButtonSystemClass != nullptr)
	{
		UButtonCommon* ButtonSystem = nullptr;
		
		for (int32 i = 0; i < DataManager->GameSystems.Num(); i++)
		{
			ButtonSystem = CreateWidget<UButtonCommon>(DataManager->MainScreenReference->GetOwningPlayer(), DataManager->MainScreenReference->ButtonSystemClass);
			ButtonSystem->OnClickTrigger.AddDynamic(DataManager->MainScreenReference , &UMainScreen::OnClickSystem);
			ButtonSystem->SetText((i == 0) ? LOCTEXT("Systems", "Show Systems") : FText::FromString(DataManager->GameSystems[i].SystemLabel));
			ButtonSystem->SetIndex(i);
			DataManager->MainScreenReference->ButtonSystemReferences.Add(ButtonSystem);
			DataManager->MainScreenReference->WBPSystemsList->ScrollBox->SetContent(ButtonSystem);
		}
		DataManager->MainScreenReference->GameSystems = DataManager->GameSystems;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("buttonSystemClass Not Found"));
	}
}

void UScreenManager::SetMainInterfaceData() const
{
	DataManager->MainScreenReference->CountSystem = DataManager->IndexGameSystem;
	DataManager->MainScreenReference->ConfigurationData = DataManager->ConfigurationData;
	DataManager->MainScreenReference->ClassicGameInstance = DataManager->ClassicGameInstance;
}

void UScreenManager::CreateNewGameList()
{
	DataManager->GameSystems.Empty();

	FString ConfigResult;
	FString ConfigRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\configsys.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, ConfigRoot))
	{
		// check if is empty to avoid error in UEasyXMLElement*
		if (ConfigResult.IsEmpty())
		{
			MessageShow.Broadcast(LOCTEXT("configsysempty", "configsys.xml is Empty"));
			return;
		}

		TArray<FGameSystem> TempSystems;
		UClassicFunctionLibrary::SetGameSystem(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("config.system")), TempSystems);

		TempSystems = UClassicFunctionLibrary::SortConfigSystem(TempSystems);
		DataManager->GameSystems.Add(UClassicFunctionLibrary::SetSystemToGameData(TempSystems));

		for (int32 i = 0; i < TempSystems.Num(); i++)
		{
			ConfigRoot = TempSystems[i].RomPath + TEXT("\\gamelist.xml");
			if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, ConfigRoot))
			{
				if (ConfigResult.IsEmpty()) continue; // check if is empty to avoid error in UEasyXMLElement*

				UClassicFunctionLibrary::SetGameData(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("gameList.game")), TempSystems[i].GameDatas, nullptr);
				if (TempSystems[i].GameDatas.Num() > 0)
				{
					DataManager->GameSystems.Add(TempSystems[i]);
				}
			}
		}
		if (TempSystems.Num() == 0)
		{
			MessageShow.Broadcast(LOCTEXT("sysnotfound", "Systems not found"));
			return;
		}
		TempSystems.Empty();
	}
	else
	{
		MessageShow.Broadcast(LOCTEXT("configsysnotfound", "configsys.xml not found"));
		return;
	}

	PrepareToSaveNewGameList();
}

void UScreenManager::PrepareToSaveNewGameList()
{
	AsyncTask(ENamedThreads::AnyThread, [this]()
	{

		for (int32 i = 1; i < DataManager->GameSystems.Num(); i++)
		{
			FString GameResult;
			FString GameRoot = DataManager->GameSystems[i].RomPath + TEXT("\\gamelist.xml");

			UClassicFunctionLibrary::SortGameDate(DataManager->GameSystems[i].GameDatas);
			UClassicFunctionLibrary::FormatGameData(DataManager->GameSystems[i].GameDatas, DataManager->ConfigurationData, DataManager->GameSystems[i]);
			AsyncTask(ENamedThreads::GameThread, [this,i]()
			{
				const FText Loading = FText::Format(LOCTEXT("loading", "Loading {0}") , FText::FromString(DataManager->GameSystems[i].SystemLabel));
				MessageShow.Broadcast(Loading);
			});

		}
		
		if (DataManager->ClassicGameInstance->SaveGameSystem(DataManager->GameSystems))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved"));
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				//MainInterfaceReference->RemoveFromParent();
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

	if (DataManager->MainScreenReference != nullptr)
	{
		DataManager->MainScreenReference->SetVisibility(ESlateVisibility::Visible);
		//Loading Game list in main interface
		DataManager->MainScreenReference->LoadGamesList();
		//Show the Cursor.
		DataManager->GameplayStatics->bShowMouseCursor = true;

		//Input mode settings. 
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		//set input mode
		DataManager->GameplayStatics->SetInputMode(InputMode);

		//Mouse focus mode 
		UGameplayStatics::SetViewportMouseCaptureMode(DataManager->GameplayStatics, EMouseCaptureMode::NoCapture);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference Not Found or Null"));
	}
}

void UScreenManager::AddLoadingScreenToViewPort()
{
	if (DataManager->LoadingScreenReference != nullptr)
	{
		DataManager->LoadingScreenReference->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingScreenReference Not Found or Null"));
	}
}

void UScreenManager::RemoveLoadingScreenToParent()
{
	if (DataManager->LoadingScreenReference != nullptr)
	{
		DataManager->LoadingScreenReference->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UScreenManager::SetToRestartWidgets()
{
	DataManager->MainScreenReference->Clear();
	DataManager->MainScreenReference->SetPlayAnimation(TEXT("AnimationShowConfigurationReverse"));
	//MainInterfaceReference->SetPlayAnimation(TEXT("LoadListGameReverse"));
	DataManager->MainScreenReference->Header->SetFocusButton();
	DataManager->MainScreenReference->Frame->SetFramePositionWithoutAnimation(1);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::RestartWidgets, 0.1f, false, DELAY + 1.0f);
	AddLoadingScreenToViewPort();
	const FText Message = LOCTEXT("UpdateGame", "Update game wait");
	DataManager->LoadingScreenReference->ShowMessage(Message);
}

void UScreenManager::RestartWidgets()
{
	CreateNewGameList();
}

void UScreenManager::Message(FText Message)
{
	DataManager->LoadingScreenReference->ShowMessage(Message);
}

#undef LOCTEXT_NAMESPACE
