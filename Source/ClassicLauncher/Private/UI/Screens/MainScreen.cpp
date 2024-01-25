// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Screens/MainScreen.h"

#include "UI/Components/Card.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Core/ClassicGameinstance.h"
#include "Core/ScreenManager.h"
#include "UI/Layout/ButtonsPrompt.h"
#include "UI/Layout/MoreInformationsLayout.h"
#include "UI/Layout/GameListLayout.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"
#include "Audio/ClassicMediaPlayer.h"
#include "UI/Layout/OptionsLayout.h"
#include "UI/Components/MessageBalloon.h"
#include "UI/Layout/LoopScroll.h"
#include "UI/Layout/Frame.h"
#include "UI/Components/TextBoxScroll.h"
#include "TextImageBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Data/DataManager.h"
#include "Data/GameDataFunctionLibrary.h"
#include "UI/Layout/FooterDetails.h"
#include "UI/Layout/ToolTipsLayout.h"
#include "UI/Layout/Header.h"

#define LOCTEXT_NAMESPACE "ButtonsSelection"

UMainScreen::UMainScreen(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Clear();
}

void UMainScreen::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMainScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Header->OnClickDelegate.AddDynamic(this, &UMainScreen::OnClickHeader);
	Header->OnFocusDelegate.AddDynamic(this, &UMainScreen::OnFocusHeader);
	Header->OnLostFocusDelegate.AddDynamic(this, &UMainScreen::OnLostFocusHeader);
	LoopScroll->OnCardClick.AddDynamic(this, &UMainScreen::OnClickLaunch);
	DataManager->OnDoneGameListNative.AddDynamic( this, &UMainScreen::OnLoadGamesList);

	CanvasPanelScreen->SetRenderOpacity(0);
}

void UMainScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bKeyPressed)
	{
		NavigationGame(InputPressed);
		if (PositionY == EPositionY::BOTTOM)
		{
			FooterDetails->TextBoxScroll->SetNewScroll(InputPressed, 0.0025f);
		}
	}
}

void UMainScreen::SteamRunApp()
{
	if (!UClassicFunctionLibrary::IsRunningSteamApp(DataManager->GetGameData().Path))
	{
		RunningGame(false);
	}
	UClassicFunctionLibrary::PauseProcess(0.5f);
}

void UMainScreen::ExternRunApp()
{
	if (!UClassicFunctionLibrary::ClassicIsApplicationRunning(ProcessID))
	{
		RunningGame(false);
	}

	if (MultiInput.CheckInputPressed() && ProcessID != 0)
	{
		TArray<FString> TextArguments;
		TextArguments.Add(TEXT("  /PID   "));
		TextArguments.Add(FString::FromInt(ProcessID));
		int32 Proc;
		UClassicFunctionLibrary::CreateProc(Proc, TEXT("taskkill  "), TextArguments, false, true, 0, TEXT(""));
	}
	UClassicFunctionLibrary::PauseProcess(0.125f);
}

void UMainScreen::LoadGamesList()
{
	const int32 IndexGameSystem = DataManager->IndexGameSystem;
	if (IndexGameSystem == 0)
	{
		Frame->FrameIndexTop = 1;
		if (GEngine)
		{
			GEngine->ForceGarbageCollection(true);
		}
	}
	Header->EnableButtonsHeader(IndexGameSystem);
	DataManager->LoadGamesList();
}

void UMainScreen::ShowGames()
{
	SetPlayAnimation(TEXT("LoadListGame"));
	PrepareThemes();
	DataManager->GetScreenManager()->SetVisibilityLoadingScreen(ESlateVisibility::Collapsed);
	SetInputEnable(true);
}

void UMainScreen::PrepareThemes()
{
	OnPrepareThemes(); //BlueprintImplementableEvent
}

void UMainScreen::NativePressInput(const FKey& InKey)
{
	Super::NativePressInput(InKey);
	if (!GetInputEnable())
	{
		MultiInput.SetInput(UClassicFunctionLibrary::GetInputButtonsGame(InKey));
	}
}

