// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "MainInterface.h"

#include "AnimationUILoader.h"
#include "Card.h"
#include "ClassicButtonSystem.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "ClassicFunctionLibrary.h"
#include "ClassicGameinstance.h"
#include "ClassicSaveGame.h"
#include "ClassicButtonsIcons.h"
#include "ClassicInfoInterface.h"
#include "ClassicSystemListInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"
#include "ClassicMediaPlayer.h"
#include "ClassicConfigurations.h"
#include "ClassicGameMode.h"
#include "Cover.h"
#include "EngineUtils.h"
#include "MessageBalloon.h"
#include "LoopScrollBox.h"
#include "Frame.h"
#include "LoadingGameData.h"
#include "TextBoxScroll.h"
#include "TextImageBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "UI/Layout/FooterDetails.h"
#include "UI/Layout/ToolTipsLayout.h"
#include "UI/Layout/Header.h"

#define LOCTEXT_NAMESPACE "ButtonsSelection"

UMainInterface::UMainInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Clear();
	
	TextTop.Add(FText::FromString(TEXT("")));
	TextTop.Add(LOCTEXT("buttonGameList", "Game List"));
	TextTop.Add(LOCTEXT("buttonSelect", "Select"));
	TextTop.Add(LOCTEXT("buttonOk", "Ok"));
	TextTop.Add(LOCTEXT("buttonBack", "Back"));
	TextTop.Add(FText::FromString(TEXT("")));
	TextTop.Shrink();

	TextCenter.Add(LOCTEXT("buttonMenu", "Menu"));
	TextCenter.Add(LOCTEXT("buttonDetails", "Details"));
	TextCenter.Add(LOCTEXT("buttonSelect", "Select"));
	TextCenter.Add(LOCTEXT("buttonStartGame", "Start Game"));
	TextCenter.Add(LOCTEXT("buttonBack", "Back"));
	TextCenter.Add(LOCTEXT("buttonFavorite", "Favorite"));
	TextCenter.Shrink();

	TextCenterSystem.Add(LOCTEXT("buttonMenu", "Menu"));
	TextCenterSystem.Add(LOCTEXT("buttonDetails", "Details"));
	TextCenterSystem.Add(LOCTEXT("buttonSelect", "Select"));
	TextCenterSystem.Add(LOCTEXT("buttonOk", "Ok"));
	TextCenterSystem.Add(LOCTEXT("buttonExit", "Exit"));
	TextCenterSystem.Add(FText::FromString(TEXT("")));
	TextCenterSystem.Shrink();

	IconTop.Add(ESlateVisibility::Collapsed);
	IconTop.Add(ESlateVisibility::Visible);
	IconTop.Add(ESlateVisibility::Visible);
	IconTop.Add(ESlateVisibility::Visible);
	IconTop.Add(ESlateVisibility::Visible);
	IconTop.Add(ESlateVisibility::Collapsed);
	IconTop.Shrink();

	IconCenter.Add(ESlateVisibility::Visible);
	IconCenter.Add(ESlateVisibility::Visible);
	IconCenter.Add(ESlateVisibility::Visible);
	IconCenter.Add(ESlateVisibility::Visible);
	IconCenter.Add(ESlateVisibility::Visible);
	IconCenter.Add(ESlateVisibility::Visible);
	IconCenter.Shrink();

	IconCenterSystem.Add(ESlateVisibility::Visible);
	IconCenterSystem.Add(ESlateVisibility::Visible);
	IconCenterSystem.Add(ESlateVisibility::Visible);
	IconCenterSystem.Add(ESlateVisibility::Visible);
	IconCenterSystem.Add(ESlateVisibility::Visible);
	IconCenterSystem.Add(ESlateVisibility::Collapsed);
	IconCenterSystem.Shrink();
}

