// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoadingGameData.h"

#include "ClassicConfigurations.h"
#include "ClassicGameinstance.h"
#include "ClassicMediaPlayer.h"
#include "ClassicSaveGame.h"
#include "ClassicSlide.h"
#include "Frame.h"
#include "MainInterface.h"
#include "LoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#define DELAY 1.0f
#define LOCTEXT_NAMESPACE "ButtonsSelection"

void ULoadingGameData::Init(TSubclassOf<UMainInterface> MainInterface, TSubclassOf<class ULoadingScreen> LoadingScreen)
{
	MainInterfaceClass = MainInterface;
	LoadingScreenClass = LoadingScreen;
	GameplayStatics = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ClassicGameInstance = Cast<UClassicGameInstance>(GetWorld()->GetGameInstance());
	MessageShow.AddDynamic(this, &ULoadingGameData::Message);
}

void ULoadingGameData::CreateWidgets()
{
	if (MainInterfaceClass != nullptr && LoadingScreenClass != nullptr) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		MainInterfaceReference = CreateWidget<UMainInterface>(GameplayStatics, MainInterfaceClass);
		LoadingScreenReference = CreateWidget<ULoadingScreen>(GameplayStatics, LoadingScreenClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceClass or LoadingScreenClass the Assets is not assigned"));
	}
}

void ULoadingGameData::LoadConfiguration()
{
	LoadingScreenReference->ShowMessage(FText::FromString(TEXT("Loading Configurations...")));

	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), ConfigurationData);
		UGameplayStatics::SetEnableWorldRendering(this, ConfigurationData.Rendering);
		ConfigurationData.PathMedia = (ConfigurationData.PathMedia != TEXT("")) ? ConfigurationData.PathMedia : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");

		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ULoadingGameData::LoadGameSystems, 0.1f, false, DELAY);
	}
	MainInterfaceReference->ClassicMediaPlayerReference->SetMusics(TEXT(""));
	MainInterfaceReference->ClassicMediaPlayerReference->PlaylistMusic(false);
}

void ULoadingGameData::LoadGameSystems()
{
	Systems = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave;
	UClassicFunctionLibrary::CreateFolders(ConfigurationData.PathMedia, Systems);

	LoadingScreenReference->ShowMessage(LOCTEXT("Loading", "Loading Games Wait..."));
	if (Systems.Num() > 0)
	{
		MainInterfaceReference->AddSystems(Systems);

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
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ULoadingGameData::AddMainInterfaceToViewPort, 0.1f, false, DELAY);
	}
	else
	{
		//create new list game and save GameSystems internal
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ULoadingGameData::CreateNewGameList, 0.1f, false, DELAY);
	}
}

void ULoadingGameData::SetMainInterfaceData() const
{
	MainInterfaceReference->CountSystem = CountSystem;
	MainInterfaceReference->CountLocationY = CountSystem;
	MainInterfaceReference->ConfigurationData = ConfigurationData;
	MainInterfaceReference->ClassicGameInstance = ClassicGameInstance;
	MainInterfaceReference->WBPClassicConfigurationsInterface->SlideVolume->SetSlideValue(FMath::Clamp(ConfigurationData.Volume, 0, 100));
}

void ULoadingGameData::CreateNewGameList()
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

void ULoadingGameData::PrepareToSaveNewGameList()
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
				MainInterfaceReference->RemoveFromParent();
				MessageShow.Broadcast(LOCTEXT("LogSuccessfullyGameList", "Game list update successfully wait..."));
				GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ULoadingGameData::LoadGameSystems, 0.1f, false, DELAY + 3.0f);
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

void ULoadingGameData::AddMainInterfaceToViewPort()
{

	if (MainInterfaceReference != nullptr)
	{
		//let add it to the view port
		MainInterfaceReference->AddToViewport(0);
		//Loading Game list in main interface
		MainInterfaceReference->LoadGamesList();
		//Show the Cursor.
		GameplayStatics->bShowMouseCursor = false;

		//Input mode settings. 
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		//set input mode
		GameplayStatics->SetInputMode(InputMode);

		//Mouse focus mode 
		UGameplayStatics::SetViewportMouseCaptureMode(GameplayStatics, EMouseCaptureMode::CaptureDuringRightMouseDown);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference Not Found or Null"));
	}
}

void ULoadingGameData::AddLoadingScreenToViewPort()
{
	if (LoadingScreenReference != nullptr)
	{
		LoadingScreenReference->AddToViewport(1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingScreenReference Not Found or Null"));
	}
}

void ULoadingGameData::RemoveLoadingScreenToParent()
{
	if (LoadingScreenReference != nullptr)
	{
		LoadingScreenReference->RemoveFromParent();
	}
}

void ULoadingGameData::SetToRestartWidgets()
{
	MainInterfaceReference->Clear();
	MainInterfaceReference->PlayAnimationReverse(MainInterfaceReference->AnimationShowConfiguration);
	MainInterfaceReference->PlayAnimationReverse(MainInterfaceReference->LoadListGame);
	MainInterfaceReference->PlayAnimationReverse(MainInterfaceReference->BarTop);
	MainInterfaceReference->WBPFrame->SetFrameCenterPosition(1);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ULoadingGameData::RestartWidgets, 0.1f, false, DELAY + 1.0f);
	AddLoadingScreenToViewPort();
	const FText Message = LOCTEXT("UpdateGame", "Update game wait");
	LoadingScreenReference->ShowMessage(Message);
}

void ULoadingGameData::RestartWidgets()
{
	CreateNewGameList();
}

void ULoadingGameData::Message(FText Message)
{
	LoadingScreenReference->ShowMessage(Message);
}

#undef LOCTEXT_NAMESPACE