void UMainScreen::NativeReleaseInput(const FKey& InKey)
{
	Super::NativeReleaseInput(InKey);
	if (GetInputEnable())
	{
		if (InputLastPressed == EButtonsGame::B)
		{
			OnClickBackAction();
		}
		if (InputLastPressed == EButtonsGame::M)
		{
			DataManager->GetClassicMediaPlayerReference()->PlaylistMusic();
		}
		if (InputLastPressed == EButtonsGame::Y)
		{
			DataManager->SetFavoriteToSave();
		}
	}
	MultiInput.SetAllNoneInput();
}

FReply UMainScreen::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float ScrollScale = InMouseEvent.GetWheelDelta();
	if (PositionY == EPositionY::BOTTOM)
	{
		if (ScrollScale > 0)
		{
			FooterDetails->TextBoxScroll->SetNewScroll(EButtonsGame::SCROLLUP, 0.0025f * 4);
		}
		else if (ScrollScale < 0)
		{
			FooterDetails->TextBoxScroll->SetNewScroll(EButtonsGame::SCROLLDOWN, 0.0025f * 4);
		}
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

/////////////////////////////////////////////////
///navigation area

#pragma region NavigationArea

void UMainScreen::NavigationGame(EButtonsGame Input)
{
	if (Input == EButtonsGame::UP || Input == EButtonsGame::DOWN ||
		Input == EButtonsGame::LEFT || Input == EButtonsGame::RIGHT ||
		Input == EButtonsGame::LB || Input == EButtonsGame::RB)
	{
		if (GetInputEnable() && !GetDelayInput())
		{
			DelayInput(TimerDelayInput + FirstDelayInput);
			FirstDelayInput = 0.0f;
			TimerDelayInput = 0.09f;

			if (Focus == EFocus::MAIN)
			{
				NavigationMain(Input);
			}
			else if (Focus == EFocus::SYSTEM)
			{
				NavigationSystem(Input);
			}
			else if (Focus == EFocus::INFO)
			{
				NavigationInfo(Input);
			}
			else if (Focus == EFocus::CONFIG)
			{
				NavigationConfiguration(Input);
			}
		}
		OnNavigationGame(Input); //BlueprintImplementableEvent
	}
}

void UMainScreen::NavigationMain(EButtonsGame Input)
{
	TimerDelayInput = 0.14f;
	const bool LeftRight = InputLastPressed == EButtonsGame::LEFT || InputLastPressed == EButtonsGame::RIGHT;
	const bool LeftRightTrigger = InputLastPressed == EButtonsGame::LB || InputLastPressed == EButtonsGame::RB;

	if (LeftRight || LeftRightTrigger)
	{
		if (PositionY == EPositionY::CENTER)
		{
			TimerDelayInput = 0.0f;
			if (LoopScroll->GetUnlockInput())
			{
				if (Input == EButtonsGame::RIGHT || Input == EButtonsGame::RB)
				{
					LoopScroll->DirectionRight();
				}
				else if (Input == EButtonsGame::LEFT || Input == EButtonsGame::LB)
				{
					LoopScroll->DirectionLeft();
				}
			}
		}
		else if (PositionY == EPositionY::TOP && LeftRight)
		{
			SetFrame();
		}
	}
	else if (InputLastPressed == EButtonsGame::UP && bUpDownPressed)
	{
		SetNavigationFocusUpBottom();
	}
	else if (InputLastPressed == EButtonsGame::DOWN && bUpDownPressed)
	{
		SetNavigationFocusDownBottom();
	}
}

void UMainScreen::NavigationSystem(EButtonsGame Input)
{
	GameList->SetFocusItem(InputLastPressed);
}

void UMainScreen::NavigationInfo(EButtonsGame Input)
{
	Information->SetFocus();
}

void UMainScreen::NavigationConfiguration(EButtonsGame Input)
{
	if (Input == EButtonsGame::LEFT || Input == EButtonsGame::RIGHT)
	{
		CancelDelay();
	}
	Options->SetFocusOptionsItem(Input);
}

void UMainScreen::SetNavigationFocusUpBottom()
{
	if (PositionY == EPositionY::BOTTOM)
	{
		FooterDetails->CloseFooter(PositionY, DataManager->GetClassicMediaPlayerReference());
		CanvasPanelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (PositionY == EPositionY::CENTER)
		{
			SetPlayAnimation(TEXT("ShowDescBottomInfoReverse"));
		}
	}
	else if (PositionY == EPositionY::CENTER)
	{
		PositionY = EPositionY::TOP;
		SetButtonsIconInterfaces(PositionY);
		SetFrame();
	}
}

void UMainScreen::SetNavigationFocusDownBottom()
{
	if (PositionY == EPositionY::CENTER)
	{
		FooterDetails->OpenFooter(DataManager->GetGameData(), DataManager->IndexGameSystem, PositionY, DataManager->GetClassicMediaPlayerReference());
		CanvasPanelRoot->SetVisibility(ESlateVisibility::HitTestInvisible);
		SetPlayAnimation(TEXT("ShowDescBottomInfo"));
	}
	else if (PositionY == EPositionY::BOTTOM)
	{
		FooterDetails->ExpandVideo();
	}
	else
	{
		PositionY = EPositionY::CENTER;
		LoopScroll->SetFocus();
		SetButtonsIconInterfaces(PositionY);
		SetFrame();
		Header->SetFocusButton();
	}
}

//end navigate area
///////////////////////////////////////////////////

#pragma endregion NavigationArea

void UMainScreen::SetTitle(int32 Index)
{
	DataManager->IndexGameData = Index;
	const FString Title = DataManager->GetGameData().nameFormated;
	TextTitleGame->SetText(FText::FromString(Title));
	FooterDetails->TextBoxScroll->SetText(DataManager->GetGameData().descFormated);
	SetButtonsIconInterfaces(PositionY);
	SetFrame();
}

void UMainScreen::OnClickLaunch()
{
	if (PositionY == EPositionY::CENTER && GetInputEnable())
	{
		LoopScroll->OpenCard();
		if (DataManager->IndexGameSystem == 0)
		{
			SetLastPositions(false);
			GetWorld()->GetTimerManager().SetTimer(LauncherTimerHandle, [&]()
			                                       {
				                                       DataManager->IndexGameSystem = DataManager->IndexGameData + 1;
				                                       ResetCards();
			                                       }
			                                       , 0.015f, false, 0.1f);
		}
		else
		{
			PlayAnimationForward(FadeStartSystem);
			DataManager->SetCountPlayerToSave();
			GetWorld()->GetTimerManager().SetTimer(LauncherTimerHandle, this, &UMainScreen::AppLaunch, 0.015f, false, 1.5f);
		}
	}
}

void UMainScreen::AppLaunch()
{
	const FString PathRomFormatted = UClassicFunctionLibrary::HomeDirectoryReplace(DataManager->GetGameData().PathFormated);
	const FString ExecutablePath = (DataManager->GetGameData().Executable == TEXT("")) ? DataManager->GetGameSystem().Executable : DataManager->GetGameData().Executable;
	const FString Arguments = (DataManager->GetGameData().Arguments == TEXT("")) ? DataManager->GetGameSystem().Arguments : DataManager->GetGameData().Arguments;
	TArray<FString> Commands;

	if (!Arguments.IsEmpty())
	{
		Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(Arguments));
	}
	if (!ExecutablePath.IsEmpty())
	{
		Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(PathRomFormatted));
	}
	OpenExternalProcess(ExecutablePath, Commands);
	UE_LOG(LogTemp, Warning, TEXT("OpenExternalProcess: %s %s %s"), *ExecutablePath, *Arguments, *PathRomFormatted);
}