void UMainInterface::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMainInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameMode = Cast<AClassicGameMode>(UGameplayStatics::GetGameMode(this));

	Header->OnClickDelegate.AddDynamic(this, &UMainInterface::OnClickHeader);
	Header->OnFocusDelegate.AddDynamic(this, &UMainInterface::OnFocusHeader);
	Header->OnLostFocusDelegate.AddDynamic(this, &UMainInterface::OnLostFocusHeader);
	LoopScroll->OnCardIndex.AddDynamic(this, &UMainInterface::CardIndex);
	LoopScroll->OnCardClick.AddDynamic(this, &UMainInterface::OnClickLaunch);
	
	for (TActorIterator<AClassicMediaPlayer> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		ClassicMediaPlayerReference = *ActorIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference AClassicMediaPlayer Founds: %s "), *ClassicMediaPlayerReference->GetName());
	}

	WBPFrame->LoopScrollReference = LoopScroll;
	ClassicMediaPlayerReference->MainInterfaceReference = this;
	LoopScroll->MainInterfaceReference = this;
	CanvasPanelScreen->SetRenderOpacity(0);
}

void UMainInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

	if (bKeyPressed && PositionY == EPositionY::CENTER && (InputPressed == EButtonsGame::LEFT || InputPressed == EButtonsGame::RIGHT))
	{
		SpeedScroll = UKismetMathLibrary::Ease(InitialSpeedScroll, TargetSpeedScroll, Alpha, EEasingFunc::EaseIn);
		Alpha = FMath::Clamp(Alpha + 0.24f * GetWorld()->GetDeltaSeconds(), 0, 1);
	}
	else if (bKeyPressed && PositionY == EPositionY::CENTER && (InputPressed == EButtonsGame::LB || InputPressed == EButtonsGame::RB))
	{
		SpeedScroll = FastSpeedScroll;
	}
	else
	{
		Alpha = 0;
		SpeedScroll = InitialSpeedScroll;
		LoopScroll->Speed = InitialSpeedScroll;
	}
	LoopScroll->Speed = SpeedScroll;
}

void UMainInterface::SteamRunApp()
{
	if (!UClassicFunctionLibrary::IsRunningSteamApp(GameData[IndexCard].Path))
	{
		RunningGame(false);
	}
	UClassicFunctionLibrary::PauseProcess(0.5f);
}

void UMainInterface::ExternRunApp()
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

void UMainInterface::LoadGamesList()
{
	TArray<FGameSystem>Systems = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave;
	Header->EnableButtonsHeader(CountSystem);
	if (CountSystem == 0)
	{
		WBPFrame->FrameIndexTop = 1;
		if (GEngine)
		{
			GEngine->ForceGarbageCollection(true);
		}
	}

	int32 NumFavorites = 0;
	GameData = UClassicFunctionLibrary::FilterGameData(Systems[CountSystem].GameDatas, Systems[CountSystem].Positions.OrderBy, NumFavorites);
	if (NumFavorites == 0 && Systems[CountSystem].Positions.OrderBy != EGamesFilter::DEFAULT)
	{
		UE_LOG(LogTemp, Warning, TEXT("Retry load filter game data with order default"));
		Systems[CountSystem].Positions.DefaultValues();
		GameData = UClassicFunctionLibrary::FilterGameData(Systems[CountSystem].GameDatas, Systems[CountSystem].Positions.OrderBy, NumFavorites);
	}

	if (GameData.Num() > 0)
	{
		ClassicGameInstance->SetSystemSave(Systems);
		ConfigurationData.DefaultStartSystem = FString::FromInt(CountSystem);
		UClassicFunctionLibrary::SaveConfig(ConfigurationData);
		GameDataIndex = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas;
		OnLoadGamesList();
	}
}

