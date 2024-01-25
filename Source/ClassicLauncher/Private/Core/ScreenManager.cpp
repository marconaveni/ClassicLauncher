// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Core/ScreenManager.h"

#include "UI/Layout/GameListLayout.h"
#include "UI/Layout/Frame.h"
#include "UI/Screens/MainScreen.h"
#include "UI/Screens/LoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Data/DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Layout/Header.h"

#define DELAY 1.0f
#define LOCTEXT_NAMESPACE "ButtonsSelection"

void UScreenManager::Init(TSubclassOf<UMainScreen> MainScreenClass, TSubclassOf<ULoadingScreen> LoadingScreenClass)
{
	DataManager = GetWorld()->GetSubsystem<UDataManager>();
	CreateWidgetsAndActors(MainScreenClass, LoadingScreenClass);
}

void UScreenManager::CreateWidgetsAndActors(TSubclassOf<UMainScreen> MainScreenClass, TSubclassOf<ULoadingScreen> LoadingScreenClass)
{
	if (MainScreenClass != nullptr && LoadingScreenClass != nullptr) // Check if the Asset is assigned.
	{
		// Create the widget and store it.
		DataManager->SetMainScreenReference(CreateWidget<UMainScreen>(DataManager->GetGameplayStatics(), MainScreenClass));
		DataManager->SetLoadingScreenReference(CreateWidget<ULoadingScreen>(DataManager->GetGameplayStatics(), LoadingScreenClass));

		if (DataManager->GetMainScreenReference() != nullptr)
		{
			//add it to the view port
			DataManager->GetMainScreenReference()->AddToViewport(0);
		}
		if (DataManager->GetLoadingScreenReference() != nullptr)
		{
			//add it to the view port
			DataManager->GetLoadingScreenReference()->AddToViewport(1);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainInterfaceClass or LoadingScreenClass the Assets is not assigned"));
	}
}


void UScreenManager::ShowMainScreenToViewPort()
{
	if (DataManager->GetMainScreenReference() != nullptr)
	{
		DataManager->GetMainScreenReference()->SetVisibility(ESlateVisibility::Visible);
		DataManager->GetMainScreenReference()->GameList->AddSystems(DataManager->GameSystems, DataManager->GetMainScreenReference());
		DataManager->GetMainScreenReference()->LoadGamesList();

		//Input mode settings. 
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		//Mouse behavior mode 
		DataManager->GetGameplayStatics()->SetInputMode(InputMode);
		DataManager->GetGameplayStatics()->bShowMouseCursor = true;
		UGameplayStatics::SetViewportMouseCaptureMode(DataManager->GetGameplayStatics(), EMouseCaptureMode::NoCapture);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainInterfaceReference Not Found or Null"));
	}
}


void UScreenManager::SetVisibilityLoadingScreen(const ESlateVisibility Visibility) const
{
	if (DataManager->GetLoadingScreenReference() != nullptr)
	{
		DataManager->GetLoadingScreenReference()->SetVisibility(Visibility);
	}
}


void UScreenManager::SetToRestartWidgets()
{
	DataManager->GetMainScreenReference()->SetInputEnable(false);
	DataManager->GetMainScreenReference()->Clear();
	DataManager->GetMainScreenReference()->SetPlayAnimation(TEXT("AnimationShowConfigurationReverse"));
	DataManager->GetMainScreenReference()->Header->SetFocusButton();
	DataManager->GetMainScreenReference()->Frame->SetFramePositionWithoutAnimation(1);
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UScreenManager::RestartConfigurations, 0.1f, false, DELAY);
	SetVisibilityLoadingScreen(ESlateVisibility::Visible);
	ShowMessage(LOCTEXT("UpdateGame", "Update game wait"));
}

void UScreenManager::RestartConfigurations()
{
	DataManager->LoadConfiguration();
	DataManager->InitGameData();
}

void UScreenManager::ShowMessage(const FText& Message) const
{
	DataManager->GetLoadingScreenReference()->ShowMessage(Message);
}

#undef LOCTEXT_NAMESPACE