void UMainScreen::OpenExternalProcess(FString ExecutablePath, TArray<FString> CommandArgs)
{
	const FString WorkingDirectory = FPaths::GetPath(ExecutablePath);
	UClassicFunctionLibrary::CreateProc(ProcessID, ExecutablePath, CommandArgs, false, false, 0, WorkingDirectory);
	UClassicFunctionLibrary::GameSettingsRunning();
	RunningGame(true);
	if (DataManager->GetGameSystem().Executable.Equals(TEXT("steam")))
	{
		GetWorld()->GetTimerManager().SetTimer(DelayRunAppTimerHandle, this, &UMainScreen::SteamRunApp, 0.064f, true, 10);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DelayRunAppTimerHandle, this, &UMainScreen::ExternRunApp, 0.064f, true, 10);
	}
}

void UMainScreen::OnClickSystem(int32 Value)
{
	if (GetInputEnable())
	{
		UE_LOG(LogTemp, Warning, TEXT("The OnClickSystem parameter value is: %d"), Value);

		if (DataManager->IndexGameSystem == Value)
		{
			Focus = EFocus::MAIN;
			PositionY = EPositionY::CENTER;
			LoopScroll->SetFocus();
			SetButtonsIconInterfaces(PositionY);
			Frame->SetFramePositionWithoutAnimation(Frame->FrameIndexCenter);
			SetPlayAnimation(TEXT("ShowSystemReverse"));
			Header->SetFocusButton();
			return;
		}
		SetLastPositions(false);
		DataManager->IndexGameSystem = Value;
		ResetCards();
		OnClickOnSystem();
	}
}