void UMainInterface::LoadImages(const EButtonsGame Input, TArray<FGameData>& Data, const int32 DistanceIndex)
{
	if ((Input == EButtonsGame::RIGHT || Input == EButtonsGame::RB ||
		Input == EButtonsGame::LEFT || Input == EButtonsGame::LB) &&
		DistanceIndex > 0 && Data.Num() >= 64)
	{
		if (Input == EButtonsGame::RIGHT || Input == EButtonsGame::RB)
		{
			FirstIndex = IndexCard - DistanceIndex;
			LastIndex = IndexCard + (DistanceIndex - 1);
		}
		else if (Input == EButtonsGame::LEFT || Input == EButtonsGame::LB)
		{
			FirstIndex = IndexCard - (DistanceIndex - 1);
			LastIndex = IndexCard + DistanceIndex;
		}

		if (FirstIndex < 0)
		{
			FirstIndex = Data.Num() - FMath::Abs(FirstIndex);
		}
		if (LastIndex >= Data.Num())
		{
			LastIndex = LastIndex - Data.Num();
		}

		FirstIndex = FMath::Clamp(FirstIndex, 0, Data.Num() - 1);
		LastIndex = FMath::Clamp(LastIndex, 0, Data.Num() - 1);

		int32 IndexLoad = -1, IndexUnload = -1;
		if (Input == EButtonsGame::RIGHT || Input == EButtonsGame::RB)
		{
			IndexUnload = FirstIndex;
			IndexLoad = LastIndex;
		}
		else if (Input == EButtonsGame::LEFT || Input == EButtonsGame::LB)
		{
			IndexUnload = LastIndex;
			IndexLoad = FirstIndex;
		}
		if (IndexLoad != -1 && IndexUnload != -1)
		{
			LoopScroll->CoverReference[IndexUnload]->SetCoverImage(ImageNull, 1, 1);
			OnLoadImages(IndexLoad, Data[IndexLoad].imageFormated);
			UE_LOG(LogTemp, Warning, TEXT("FirstIndex %d  IndexCard %d LastIndex %d"), FirstIndex, IndexCard, LastIndex);
		}

	}
}

void UMainInterface::ShowGames()
{
	SetPlayAnimation(TEXT("LoadListGame"));
	SetInputEnable(true);
	PrepareThemes();
	OnShowGames(); //BlueprintImplementableEvent
	GameMode->LoadingGameData->RemoveLoadingScreenToParent();
}

void UMainInterface::PrepareThemes()
{
	OnPrepareThemes(); //BlueprintImplementableEvent
}

void UMainInterface::NativePressInput(const FKey& InKey)
{
	Super::NativePressInput(InKey);
	if (!GetInputEnable())
	{
		MultiInput.SetInput(UClassicFunctionLibrary::GetInputButtonsGame(InKey));
	}
}

void UMainInterface::NativeReleaseInput(const FKey& InKey)
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
			ClassicMediaPlayerReference->PlaylistMusic();
		}
		if(InputLastPressed == EButtonsGame::Y)
		{
			SetFavoriteToSave();
		}
	}
	MultiInput.SetAllNoneInput();
}

FReply UMainInterface::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float ScrollScale = InMouseEvent.GetWheelDelta();

	if(PositionY == EPositionY::CENTER && LoopScroll->bUnlockInput)
	{
		if (ScrollScale > 0)
		{
			InputLastPressed = EButtonsGame::LEFT;
			LoopScroll->DirectionLeft(true);
		}
		else if (ScrollScale < 0)
		{
			InputLastPressed = EButtonsGame::RIGHT;
			LoopScroll->DirectionRight(true);
		}
		if(ScrollScale != 0)
		{
			if(CanvasPanelRoot->GetVisibility() != ESlateVisibility::HitTestInvisible)
			{
				CanvasPanelRoot->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			GetWorld()->GetTimerManager().ClearTimer(MouseScrollTimerHandle);
			MouseScrollTimerHandle.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(
				MouseScrollTimerHandle, [&]()
               {
                   CanvasPanelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
               }
               , 0.015f, false, 0.5f);
		}
	}
	else if (PositionY == EPositionY::BOTTOM)
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

void UMainInterface::NavigationGame(EButtonsGame Input)
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
			else if (Focus == EFocus::CONFIG) {
				NavigationConfiguration(Input);
			}
		}
		OnNavigationGame(Input); //BlueprintImplementableEvent
	}
}

