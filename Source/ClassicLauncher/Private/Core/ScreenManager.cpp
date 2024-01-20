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
#include "Kismet/GameplayStatics.h"
#include "UI/Layout/Header.h"
#include "UI/Components/ScrollBoxEnhanced.h"

#define DELAY 1.0f
#define LOCTEXT_NAMESPACE "ButtonsSelection"

void UScreenManager::Init(TSubclassOf<UMainScreen> MainInterface, TSubclassOf<class ULoadingScreen> LoadingScreen)
{
	MainInterfaceClass = MainInterface;
	LoadingScreenClass = LoadingScreen;
	GameplayStatics = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ClassicGameInstance = Cast<UClassicGameInstance>(GetWorld()->GetGameInstance());
	MessageShow.AddDynamic(this, &UScreenManager::Message);
}

void UScreenManager::CreateWidgets()
{
	if (MainInterfaceClass != nullptr && LoadingScreenClass != nullptr) // Check if the Asset is assigned in the blueprint.
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
	}
}

void UScreenManager::LoadConfiguration()
{
	LoadingScreenReference->ShowMessage(FText::FromString(TEXT("Loading Configurations...")));

	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), ConfigurationData);
		UGameplayStatics::SetEnableWorldRendering(this, ConfigurationData.Rendering);
		ConfigurationData.PathMedia = (ConfigurationData.PathMedia != TEXT("")) ? ConfigurationData.PathMedia : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");

		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::LoadGameSystems, 0.1f, false, DELAY);
	}
	MainInterfaceReference->ClassicMediaPlayerReference->SetMusics(TEXT(""));
	MainInterfaceReference->ClassicMediaPlayerReference->PlaylistMusic(false);
}