void UMainScreen::SetButtonsIconInterfaces(EPositionY GetPosition)
{
	if (GetPosition == EPositionY::TOP)
	{
		PromptMain->SetTexts(PromptHelper.TextTop);
		PromptMain->SetButtonsVisibility(PromptHelper.IconTop);
	}
	else if (GetPosition == EPositionY::CENTER && DataManager->IndexGameSystem != 0)
	{
		if (PromptHelper.TextCenter.IsValidIndex(5))
		{
			PromptHelper.TextCenter[5] = (DataManager->GetGameData().favorite) ? LOCTEXT("buttonRemoveFavorite", "Remove Favorite") : LOCTEXT("buttonRemoveFavoriteAddFavorite", "Add Favorite");
		}
		PromptMain->SetTexts(PromptHelper.TextCenter);
		PromptMain->SetButtonsVisibility(PromptHelper.IconCenter);
	}
	else
	{
		PromptMain->SetTexts(PromptHelper.TextCenterSystem);
		PromptMain->SetButtonsVisibility(PromptHelper.IconCenterSystem);
	}
}

void UMainScreen::RunningGame(const bool bIsRun)
{
	if (bIsRun)
	{
		DataManager->GetClassicMediaPlayerReference()->PauseMusic();
		SetInputEnable(false);
	}
	else
	{
		ProcessID = 0;
		UClassicFunctionLibrary::GameSettingsInit();
		GetWorld()->GetTimerManager().ClearTimer(DelayRunAppTimerHandle);
		DelayRunAppTimerHandle.Invalidate();
		InputLastPressed = EButtonsGame::NONE;

		FTimerHandle ReturnHandle;
		GetWorld()->GetTimerManager().SetTimer(
			ReturnHandle,
			[&]()
			{
				DataManager->GetClassicMediaPlayerReference()->PlaylistMusic();
				PlayAnimationReverse(FadeStartSystem);
				LoopScroll->SetUnlockInput(true);
				SetFrame();
				SetInputEnable(true);
			}, 0.18f, false);
	}
}

void UMainScreen::ResetCards()
{
	Frame->ImageFrameTop->SetVisibility(ESlateVisibility::Hidden);
	LoopScroll->SetFocus();
	SetInputEnable(false);
	SetPlayAnimation(TEXT("ShowSystemReverse"));
	Header->SetFocusButton();
	Focus = EFocus::MAIN;
	FooterDetails->Clear();
	PositionY = EPositionY::CENTER;
	SetPlayAnimation(TEXT("LoadListGameReverse"));
	MessageDisplay->CancelMessage();
	DataManager->IndexGameData = 0;
	DataManager->GameData.Empty();
}

