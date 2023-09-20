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

void ULoadingGameData::Init(TSubclassOf<UMainInterface> MainInterface, TSubclassOf<class ULoadingScreen> LoadingScreen)
{
	MainInterfaceClass = MainInterface;
	LoadingScreenClass = LoadingScreen;
	GameplayStatics = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ClassicGameInstance = Cast<UClassicGameInstance>(GetWorld()->GetGameInstance());
}

void ULoadingGameData::CreateWidgets()
{
	if (MainInterfaceClass != nullptr && LoadingScreenClass != nullptr) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		MainInterfaceReference = CreateWidget<UMainInterface>(GameplayStatics, MainInterfaceClass);
		LoadingScreenReference = CreateWidget<ULoadingScreen>(GameplayStatics, LoadingScreenClass);

		//MainInterfaceReference->Loaded.AddDynamic(this, &ULoadingGameData::AddMainInterfaceToViewPort);
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
}

void ULoadingGameData::LoadGameSystems()
{
	Systems = ClassicGameInstance->GetSystemSave();
	UClassicFunctionLibrary::CreateFolders(ConfigurationData.PathMedia, Systems);

	LoadingScreenReference->ShowMessage(FText::FromString(TEXT("Loading Systems...")));
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

void ULoadingGameData::SetMainInterfaceData()
{
	MainInterfaceReference->CountSystem = CountSystem;
	MainInterfaceReference->CountLocationY = CountSystem;
	MainInterfaceReference->ConfigurationData = ConfigurationData;
	MainInterfaceReference->ClassicGameInstance = ClassicGameInstance;
	MainInterfaceReference->WBPClassicConfigurationsInterface->SlideVolume->SetSlideValue(FMath::Clamp(ConfigurationData.Volume, 0, 100));
}


void ULoadingGameData::CreateNewGameList()
{
	LoadingScreenReference->ShowMessage(FText::FromString(TEXT("Create Systems List...")));

	FMessageDelegate Message;
	Message.AddDynamic(this, &ULoadingGameData::AddLoadingScreenToViewPort);

	AsyncTask(ENamedThreads::AnyThread, [=]()
	{
		FString ConfigResult;
		FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\configsys.xml");

		if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
		{

			//TArray<FGameSystem>Systems;

			UClassicFunctionLibrary::SetGameSystem(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("config.system")), Systems);

			Systems = UClassicFunctionLibrary::SortConfigSystem(Systems);
			for (int32 i = 0; i < Systems.Num(); i++)
			{
				GameRoot = Systems[i].RomPath + TEXT("\\gamelist.xml");
				if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
				{
					UClassicFunctionLibrary::SetGameData(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("gameList.game")), GameData, nullptr);
					UClassicFunctionLibrary::SortGameDate(GameData);
					UClassicFunctionLibrary::FormatGameData(GameData, ConfigurationData, Systems[i]);
					Systems[i].GameDatas = GameData;
					GameData.Empty();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
				}
			}
			ClassicGameInstance->SetSystemSave(Systems);
			if (UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0))
			{
				UE_LOG(LogTemp, Warning, TEXT("Saved"));
				AsyncTask(ENamedThreads::GameThread, [=]()
				{
					MainInterfaceReference->RemoveFromParent();
					LoadingScreenReference->ShowMessage(FText::FromString(TEXT("Created Systems List...")));
					GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ULoadingGameData::LoadGameSystems, 0.1f, false, DELAY + 3.0f);
				});
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Not Saved"));
			}

		}

	});
}

void ULoadingGameData::AddMainInterfaceToViewPort()
{
	if (LoadingScreenReference != nullptr)
	{
		LoadingScreenReference->RemoveFromParent();
	}
	if (MainInterfaceReference != nullptr)
	{
		//let add it to the view port
		MainInterfaceReference->AddToViewport(0);
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
	if (LoadingScreenReference)
	{
		LoadingScreenReference->AddToViewport(1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingScreenReference Not Found or Null"));
	}
}

void ULoadingGameData::SetToRestartWidgets()
{
	MainInterfaceReference->Clear();
	MainInterfaceReference->PlayAnimationReverse(MainInterfaceReference->AnimationShowConfiguration);
	MainInterfaceReference->PlayAnimationReverse(MainInterfaceReference->LoadListGame);
	MainInterfaceReference->PlayAnimationReverse(MainInterfaceReference->BarTop);
	MainInterfaceReference->WBPFrame->SetFramePosition(1, EFocusTop::NONE);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ULoadingGameData::RestartWidgets, 0.1f, false, DELAY + 1.0f);
}

void ULoadingGameData::RestartWidgets()
{
	AddLoadingScreenToViewPort();
	CreateNewGameList();
}