void UScreenManager::LoadGameSystems()
{
	Systems = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave;
	UClassicFunctionLibrary::CreateFolders(ConfigurationData.PathMedia, Systems);

	LoadingScreenReference->ShowMessage(LOCTEXT("Loading", "Loading Games Wait..."));
	if (Systems.Num() > 0)
	{
		//MainInterfaceReference->AddSystems(Systems);
		AddSystems();

		for (int32 i = 0; i < Systems.Num(); i++)
		{
			if (FString::FromInt(i) == ConfigurationData.DefaultStartSystem)
			{
				CountSystem = i;
				UE_LOG(LogTemp, Warning, TEXT("%s ConfigurationData"), *ConfigurationData.DefaultStartSystem);
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
	MainInterfaceReference->WBPSystemsList->ScrollBox->ClearAllChildrenContent();
	MainInterfaceReference->GameSystems.Empty();
	MainInterfaceReference->ButtonSystemReferences.Empty();

	if (MainInterfaceReference->ButtonSystemClass != nullptr)
	{
		UButtonCommon* ButtonSystem = nullptr;
		
		for (int32 i = 0; i < Systems.Num(); i++)
		{
			ButtonSystem = CreateWidget<UButtonCommon>(MainInterfaceReference->GetOwningPlayer(), MainInterfaceReference->ButtonSystemClass);
			ButtonSystem->OnClickTrigger.AddDynamic(MainInterfaceReference , &UMainScreen::OnClickSystem);
			ButtonSystem->SetText((i == 0) ? LOCTEXT("Systems", "Show Systems") : FText::FromString(Systems[i].SystemLabel));
			ButtonSystem->SetIndex(i);
			/*ButtonSystem->SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);
			ButtonSystem->SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);*/
			MainInterfaceReference->ButtonSystemReferences.Add(ButtonSystem);
			MainInterfaceReference->WBPSystemsList->ScrollBox->SetContent(ButtonSystem);
		}
		MainInterfaceReference->GameSystems = Systems;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("buttonSystemClass Not Found"));
	}
}

void UScreenManager::SetMainInterfaceData() const
{
	MainInterfaceReference->CountSystem = CountSystem;
	MainInterfaceReference->CountLocationY = CountSystem;
	MainInterfaceReference->ConfigurationData = ConfigurationData;
	MainInterfaceReference->ClassicGameInstance = ClassicGameInstance;
	MainInterfaceReference->WBPClassicConfigurationsInterface->SlideVolumeSystem->SetSlideValue(FMath::Clamp(ConfigurationData.VolumeMaster, 0, 100));
	MainInterfaceReference->WBPClassicConfigurationsInterface->SlideVolumeMusic->SetSlideValue(FMath::Clamp(ConfigurationData.VolumeMusic, 0, 100));
	MainInterfaceReference->WBPClassicConfigurationsInterface->SlideVolumeVideo->SetSlideValue(FMath::Clamp(ConfigurationData.VolumeVideo, 0, 100));
}

void UScreenManager::CreateNewGameList()
{
	Systems.Empty();

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
		Systems.Add(UClassicFunctionLibrary::SetSystemToGameData(TempSystems));

		for (int32 i = 0; i < TempSystems.Num(); i++)
		{
			ConfigRoot = TempSystems[i].RomPath + TEXT("\\gamelist.xml");
			if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, ConfigRoot))
			{
				if (ConfigResult.IsEmpty()) continue; // check if is empty to avoid error in UEasyXMLElement*

				UClassicFunctionLibrary::SetGameData(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("gameList.game")), TempSystems[i].GameDatas, nullptr);
				if (TempSystems[i].GameDatas.Num() > 0)
				{
					Systems.Add(TempSystems[i]);
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

		for (int32 i = 1; i < Systems.Num(); i++)
		{
			FString GameResult;
			FString GameRoot = Systems[i].RomPath + TEXT("\\gamelist.xml");

			UClassicFunctionLibrary::SortGameDate(Systems[i].GameDatas);
			UClassicFunctionLibrary::FormatGameData(Systems[i].GameDatas, ConfigurationData, Systems[i]);
			AsyncTask(ENamedThreads::GameThread, [this,i]()
			{
				const FText Loading = FText::Format(LOCTEXT("loading", "Loading {0}") , FText::FromString(Systems[i].SystemLabel));
				MessageShow.Broadcast(Loading);
			});

		}
		ClassicGameInstance->SetSystemSave(Systems);
		if (UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0))
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

	if (MainInterfaceReference != nullptr)
	{
		MainInterfaceReference->SetVisibility(ESlateVisibility::Visible);
		//Loading Game list in main interface
		MainInterfaceReference->LoadGamesList();
		//Show the Cursor.
		GameplayStatics->bShowMouseCursor = true;

		//Input mode settings. 
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		//set input mode
		GameplayStatics->SetInputMode(InputMode);

		//Mouse focus mode 
		UGameplayStatics::SetViewportMouseCaptureMode(GameplayStatics, EMouseCaptureMode::NoCapture);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference Not Found or Null"));
	}
}

void UScreenManager::AddLoadingScreenToViewPort()
{
	if (LoadingScreenReference != nullptr)
	{
		LoadingScreenReference->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingScreenReference Not Found or Null"));
	}
}

void UScreenManager::RemoveLoadingScreenToParent()
{
	if (LoadingScreenReference != nullptr)
	{
		LoadingScreenReference->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UScreenManager::SetToRestartWidgets()
{
	MainInterfaceReference->Clear();
	MainInterfaceReference->SetPlayAnimation(TEXT("AnimationShowConfigurationReverse"));
	//MainInterfaceReference->SetPlayAnimation(TEXT("LoadListGameReverse"));
	MainInterfaceReference->Header->SetFocusButton();
	MainInterfaceReference->WBPFrame->SetFramePositionWithoutAnimation(1);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::RestartWidgets, 0.1f, false, DELAY + 1.0f);
	AddLoadingScreenToViewPort();
	const FText Message = LOCTEXT("UpdateGame", "Update game wait");
	LoadingScreenReference->ShowMessage(Message);
}

void UScreenManager::RestartWidgets()
{
	CreateNewGameList();
}

void UScreenManager::Message(FText Message)
{
	LoadingScreenReference->ShowMessage(Message);
}

#undef LOCTEXT_NAMESPACE
