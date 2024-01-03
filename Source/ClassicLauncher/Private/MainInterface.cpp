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

	for (TActorIterator<AClassicMediaPlayer> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		ClassicMediaPlayerReference = *ActorIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference AClassicMediaPlayer Founds: %s "), *ClassicMediaPlayerReference->GetName());
	}

	ClassicMediaPlayerReference->MainInterfaceReference = this;
	LoopScroll->MainInterfaceReference = this;
	CanvasPanelScreen->SetRenderOpacity(0);
}

void UMainInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bKeyPressed)
	{
		NavigationGame(ENavigationLastButton);

		if (PositionY == EPositionY::TOP)
		{
			WBPInfo->ScrollTopEnd(ENavigationLastButton);
		}
		if (PositionY == EPositionY::BOTTOM)
		{
			FooterDetails->TextBoxScroll->SetNewScroll(ENavigationLastButton, 0.0025f);
		}
	}

	if (bKeyPressed && PositionY == EPositionY::CENTER && (ENavigationLastButton == EButtonsGame::LEFT || ENavigationLastButton == EButtonsGame::RIGHT))
	{
		SpeedScroll = UKismetMathLibrary::Ease(InitialSpeedScroll, TargetSpeedScroll, Alpha, EEasingFunc::EaseIn);
		Alpha = FMath::Clamp(Alpha + 0.24f * GetWorld()->GetDeltaSeconds(), 0, 1);
	}
	else if (bKeyPressed && PositionY == EPositionY::CENTER && (ENavigationLastButton == EButtonsGame::LB || ENavigationLastButton == EButtonsGame::RB))
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