void UMainInterface::NavigationMain(EButtonsGame Input)
{
	TimerDelayInput = 0.14f;
	const bool LeftRight = InputLastPressed == EButtonsGame::LEFT || InputLastPressed == EButtonsGame::RIGHT;
	const bool LeftRightTrigger = InputLastPressed == EButtonsGame::LB || InputLastPressed == EButtonsGame::RB;
	
	if (LeftRight || LeftRightTrigger)
	{
		if (PositionY == EPositionY::CENTER)
		{
			TimerDelayInput = 0.0f;
			if (LoopScroll->bUnlockInput)
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

void UMainInterface::NavigationSystem(EButtonsGame Input)
{
	WBPSystemsList->SetFocusItem(InputLastPressed, CountLocationY);
}

void UMainInterface::NavigationInfo(EButtonsGame Input)
{
	WBPInfo->SetFocus();
}

void UMainInterface::NavigationConfiguration(EButtonsGame Input)
{
	if (Input == EButtonsGame::LEFT || Input == EButtonsGame::RIGHT)
	{
		CancelDelay();
	}
	WBPClassicConfigurationsInterface->SetFocusItem(Input);
}

void UMainInterface::SetNavigationFocusUpBottom()
{
	if (PositionY == EPositionY::BOTTOM)
	{
		FooterDetails->CloseFooter(PositionY, ClassicMediaPlayerReference);
		CanvasPanelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if(PositionY == EPositionY::CENTER)
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

void UMainInterface::SetNavigationFocusDownBottom()
{
	if (PositionY == EPositionY::CENTER)
	{
		FooterDetails->OpenFooter(GameData[IndexCard], IndexCard,PositionY, ClassicMediaPlayerReference);
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


void UMainInterface::SetTitle(int32 Index)
{
	if (GameData.IsValidIndex(Index))
	{
		IndexCard = Index;
		const FString Title = GameData[IndexCard].nameFormated;
		TextTitleGame->SetText(FText::FromString(Title));
		FooterDetails->TextBoxScroll->SetText(GameData[IndexCard].descFormated);
		SetButtonsIconInterfaces(PositionY);
	}
}

void UMainInterface::OnClickLaunch()
{
	if (PositionY == EPositionY::CENTER && GetInputEnable())
	{
		LoopScroll->OpenCard();
		if (CountSystem == 0)
		{
			ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas = GameDataIndex;
			SetLastPositions(false);
			GetWorld()->GetTimerManager().SetTimer(LauncherTimerHandle, [&]()
			{
				CountSystem = IndexCard + 1;   //CountSystem = CountLocationY;
				ResetCards();
			}
			, 0.015f, false, 0.1f);
		}
		else
		{
			PlayAnimationForward(FadeStartSystem);
			AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this]
			{
				SetCountPlayerToSave();
			});
			GetWorld()->GetTimerManager().SetTimer(LauncherTimerHandle, this, &UMainInterface::AppLaunch, 0.015f, false, 1.5f);
		}
	}
}

void UMainInterface::AppLaunch()
{
	const FString PathRomFormated = UClassicFunctionLibrary::HomeDirectoryReplace(GameData[IndexCard].PathFormated);
	const FString ExecutablePath = (GameData[IndexCard].Executable == TEXT("")) ? GameSystems[CountSystem].Executable : GameData[IndexCard].Executable;
	const FString Arguments = (GameData[IndexCard].Arguments == TEXT("")) ? GameSystems[CountSystem].Arguments : GameData[IndexCard].Arguments;
	TArray<FString> Commands;

	if (!Arguments.IsEmpty())
	{
		Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(Arguments));
	}
	if (!ExecutablePath.IsEmpty())
	{
		Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(PathRomFormated));
	}
	OpenExternalProcess(ExecutablePath, Commands);
	UE_LOG(LogTemp, Warning, TEXT("OpenExternalProcess: %s %s %s"), *ExecutablePath, *Arguments, *PathRomFormated);
}

void UMainInterface::OpenExternalProcess(FString ExecutablePath, TArray<FString> CommandArgs)
{
	const FString WorkingDirectory = FPaths::GetPath(ExecutablePath);
	UClassicFunctionLibrary::CreateProc(ProcessID, ExecutablePath, CommandArgs, false, false, 0, WorkingDirectory);
	GameMode->GameSettingsRunning();
	RunningGame(true);
	if (GameSystems[CountSystem].Executable.Equals(TEXT("steam")))
	{
		GetWorld()->GetTimerManager().SetTimer(DelayRunAppTimerHandle, this, &UMainInterface::SteamRunApp, 0.064f, true, 10);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DelayRunAppTimerHandle, this, &UMainInterface::ExternRunApp, 0.064f, true, 10);
	}
}

void UMainInterface::OnClickSystem(int32 Value)
{
	if (GetInputEnable())
	{
		UE_LOG(LogTemp, Warning, TEXT("The OnClickSystem parameter value is: %d"), Value);

		if (CountSystem == Value)
		{
			Focus = EFocus::MAIN;
			PositionY = EPositionY::CENTER;
			LoopScroll->SetFocus();
			SetButtonsIconInterfaces(PositionY);
			WBPFrame->SetFramePositionWithoutAnimation(WBPFrame->FrameIndexCenter);
			SetPlayAnimation(TEXT("ShowSystemReverse"));
			Header->SetFocusButton();
			return;
		}
		SetLastPositions(false);
		CountSystem = Value;   //CountSystem = CountLocationY;
		ResetCards();
		OnClickOnSystem();
	}
}

void UMainInterface::SetButtonsIconInterfaces(EPositionY GetPosition)
{
	if (GetPosition == EPositionY::TOP)
	{
		WBPButtonsIconsInterfaces->SetTexts(TextTop);
		WBPButtonsIconsInterfaces->SetButtonsVisibility(IconTop);
	}
	else if (GetPosition == EPositionY::CENTER && CountSystem != 0)
	{
		if (GameData.IsValidIndex(IndexCard) && TextCenter.IsValidIndex(5))
		{
			TextCenter[5] = (GameData[IndexCard].favorite) ? LOCTEXT("buttonRemoveFavorite", "Remove Favorite") : LOCTEXT("buttonRemoveFavoriteAddFavorite", "Add Favorite");
		}

		WBPButtonsIconsInterfaces->SetTexts(TextCenter);
		WBPButtonsIconsInterfaces->SetButtonsVisibility(IconCenter);
	}
	else
	{
		WBPButtonsIconsInterfaces->SetTexts(TextCenterSystem);
		WBPButtonsIconsInterfaces->SetButtonsVisibility(IconCenterSystem);
	}
}

void UMainInterface::SetCountPlayerToSave()
{
	int32 Find;

	if (UClassicFunctionLibrary::FindGameData(ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas, GameData[IndexCard], Find))
	{
		// Find = return inline found index
		ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas[Find].playcount++;
		ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas[Find].lastplayed = UClassicFunctionLibrary::FormatDateToXml();

		GameData[IndexCard].playcount++;
		GameData[IndexCard].lastplayed = UClassicFunctionLibrary::FormatDateToXml();
		GameDataIndex[Find].playcount++;
		GameDataIndex[Find].lastplayed = GameData[IndexCard].lastplayed;

		FString Path = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].RomPath;
		UClassicFunctionLibrary::SaveGameListXML(Path, ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas);

		UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0);
	}
}