void UMainScreen::Clear()
{
	SetInputEnable(false);
	ProcessID = 0;
	InputPressed = EButtonsGame::NONE;
	InputLastPressed = EButtonsGame::NONE;
	MultiInput.SetAllNoneInput();
	PositionY = EPositionY::CENTER;
	Focus = EFocus::MAIN;
	bKeyPressed = false;
	bUpDownPressed = true;
	bDelayQuit = false;
	if (DataManager != nullptr)
	{
		DataManager->IndexGameData = 0;
		DataManager->IndexGameSystem = 0;
		DataManager->GameData.Empty();
	}
}

void UMainScreen::SetHeaderButtonFocus()
{
	const int32 Index = Frame->FrameIndexTop;
	Header->SetFocusButton(Index, true);
	if (Index == 4)
	{
		Information->SetGameInfo(DataManager->GetGameData());
	}
}

void UMainScreen::OnClickHeader(int32 Index)
{
	if (PositionY == EPositionY::BOTTOM) return;
	PositionY = EPositionY::TOP;
	switch (Index)
	{
	case 1: OnClickSelectSystem();
		break;
	case 2: OnClickConfigurations();
		break;
	case 3: OnClickFilters();
		break;
	case 4: OnClickInfo();
		break;
	default: break;
	}
}

void UMainScreen::OnFocusHeader(int32 Index)
{
	if (GetMouseEnable())
	{
		PositionY = EPositionY::TOP;
		ToolTips->OnFocus(Index);
		Header->AnimationFocus(true);
		Frame->FrameIndexTop = Index;
		Frame->SetFramePositionWithAnimation(PositionY);
		SetButtonsIconInterfaces(PositionY);
		if (Index == 4)
		{
			Information->SetGameInfo(DataManager->GetGameData());
		}
	}
	if (Index == 0) return;
	ToolTips->OnFocus(Frame->FrameIndexTop);
}

void UMainScreen::OnLostFocusHeader(int32 Index)
{
	ToolTips->OnLostFocus(Index);
}

void UMainScreen::OnClickSelectSystem()
{
	Focus = EFocus::SYSTEM;
	SetPlayAnimation(TEXT("ShowSystem"));
	PositionY = EPositionY::TOP;
	GameList->SetFocusItem(EButtonsGame::NONE);
}

void UMainScreen::OnClickConfigurations()
{
	Focus = EFocus::CONFIG;
	SetPlayAnimation(TEXT("AnimationShowConfiguration"));
	Options->SetFocusOptionsItem(EButtonsGame::NONE);
}

void UMainScreen::OnClickFilters()
{
	const bool NumFavorites = UGameDataFunctionLibrary::IsHasFavorites(DataManager->GameData);
	FText Message = LOCTEXT("MessageNoFavorites", "No favorites to show");

	const int32 IndexGameSystem = DataManager->IndexGameSystem;
	DataManager->GameSystems[IndexGameSystem].Positions.ChangeFilter();
	const EGamesFilter Filter = DataManager->GameSystems[IndexGameSystem].Positions.OrderBy;

	if (NumFavorites && Filter == EGamesFilter::FAVORITES_FIRST)
	{
		Message = LOCTEXT("MessageFavoritesFirst", "Show favorites first");
	}
	else if (NumFavorites && Filter == EGamesFilter::FAVORITES_ONLY)
	{
		Message = LOCTEXT("MessageOnlyFavorites", "Show only favorites");
	}
	else if (Filter == EGamesFilter::DEFAULT)
	{
		Message = LOCTEXT("MessageAllGames", "Show all games");
	}
	else
	{
		Message = LOCTEXT("MessageAllGames", "Show all games");
	}

	ShowMessage(Message, 3.5f);
	NavigationGame(EButtonsGame::DOWN);
	SetLastPositions(true);
	Header->SetFocusButton();
	ResetCards();
}