void UMainInterface::LoadImages(const int32 DistanceIndex)
{
	if ((ENavigationButton == EButtonsGame::RIGHT || ENavigationButton == EButtonsGame::RB ||
		ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::LB) &&
		DistanceIndex > 0 && GameData.Num() >= 64)
	{
		if (ENavigationButton == EButtonsGame::RIGHT || ENavigationButton == EButtonsGame::RB)
		{
			FirstIndex = IndexCard - DistanceIndex;
			LastIndex = IndexCard + (DistanceIndex - 1);
		}
		else if (ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::LB)
		{
			FirstIndex = IndexCard - (DistanceIndex - 1);
			LastIndex = IndexCard + DistanceIndex;
		}

		if (FirstIndex < 0)
		{
			FirstIndex = GameData.Num() - FMath::Abs(FirstIndex);
		}
		if (LastIndex >= GameData.Num())
		{
			LastIndex = LastIndex - GameData.Num();
		}

		FirstIndex = FMath::Clamp(FirstIndex, 0, GameData.Num() - 1);
		LastIndex = FMath::Clamp(LastIndex, 0, GameData.Num() - 1);

		int32 IndexLoad = -1, IndexUnload = -1;
		if (ENavigationButton == EButtonsGame::RIGHT || ENavigationButton == EButtonsGame::RB)
		{
			IndexUnload = FirstIndex;
			IndexLoad = LastIndex;
		}
		else if (ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::LB)
		{
			IndexUnload = LastIndex;
			IndexLoad = FirstIndex;
		}
		if (IndexLoad != -1 && IndexUnload != -1)
		{
			LoopScroll->CoverReference[IndexUnload]->SetCoverImage(ImageNull, 1, 1);
			OnLoadImages(IndexLoad, GameData[IndexLoad].imageFormated);
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

void UMainInterface::NativePressedInput(const FKey& InKey)
{
	Super::NativePressedInput(InKey);
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButtonsGame(InKey);
	if (GetInputEnable())
	{
		/*bKeyPressed = (Input != EButtonsGame::A && Input != EButtonsGame::NONE);

		if (ENavigationLastButton == EButtonsGame::NONE && Input != EButtonsGame::A)
		{
			ENavigationLastButton = Input;
		}*/
		bKeyPressed = (Input != EButtonsGame::NONE);
		ENavigationLastButton = Input;
		
		if (Input == EButtonsGame::B || Input == EButtonsGame::Y)
		{
			ENavigationButton = Input;
			HoldFavorite();
			if (GetInputEnable() && PositionY == EPositionY::CENTER)
			{
				GetWorld()->GetTimerManager().SetTimer(DelayFavoriteTimerHandle, this, &UMainInterface::SetFavoriteToSave, 0.5f, false, -1);
			}
		}
	}
	else
	{
		MultiInput.SetInput(Input);
	}
}

void UMainInterface::NativeReleaseInput(const FKey& InKey)
{
	Super::NativeReleaseInput(InKey);
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButtonsGame(InKey);
	ENavigationLastButton = EButtonsGame::NONE;

	if (GetInputEnable())
	{
		bKeyPressed = false;
		GetWorld()->GetTimerManager().ClearTimer(DelayFavoriteTimerHandle);
		
		if (Input == EButtonsGame::B)
		{
			OnClickBackAction();
		}
		if (Input == EButtonsGame::M)
		{
			ClassicMediaPlayerReference->PlaylistMusic();
		}
	}

	MultiInput.SetAllNoneInput();
}

FReply UMainInterface::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

FReply UMainInterface::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//OnPreventLoseFocus();
	return  Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UMainInterface::OnPreventLoseFocus()
{
	if (Focus == EFocus::MAIN)
	{
		if (PositionY == EPositionY::TOP)
		{
			const int32 Index = WBPFrame->FrameIndexTop;
			Header->SetFocusButton(Index, false);
			ToolTips->OnFocus(Index);
			return;
		}
		LoopScroll->SetCenterFocus();
	}
	else if (Focus == EFocus::SYSTEM || Focus == EFocus::INFO)
	{
		if (ButtonSystemReferences.IsValidIndex(CountLocationY))
		{
			ButtonSystemReferences[CountLocationY]->SetFocusButton(false);
		}
	}
	else if (Focus == EFocus::CONFIG)
	{
		WBPClassicConfigurationsInterface->SetFocusSelect(false);
	}
}

/////////////////////////////////////////////////
///navigation area

#pragma region NavigationArea

void UMainInterface::NavigationGame(EButtonsGame Navigate)
{
	if (Navigate == EButtonsGame::UP || Navigate == EButtonsGame::DOWN ||
		Navigate == EButtonsGame::LEFT || Navigate == EButtonsGame::RIGHT ||
		Navigate == EButtonsGame::LB || Navigate == EButtonsGame::RB)
	{

		if (GetInputEnable() && !GetDelayInput())
		{
			DelayInput(TimerDelayInput + FirstDelayInput);
			FirstDelayInput = 0.0f;
			TimerDelayInput = 0.09f;

			if (Focus == EFocus::MAIN)
			{
				NavigationMain(Navigate);
			}
			else if (Focus == EFocus::SYSTEM)
			{
				NavigationSystem(Navigate);
			}
			else if (Focus == EFocus::INFO)
			{
				NavigationInfo(Navigate);
			}
			else if (Focus == EFocus::CONFIG) {
				NavigationConfiguration(Navigate);
			}
		}
		OnNavigationGame(Navigate); //BlueprintImplementableEvent
	}
}

void UMainInterface::NavigationMain(EButtonsGame Navigate)
{
	ENavigationButton = Navigate;
	TimerDelayInput = 0.14f;
	const bool LeftRight = ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::RIGHT;
	const bool LeftRightTrigger = ENavigationButton == EButtonsGame::LB || ENavigationButton == EButtonsGame::RB;
	
	if (LeftRight || LeftRightTrigger)
	{
		if (PositionY == EPositionY::CENTER)
		{
			TimerDelayInput = 0.0f;
			if (LoopScroll->bUnlockInput)
			{
				SetDirection(ENavigationButton);
			}
		}
		else if (PositionY == EPositionY::TOP && LeftRight)
		{
			SetFrame();
		}
	}
	else if (ENavigationButton == EButtonsGame::UP && bUpDownPressed)
	{
		SetNavigationFocusUpBottom();
	}
	else if (ENavigationButton == EButtonsGame::DOWN && bUpDownPressed)
	{
		SetNavigationFocusDownBottom();
	}
}

void UMainInterface::NavigationSystem(EButtonsGame Navigate)
{
	ENavigationButton = Navigate;
	WBPSystemsList->SetFocusItem(ENavigationButton, CountLocationY);
}

void UMainInterface::NavigationInfo(EButtonsGame Navigate)
{
}

void UMainInterface::NavigationConfiguration(EButtonsGame Navigate)
{
	if (Navigate == EButtonsGame::LEFT || Navigate == EButtonsGame::RIGHT)
	{
		CancelDelay();
	}
	WBPClassicConfigurationsInterface->SetIndexFocus(Navigate);
}

void UMainInterface::SetDirection(EButtonsGame Navigate)
{
	if (Navigate == EButtonsGame::RIGHT || Navigate == EButtonsGame::RB)
	{
		LoopScroll->DirectionRight();
	}
	else if (Navigate == EButtonsGame::LEFT || Navigate == EButtonsGame::LB)
	{
		LoopScroll->DirectionLeft();
	}
}

void UMainInterface::SetNavigationFocusUpBottom()
{
	if (PositionY == EPositionY::BOTTOM)
	{
		FooterDetails->CloseFooter(PositionY, ClassicMediaPlayerReference);
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
		SetPlayAnimation(TEXT("ShowDescBottomInfo"));
	}
	else if (PositionY == EPositionY::BOTTOM)
	{
		FooterDetails->ExpandVideo();
	}
	else
	{
		PositionY = EPositionY::CENTER;
		LoopScroll->SetCenterFocus();
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
			LoopScroll->SetCenterFocus();
			SetButtonsIconInterfaces(PositionY);
			WBPFrame->SetFrameCenterPosition(WBPFrame->FrameIndexCenter);
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

void UMainInterface::HoldFavorite()
{
	if (GetInputEnable() && PositionY == EPositionY::CENTER /*&& !bDelayFavoriteClick*/)
	{
		// bDelayFavoriteClick = true;
		GetWorld()->GetTimerManager().SetTimer(DelayFavoriteTimerHandle, this, &UMainInterface::SetFavoriteToSave, 0.5f, false, -1);
	}
}

void UMainInterface::SetFavoriteToSave()
{
	if (!GameSystems.IsValidIndex(CountSystem)) return;

	if (/*bDelayFavoriteClick && */ENavigationButton == EButtonsGame::Y && !GameSystems[CountSystem].SystemName.Equals(TEXT("${System}")))
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
		ENavigationButton = EButtonsGame::NONE;

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
	ENavigationLastButton = EButtonsGame::NONE;
	ENavigationButton = EButtonsGame::NONE;
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
		WBPFrame->SetFrame(Index, PositionY);
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
	WBPClassicConfigurationsInterface->SetFocusSelect(false);
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
		WBPFrame->SetFrameIndexTop(ENavigationButton, CountSystem);
		SetHeaderButtonFocus();
	}
	const int32 FrameIndex = LoopScroll->PositionOffsetFocus;
	WBPFrame->SetFrame(FrameIndex, PositionY);
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
		ENavigationButton = EButtonsGame::NONE;
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

void UMainInterface::SetVisibiltyDebugButton(UButton* Button)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Button->SetVisibility(ESlateVisibility::Visible);
	bDebug = true;
#endif
}

#undef LOCTEXT_NAMESPACE