void UMainInterface::SetFavoriteToSave()
{
	if (!GameSystems.IsValidIndex(CountSystem)) return;

	if (InputLastPressed == EButtonsGame::Y && !GameSystems[CountSystem].SystemName.Equals(TEXT("${System}")))
	{
		AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this]()
		{
			int32 Find;
			if (UClassicFunctionLibrary::FindGameData(ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas, GameData[IndexCard], Find))
			{
				// Find = return inline found index
				const bool ToggleFavorite = !ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas[Find].favorite;
				ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas[Find].favorite = ToggleFavorite;

				GameData[IndexCard].favorite = ToggleFavorite;
				GameDataIndex[Find].favorite = ToggleFavorite;

				FString Path = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].RomPath;
				UClassicFunctionLibrary::SaveGameListXML(Path, ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas);

				if (UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0))
				{

					AsyncTask(ENamedThreads::GameThread, [this, ToggleFavorite]()
					{

						LoopScroll->SetCardFavorite(ToggleFavorite);
						SetButtonsIconInterfaces(EPositionY::CENTER);

						ShowMessage((ToggleFavorite) ? LOCTEXT("MessageAddFavorite", "Add game to favorite") : LOCTEXT("RemoveFavorite", "Remove game to favorite"), 3.5f);

					});
				}
			}
		});
	}
}