void UMainScreen::OnClickInfo()
{
	if (Focus == EFocus::MAIN)
	{
		SetPlayAnimation(TEXT("ShowInfo"));
		Focus = EFocus::INFO;
	}
}

void UMainScreen::OnClickBackAction()
{
	if (PositionY == EPositionY::TOP && Focus != EFocus::MAIN)
	{
		CloseMenus();
	}
	else if (PositionY == EPositionY::CENTER || PositionY == EPositionY::TOP)
	{
		if (DataManager->IndexGameSystem != 0)
		{
			SetLastPositions(false);
			DataManager->IndexGameSystem = 0;
			ResetCards();
			return;
		}

		if (bDelayQuit)
		{
			SetLastPositions(false);
			if (DataManager->GetClassicGameInstance()->SaveGameSystem(DataManager->GameSystems))
			{
				UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
			}
		}
		bDelayQuit = true;
		GetWorld()->GetTimerManager().SetTimer(DelayQuitTimerHandle, [this]()
		                                       {
			                                       bDelayQuit = false;
		                                       }
		                                       , 3.f, false, -1);
		ShowMessage(LOCTEXT("PressAgainToQuit", "Press Again To Quit"), 3);
	}
	else if (PositionY == EPositionY::BOTTOM)
	{
		SetNavigationFocusUpBottom();
	}
}

void UMainScreen::SetLastPositions(const bool bResetPositions)
{
	const int32 IndexGameSystem = DataManager->IndexGameSystem;
	FIndexPositions Positions = LoopScroll->GetScrollOffSet();
	Positions.OrderBy = DataManager->GameSystems[IndexGameSystem].Positions.OrderBy;
	if (bResetPositions || Positions.OrderBy != EGamesFilter::DEFAULT)
	{
		DataManager->GameSystems[IndexGameSystem].Positions.DefaultValues();
		DataManager->GameSystems[IndexGameSystem].Positions.OrderBy = Positions.OrderBy;
		return;
	}
	DataManager->GameSystems[IndexGameSystem].Positions = Positions;
}

void UMainScreen::SetFrame()
{
	if (PositionY == EPositionY::TOP)
	{
		Frame->SetIndexTop(InputLastPressed, DataManager->IndexGameSystem);
		SetHeaderButtonFocus();
	}
	Frame->SetFramePositionWithAnimation(PositionY);
}

void UMainScreen::CloseMenus()
{
	if (Focus == EFocus::SYSTEM)
	{
		SetPlayAnimation(TEXT("ShowSystemReverse"));
		Header->SetFocusButton(1);
		SetFrame();
		Focus = EFocus::MAIN;
	}
	else if (Focus == EFocus::CONFIG)
	{
		InputLastPressed = EButtonsGame::NONE;
		if (Options->WSScreens->GetActiveWidgetIndex())
		{
			Options->CloseModal();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(
				BackButtonTimerHandle,
				[&]()
				{
					SetPlayAnimation(TEXT("AnimationShowConfigurationReverse"));
					Header->SetFocusButton(2);
					SetFrame();
				},
				0.1f, false, -1);
			Focus = EFocus::MAIN;
		}
	}
	else if (Focus == EFocus::INFO)
	{
		SetPlayAnimation(TEXT("ShowInfoReverse"));
		Header->SetFocusButton(4);
		SetFrame();
		Focus = EFocus::MAIN;
	}
	else
	{
		SetNavigationFocusDownBottom();
	}
}

void UMainScreen::ShowMessage(const FText Message, const float InRate)
{
	MessageDisplay->ShowMessage(Message, InRate);
}

void UMainScreen::SetVisibilityDebugButton(UButton* Button)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Button->SetVisibility(ESlateVisibility::Visible);
	bDebug = true;
#endif
}

#undef LOCTEXT_NAMESPACE