void UMainInterface::RunningGame(const bool bIsRun)
{
	if (bIsRun)
	{
		ClassicMediaPlayerReference->PauseMusic();
		SetInputEnable(false);
	}
	else
	{
		ProcessID = 0;
		GameMode->GameSettingsInit();
		GetWorld()->GetTimerManager().ClearTimer(DelayRunAppTimerHandle);
		DelayRunAppTimerHandle.Invalidate();
		InputLastPressed = EButtonsGame::NONE;

		FTimerHandle ReturnHandle;
		GetWorld()->GetTimerManager().SetTimer(
			ReturnHandle,
			[&]()
			{
				ClassicMediaPlayerReference->PlaylistMusic();
				PlayAnimationReverse(FadeStartSystem);
				SetFrame();
				SetInputEnable(true);
			},0.18f ,false);

	}
}

void UMainInterface::ResetCards()
{
	SetInputEnable(false);
	SetPlayAnimation(TEXT("ShowSystemReverse"));
	Header->SetFocusButton();
	Focus = EFocus::MAIN;
	GameData.Empty();
	IndexCard = 0;
	FooterDetails->Clear();
	PositionY = EPositionY::CENTER;
	SetPlayAnimation(TEXT("LoadListGameReverse"));
	MessageDisplay->CancelMessage();
}

void UMainInterface::Clear()
{
	SetInputEnable(false);
	IndexCard = 0;
	ProcessID = 0;
	InputPressed = EButtonsGame::NONE;
	InputLastPressed = EButtonsGame::NONE;
	MultiInput.SetAllNoneInput();
	PositionY = EPositionY::CENTER;
	Focus = EFocus::MAIN;
	bKeyPressed = false;
	bUpDownPressed = true;
	bDelayQuit = false;
	CountSystem = 0;
	CountLocationY = 0;
	SpeedScroll = TargetSpeedScroll;
	GameData.Empty();
	GameDataIndex.Empty();
}

void UMainInterface::SetHeaderButtonFocus()
{
	const int32 Index = WBPFrame->FrameIndexTop;
	Header->SetFocusButton(Index, true);
	ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas = GameDataIndex;
	if (Index == 4)
	{
		WBPInfo->SetGameInfo(GameData[IndexCard]);
	}
}

void UMainInterface::OnClickHeader(int32 Index)
{
	if (PositionY == EPositionY::BOTTOM) return;
	PositionY = EPositionY::TOP;
	switch (Index)
	{
	case 1: OnClickSelectSystem(); break;
	case 2: OnClickConfigurations(); break;
	case 3: OnClickFavorites(); break;
	case 4: OnClickInfo(); break;
	default: break;
	}
}

void UMainInterface::OnFocusHeader(int32 Index)
{
	if(GetMouseEnable())
	{
		PositionY = EPositionY::TOP;
		ToolTips->OnFocus(Index);
		Header->AnimationFocus(true);
		WBPFrame->FrameIndexTop = Index;
		WBPFrame->SetFramePositionWithAnimation(Index, PositionY);
		SetButtonsIconInterfaces(PositionY);
		ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas = GameDataIndex;
		if (Index == 4)
		{
			WBPInfo->SetGameInfo(GameData[IndexCard]);
		}
	}
	if (Index == 0) return;
	ToolTips->OnFocus(WBPFrame->FrameIndexTop);
}

void UMainInterface::OnLostFocusHeader(int32 Index)
{
	ToolTips->OnLostFocus(Index);
}

void UMainInterface::OnClickSelectSystem()
{
	Focus = EFocus::SYSTEM;
	SetPlayAnimation(TEXT("ShowSystem"));
	PositionY = EPositionY::TOP;
	WBPSystemsList->SetFocusItem(EButtonsGame::NONE, CountLocationY);
}

void UMainInterface::OnClickConfigurations()
{
	Focus = EFocus::CONFIG;
	SetPlayAnimation(TEXT("AnimationShowConfiguration"));
	WBPClassicConfigurationsInterface->SetFocusItem(EButtonsGame::NONE);
}

void UMainInterface::OnClickFavorites()
{
	const int32 NumFavorites = UClassicFunctionLibrary::CountFavorites(GameData);
	FText Message = LOCTEXT("MessageNoFavorites", "No favorites to show");

	ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].Positions.ChangeFilter();
	const EGamesFilter Filter = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].Positions.OrderBy;

	if (NumFavorites != 0 && Filter == EGamesFilter::FAVORITES_FIRST)
	{
		Message = LOCTEXT("MessageFavoritesFirst", "Show favorites first");
	}
	else if (NumFavorites != 0 && Filter == EGamesFilter::FAVORITES_ONLY)
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

void UMainInterface::OnClickInfo()
{
	if (Focus == EFocus::MAIN)
	{
		SetPlayAnimation(TEXT("ShowInfo"));
		Focus = EFocus::INFO;
	}
}

void UMainInterface::OnClickBackAction()
{
	if (PositionY == EPositionY::TOP && Focus != EFocus::MAIN)
	{
		CloseMenus();
	}
	else if (PositionY == EPositionY::CENTER || PositionY == EPositionY::TOP)
	{
		if (CountSystem != 0)
		{
			ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas = GameDataIndex;
			SetLastPositions(false);
			CountSystem = 0;
			ResetCards();
			return;
		}

		if (bDelayQuit)
		{
			SetLastPositions(false);
			if (UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0))
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

void UMainInterface::SetLastPositions(const bool bResetPositions) const
{
	FIndexPositions Positions = LoopScroll->GetScrollOffSet();
	Positions.OrderBy = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].Positions.OrderBy;
	if (bResetPositions || Positions.OrderBy != EGamesFilter::DEFAULT)
	{
		ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].Positions.DefaultValues();
		ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].Positions.OrderBy = Positions.OrderBy;
		return;
	}
	ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].Positions = Positions;
}

void UMainInterface::SetFrame()
{
	if (PositionY == EPositionY::TOP)
	{
		WBPFrame->SetIndexTop(InputLastPressed, CountSystem);
		SetHeaderButtonFocus();
	}
	const int32 FrameIndex = LoopScroll->PositionOffsetFocus;
	WBPFrame->SetFramePositionWithAnimation(FrameIndex, PositionY);
}

void UMainInterface::CloseMenus()
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
		if (WBPClassicConfigurationsInterface->bFocus)
		{
			WBPClassicConfigurationsInterface->CloseModal();
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

void UMainInterface::ShowMessage(const FText Message, const float InRate)
{
	MessageDisplay->ShowMessage(Message, InRate);
}

void UMainInterface::CardIndex(int32 CardIndex, EButtonsGame Input)
{
	SetTitle(CardIndex);
	LoadImages(Input,GameData, 32);
	SetFrame();
}

void UMainInterface::SetVisibiltyDebugButton(UButton* Button)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Button->SetVisibility(ESlateVisibility::Visible);
	bDebug = true;
#endif
}

#undef LOCTEXT_NAMESPACE
