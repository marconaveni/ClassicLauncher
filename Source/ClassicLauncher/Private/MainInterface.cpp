// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "MainInterface.h"
#include "Card.h"
#include "ClassicButtonSystem.h"
#include "Components/Scrollbox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "ClassicFunctionLibrary.h"
#include "ClassicGameinstance.h"
#include "ClassicSaveGame.h"
#include "ClassicButton.h"
#include "ClassicButtonsIcons.h"
#include "ClassicInfoInterface.h"
#include "ClassicSystemListInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"
#include "ClassicMediaPlayer.h"
#include "ClassicLibretroTV.h"
#include "ClassicConfigurations.h"
#include "ClassicGameMode.h"
#include "Cover.h"
#include "ToolTip.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "EngineUtils.h"
#include "MessageBalloon.h"
#include "LoopScrollBox.h"
#include "Frame.h"
#include "LoadingGameData.h"
#include "TextBoxScroll.h"
#include "TextImageBlock.h"
//#include "Misc/OutputDeviceNull.h"

#define DEFAULTFIRSTDELAY  0.18f;
#define LOCTEXT_NAMESPACE "ButtonsSelection"

UMainInterface::UMainInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TimerDelayNavigation = 0.09f;

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
	TextCenter.Add(LOCTEXT("buttonExit", "Exit"));
	TextCenter.Add(LOCTEXT("buttonFavorite", "Favorite"));
	TextCenter.Shrink();

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

	SlotToolTipSystem = Cast<UCanvasPanelSlot>(WBPToolTipSystem->Slot);
	SlotToolTipConfiguration = Cast<UCanvasPanelSlot>(WBPToolTipConfiguration->Slot);
	SlotToolTipFavorites = Cast<UCanvasPanelSlot>(WBPToolTipFavorites->Slot);
	SlotToolTipInfo = Cast<UCanvasPanelSlot>(WBPToolTipInfo->Slot);

	BtnSelectSystem->OnFocusTrigger.AddDynamic(this, &UMainInterface::OnFocusSelectSystem);
	BtnSelectSystem->OnFocusLostTrigger.AddDynamic(this, &UMainInterface::OnLostFocusSelectSystem);
	BtnSelectSystem->OnClickTrigger.AddDynamic(this, &UMainInterface::OnClickSelectSystem);

	BtnConfigurations->OnFocusTrigger.AddDynamic(this, &UMainInterface::OnFocusConfigurations);
	BtnConfigurations->OnFocusLostTrigger.AddDynamic(this, &UMainInterface::OnLostFocusConfigurations);
	BtnConfigurations->OnClickTrigger.AddDynamic(this, &UMainInterface::OnClickConfigurations);

	BtnFavorites->OnFocusTrigger.AddDynamic(this, &UMainInterface::OnFocusFavorites);
	BtnFavorites->OnFocusLostTrigger.AddDynamic(this, &UMainInterface::OnLostFocusFavorites);
	BtnFavorites->OnClickTrigger.AddDynamic(this, &UMainInterface::OnClickFavorites);

	BtnInfo->OnFocusTrigger.AddDynamic(this, &UMainInterface::OnFocusInfo);
	BtnInfo->OnFocusLostTrigger.AddDynamic(this, &UMainInterface::OnLostFocusInfo);
	BtnInfo->OnClickTrigger.AddDynamic(this, &UMainInterface::OnClickInfo);

	for (TActorIterator<AClassicMediaPlayer> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		ClassicMediaPlayerReference = *ActorIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference AClassicMediaPlayer Founds: %s "), *ClassicMediaPlayerReference->GetName());
	}

	for (TActorIterator<AClassicLibretroTV> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		ClassicLibretroTVReference = *ActorIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference AClassicLibretroTV Founds: %s "), *ClassicLibretroTVReference->GetName());
	}

	WBPFrame->SetDefaultValues(1, DefaultFrameSpeed);
	ClassicMediaPlayerReference->MainInterfaceReference = this;
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UMainInterface::TimerTick, 0.015f, true, -1);

	SetRenderOpacityList();
}

void UMainInterface::TimerTick()
{
	if (bKeyPressed)
	{
		NavigationGame(ENavigationLastButton);

		if (PositionY == EPositionY::TOP)
		{
			WBPInfo->ScrollTopEnd(ENavigationLastButton);
		}
		if (PositionY == EPositionY::BOTTOM)
		{
			WBPTextBoxScroll->SetNewScroll(ENavigationLastButton, 0.0025f);
		}
	}
	if (bKeyPressed && PositionY == EPositionY::CENTER && (ENavigationLastButton == EButtonsGame::LEFT || ENavigationLastButton == EButtonsGame::RIGHT))
	{
		SpeedScroll = FMath::Clamp(SpeedScroll - 0.001f, DefaultMinSpeedScroll, DefaultSpeedScroll);
		LoopScroll->Speed = SpeedScroll;
	}
	else if (bKeyPressed && PositionY == EPositionY::CENTER && (ENavigationLastButton == EButtonsGame::LB || ENavigationLastButton == EButtonsGame::RB))
	{
		LoopScroll->Speed = DefaultTriggerSpeedScroll;
	}
	else
	{
		SpeedScroll = DefaultSpeedScroll;
		LoopScroll->Speed = SpeedScroll;
	}
}

void UMainInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ENavigationBack == EButtonsGame::SELECT && ENavigationA == EButtonsGame::A && ENavigationLB == EButtonsGame::LB && ENavigationRB == EButtonsGame::RB && ProcessID != 0)
	{
		TArray<FString> TextArguments;
		TextArguments.Add(TEXT("  /PID   "));
		TextArguments.Add(FString::FromInt(ProcessID));
		int32 Proc;
		UClassicFunctionLibrary::CreateProcess(Proc, TEXT("taskkill  "), TextArguments, false, true, 0, TEXT(""));
	}

	if (UClassicFunctionLibrary::ClassicIsApplicationRunning(ProcessID))
	{
		if (!bIsRunning)
		{
			UClassicFunctionLibrary::PauseProcess(1.5f);
			RunningGame(true);
		}
		bIsRunning = true;
	}
	else
	{
		if (bIsRunning)
		{
			RunningGame(false);
		}
		bIsRunning = false;
	}

}

void UMainInterface::SetCenterText(const FText Message)
{
	MessageCenter->SetVisibility(ESlateVisibility::Visible);
	MessageCenter->SetText(Message);
	OnSetCenterText(Message);
}

void UMainInterface::AddSystems(TArray<FGameSystem> Systems)
{
	WBPSystemsList->ScrollBoxSystems->ClearChildren();
	GameSystems.Empty();
	ButtonSystemReferences.Empty();

	if (ButtonSystemClass != nullptr)
	{
		UClassicButtonSystem* ButtonSystem = nullptr;
		for (int32 i = 0; i < Systems.Num(); i++)
		{
			ButtonSystem = CreateWidget<UClassicButtonSystem>(GetOwningPlayer(), ButtonSystemClass);
			ButtonSystem->OnClickTrigger.AddDynamic(this, &UMainInterface::OnClickSystem);
			ButtonSystem->SetText(Systems[i].SystemLabel);
			ButtonSystem->SetCount(i);
			ButtonSystemReferences.Add(ButtonSystem);
			WBPSystemsList->ScrollBoxSystems->AddChild(ButtonSystem);
		}
		GameSystems = Systems;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("buttonSystemClass Not Found"));
	}
}

void UMainInterface::LoadGamesList()
{
	TArray<FGameSystem>Systems = ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave;

	int32 NumFavorites = 0;
	GameData = UClassicFunctionLibrary::FilterGameData(Systems[CountSystem].GameDatas, Systems[CountSystem].Positions.OrderBy, NumFavorites);
	if (NumFavorites == 0)
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
		GetWorld()->GetTimerManager().SetTimer(InitializeTimerHandle, this, &UMainInterface::OnLoadGamesList, 0.1f, false, -1);
	}
	else
	{
		FFormatNamedArguments Args;
		Args.Add("GameRoot", FText::FromString(Systems[CountSystem].RomPath));
		SetCenterText(FText::Format(LOCTEXT("LogGameListNotFound", "gamelist.xml not found in {GameRoot}"), Args));
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

		int32 IndexLoad = -1, IndexUnLoad = -1;
		if (ENavigationButton == EButtonsGame::RIGHT || ENavigationButton == EButtonsGame::RB)
		{
			IndexUnLoad = FirstIndex;
			IndexLoad = LastIndex;
		}
		else if (ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::LB)
		{
			IndexUnLoad = LastIndex;
			IndexLoad = FirstIndex;
		}
		if (IndexLoad != -1 && IndexUnLoad != -1)
		{
			LoopScroll->AddImagesCards(ImageNull, 1, 1, IndexUnLoad);
			LoopScroll->CoverReference[IndexUnLoad]->SetCoverImage(ImageNull, 1, 1);
			OnLoadImages(IndexLoad, GameData[IndexLoad].imageFormated);
			UE_LOG(LogTemp, Warning, TEXT("FirstIndex %d  IndexCard %d LastIndex %d"), FirstIndex, IndexCard, LastIndex);
		}

	}
}

void UMainInterface::ShowGames()
{
	PlayAnimationForward(LoadListGame);
	bInputEnable = true;
	PrepareThemes();
	OnShowGames(); //BlueprintImplementableEvent
	GameMode->LoadingGameData->RemoveLoadingScreenToParent();
}

void UMainInterface::PrepareThemes()
{
	OnPrepareThemes(); //BlueprintImplementableEvent
}


FReply UMainInterface::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bInputEnable)
	{
		KeyEvent = InKeyEvent;
		const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);

		if (ENavigationLastButton == EButtonsGame::NONE && Input != EButtonsGame::A)
		{
			ENavigationLastButton = Input;
		}

		bKeyPressed = (Input != EButtonsGame::A && Input != EButtonsGame::NONE);

		if (!bScroll)
		{
			if (Input == EButtonsGame::B || Input == EButtonsGame::Y)
			{
				ENavigationButton = Input;
				OnClickFavorite();
			}
		}
	}
	else
	{
		switch (UClassicFunctionLibrary::GetInputButton(InKeyEvent))
		{
		case EButtonsGame::SELECT: ENavigationBack = EButtonsGame::SELECT; break;
		case EButtonsGame::A: ENavigationA = EButtonsGame::A; break;
		case EButtonsGame::LB: ENavigationLB = EButtonsGame::LB; break;
		case EButtonsGame::RB: ENavigationRB = EButtonsGame::RB; break;
		default: break;
		}
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UMainInterface::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	KeyEvent = InKeyEvent;
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
	ENavigationLastButton = EButtonsGame::NONE;
	LoopScroll->CancelScroll();
	FirstDelayNavigation = DEFAULTFIRSTDELAY;

	if (bInputEnable)
	{
		bKeyPressed = false;
		bUpDownPressed = true;
		bDelayFavoriteClick = false;

		if (Input == EButtonsGame::B)
		{
			OnClickBackAction();
		}
		if (Input == EButtonsGame::M)
		{
			ClassicMediaPlayerReference->PlayMusic();
		}
	}
	else
	{
		ENavigationBack = EButtonsGame::NONE;
		ENavigationA = EButtonsGame::NONE;
		ENavigationLB = EButtonsGame::NONE;
		ENavigationRB = EButtonsGame::NONE;
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

FReply UMainInterface::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float ScrollScale = InMouseEvent.GetWheelDelta();

	if (PositionY == EPositionY::BOTTOM)
	{
		if (ScrollScale > 0)
		{
			WBPTextBoxScroll->SetNewScroll(EButtonsGame::SCROLLUP, 0.0025f * 4);
		}
		else if (ScrollScale < 0)
		{
			WBPTextBoxScroll->SetNewScroll(EButtonsGame::SCROLLDOWN, 0.0025f * 4);
		}
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}


void UMainInterface::OnAnimationStartedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationStartedPlaying(Player);
	//const UWidgetAnimation* AnimationGet = Player.GetAnimation();
}

void UMainInterface::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	//const UWidgetAnimation* AnimationGet = Player.GetAnimation();
}

FReply UMainInterface::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnPreventLoseFocus();
	return  Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UMainInterface::OnPreventLoseFocus()
{
	if (Focus == EFocus::MAIN)
	{
		if (PositionY == EPositionY::TOP)
		{
			SetTopButtonFocus();
			return;
		}
		LoopScroll->BtnClick->SetKeyboardFocus();
	}
	else if (Focus == EFocus::SYSTEM || Focus == EFocus::INFO)
	{
		if (ButtonSystemReferences.IsValidIndex(IndexCard))
		{
			ButtonSystemReferences[CountLocationY]->Click->SetKeyboardFocus();
		}
	}
	else if (Focus == EFocus::CONFIG)
	{
		WBPClassicConfigurationsInterface->SetFocusSelect();
	}
}

/////////////////////////////////////////////////
///navigation area

#pragma region NavigationArea



void UMainInterface::NavigationGame(EButtonsGame Navigate)
{
	if (Navigate == EButtonsGame::UP || Navigate == EButtonsGame::DOWN || Navigate == EButtonsGame::LEFT || Navigate == EButtonsGame::RIGHT || Navigate == EButtonsGame::LB || Navigate == EButtonsGame::RB)
	{
		if (!bScroll && bInputEnable && bDelayPressed && bUpDownPressed)
		{
			OnNavigationGame(Navigate); //BlueprintImplementableEvent
			if (Focus == EFocus::MAIN) {
				NavigationMain(Navigate);
			}
			else if (Focus == EFocus::SYSTEM) {
				NavigationSystem(Navigate);
			}
			else if (Focus == EFocus::INFO) {
				NavigationInfo(Navigate);
			}
			else if (Focus == EFocus::CONFIG) {
				NavigationConfiguration(Navigate);
			}
		}
	}
}

void UMainInterface::NavigationMain(EButtonsGame Navigate)
{
	PressedDelayNavigation(TimerDelayNavigation + FirstDelayNavigation);
	FirstDelayNavigation = 0.0f;
	ENavigationButton = Navigate;
	if (ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::RIGHT)
	{
		if (PositionY == EPositionY::TOP)
		{
			SetNavigationFocusTop();
		}
		else if (PositionY == EPositionY::CENTER)
		{
			SetDirection(ENavigationButton);
		}
	}
	else if (ENavigationButton == EButtonsGame::LB || ENavigationButton == EButtonsGame::RB)
	{
		if (PositionY == EPositionY::CENTER)
		{
			SetDirection(ENavigationButton);
		}
	}
	else if (ENavigationButton == EButtonsGame::UP)
	{
		bUpDownPressed = false;
		SetNavigationFocusUpBottom();
	}
	else if (ENavigationButton == EButtonsGame::DOWN)
	{
		bUpDownPressed = false;
		SetNavigationFocusDownBottom();
	}
}

void UMainInterface::NavigationSystem(EButtonsGame Navigate)
{
	PressedDelayNavigation(TimerDelayNavigation + FirstDelayNavigation);
	FirstDelayNavigation = 0.0f;
	ENavigationButton = Navigate;
	WBPSystemsList->SetFocusItem(ENavigationButton, CountLocationY, ButtonSystemReferences);
}

void UMainInterface::NavigationInfo(EButtonsGame Navigate)
{
	PressedDelayNavigation(TimerDelayNavigation + FirstDelayNavigation);
	FirstDelayNavigation = 0.0f;
}

void UMainInterface::NavigationConfiguration(EButtonsGame Navigate)
{
	if (Navigate != EButtonsGame::LEFT || Navigate != EButtonsGame::RIGHT)
	{
		PressedDelayNavigation(TimerDelayNavigation + FirstDelayNavigation);
		FirstDelayNavigation = 0.0f;
	}
	WBPClassicConfigurationsInterface->SetIndexFocus(Navigate);
}

void UMainInterface::SetTitle(int32 Index)
{
	if (GameData.IsValidIndex(Index))
	{
		IndexCard = Index;
		const FString Title = GameData[IndexCard].nameFormated;
		TextTitleGame->SetText(FText::FromString(Title));
		WBPTextBoxScroll->SetText(GameData[IndexCard].descFormated);
		SetButtonsIconInterfaces(PositionY);
	}
}

void UMainInterface::SetDirection(EButtonsGame Navigate)
{
	if (Navigate == EButtonsGame::RIGHT || Navigate == EButtonsGame::RB)
	{
		LoopScroll->StartScrollTo(EButtonsGame::RIGHT);
	}
	else if (Navigate == EButtonsGame::LEFT || Navigate == EButtonsGame::LB)
	{
		LoopScroll->StartScrollTo(EButtonsGame::LEFT);
	}
}

void UMainInterface::SetNavigationFocusTop()
{
	if (ENavigationButton == EButtonsGame::LEFT)
	{
		if (BtnInfo->BtButton->HasKeyboardFocus())
		{
			BtnFavorites->BtButton->SetKeyboardFocus();
		}
		else if (BtnFavorites->BtButton->HasKeyboardFocus())
		{
			BtnConfigurations->BtButton->SetKeyboardFocus();
		}
		else if (BtnConfigurations->BtButton->HasKeyboardFocus())
		{
			BtnSelectSystem->BtButton->SetKeyboardFocus();
		}
	}
	else if (ENavigationButton == EButtonsGame::RIGHT)
	{
		if (BtnSelectSystem->BtButton->HasKeyboardFocus())
		{
			BtnConfigurations->BtButton->SetKeyboardFocus();
		}
		else if (BtnConfigurations->BtButton->HasKeyboardFocus())
		{
			BtnFavorites->BtButton->SetKeyboardFocus();
		}
		else if (BtnFavorites->BtButton->HasKeyboardFocus())
		{
			BtnInfo->BtButton->SetKeyboardFocus();
		}
	}
}

void UMainInterface::SetNavigationFocusUpBottom()
{
	if (PositionY == EPositionY::BOTTOM)
	{
		if (BackgroundVideo->RenderOpacity == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(StartVideoTimerHandle);
			PlayAnimationReverse(ShowDescBottomInfo);
			PositionY = EPositionY::CENTER;
			ClassicMediaPlayerReference->StopVideo();
			ClassicMediaPlayerReference->ResumeMusic();
			WBPTextBoxScroll->CancelAutoScroll();
			StopAnimation(FadeChangeImageToVideo);
			UE_LOG(LogTemp, Warning, TEXT("Close frame bottom"));
		}
		else
		{
			PlayAnimationReverse(VideoAnimation);
		}
	}
	else if (PositionY == EPositionY::CENTER)
	{
		PositionY = EPositionY::TOP;
		PlayAnimationForward(BarTop);
		SetButtonsIconInterfaces(PositionY);

		SetTopButtonFocus();
	}
}

void UMainInterface::SetNavigationFocusDownBottom()
{
	if (PositionY == EPositionY::CENTER)
	{
		if (PositionY != EPositionY::BOTTOM)
		{
			GetWorld()->GetTimerManager().ClearTimer(StartVideoTimerHandle);
			StartVideoTimerHandle.Invalidate();
			PositionY = EPositionY::BOTTOM;
			SetImageBottom();
			WBPTextBoxScroll->StartScroll();
			PlayAnimationForward(ShowDescBottomInfo);
			GetWorld()->GetTimerManager().SetTimer(StartVideoTimerHandle, this, &UMainInterface::StartVideo, 5.0f, false, -1);
			UE_LOG(LogTemp, Warning, TEXT("Open frame bottom"));
		}
	}
	else if (PositionY == EPositionY::BOTTOM)
	{
		if (BackgroundVideo->RenderOpacity == 0)
		{
			PlayAnimationForward(VideoAnimation);
		}
	}
	else
	{
		PositionY = EPositionY::CENTER;
		LoopScroll->BtnClick->SetKeyboardFocus();
		PlayAnimationReverse(BarTop);
		SetButtonsIconInterfaces(PositionY);
	}
}

void UMainInterface::SetTopButtonFocus()
{
	switch (LoopScroll->PositionTopX)
	{
	case 1:
		BtnSelectSystem->BtButton->SetKeyboardFocus();
		break;
	case 2:
		BtnConfigurations->BtButton->SetKeyboardFocus();
		break;
	case 3:
		BtnFavorites->BtButton->SetKeyboardFocus();
		break;
	case 4:
		BtnInfo->BtButton->SetKeyboardFocus();
		break;
	default:
		break;
	}
}

//end navigate area
///////////////////////////////////////////////////

#pragma endregion NavigationArea

void UMainInterface::OnClickLaunch()
{
	if (PositionY == EPositionY::CENTER && bInputEnable)
	{
		SetCountPlayerToSave();
		LoopScroll->OpenCard();
		PlayAnimationForward(FadeStartSystem);
		GetWorld()->GetTimerManager().SetTimer(LauncherTimerHandle, this, &UMainInterface::AppLaunch, 1.0f, false, -1);
	}
}

void UMainInterface::AppLaunch()
{
	const FString PathRomFormated = UClassicFunctionLibrary::HomeDirectoryReplace(GameData[IndexCard].PathFormated);
	const FString ExecutablePath = (GameData[IndexCard].Executable == TEXT("")) ? GameSystems[CountSystem].Executable : GameData[IndexCard].Executable;
	const FString Arguments = (GameData[IndexCard].Arguments == TEXT("")) ? GameSystems[CountSystem].Arguments : GameData[IndexCard].Arguments;
	bool CanUnzip = false;
	FString FormatedCore;

	if (UClassicFunctionLibrary::SwitchOnDefaultLibreto(ExecutablePath, FormatedCore, CanUnzip))
	{
		OpenLibretro(FormatedCore, PathRomFormated, CanUnzip);
		UE_LOG(LogTemp, Warning, TEXT("RomPath %s , CorePath %s , CanUnzip %s"), *PathRomFormated, *FormatedCore, (CanUnzip ? TEXT("true") : TEXT("false")));
	}
	else
	{
		TArray<FString> Commands;
		if (!Arguments.IsEmpty()) {
			Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(Arguments));
		}
		if (!ExecutablePath.IsEmpty()) {
			Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(PathRomFormated));
		}
		OpenExternalProcess(ExecutablePath, Commands);
		UE_LOG(LogTemp, Warning, TEXT("OpenExternalProcess: %s %s %s"), *ExecutablePath, *Arguments, *PathRomFormated);
	}
}

void UMainInterface::OpenLibretro(const FString CorePath, const FString RomPath, const bool CanUnzip)
{
	if (ClassicLibretroTVReference != nullptr)
	{
		ClassicLibretroTVReference->OnNativeLoadRom(CorePath, RomPath, CanUnzip);
		SetVisibility(ESlateVisibility::Hidden);
		GameMode->GameSettingsRunningInternal();
		RunningGame(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error ClassicLibretroTVReference is null"));
	}
}

void UMainInterface::OpenExternalProcess(FString ExecutablePath, TArray<FString> CommandArgs)
{
	UClassicFunctionLibrary::CreateProcess(ProcessID, ExecutablePath, CommandArgs, false, false);
	GameMode->GameSettingsRunning();
}

void UMainInterface::OnClickSystem(int32 Value)
{
	if (bInputEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("The OnClickSystem parameter value is: %d"), Value);
		if (CountSystem == Value)
		{
			PlayAnimationReverse(ShowSystem);
			Focus = EFocus::MAIN;
			PositionY = EPositionY::CENTER;
			LoopScroll->BtnClick->SetKeyboardFocus();
			PlayAnimationReverse(BarTop);
			SetButtonsIconInterfaces(PositionY);
			WBPFrame->SetFramePosition(WBPFrame->FrameIndexCenter, EFocusTop::NONE);
			return;
		}
		SetLastPositions(false);
		CountSystem = Value;   //CountSystem = CountLocationY;
		ResetCards(true, true);
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
	else if (GetPosition == EPositionY::CENTER)
	{
		if (GameData.IsValidIndex(IndexCard) && TextCenter.IsValidIndex(5))
		{
			TextCenter[5] = (GameData[IndexCard].favorite) ? LOCTEXT("buttonRemoveFavorite", "Remove Favorite") : LOCTEXT("buttonRemoveFavoriteAddFavorite", "Add Favorite");
		}

		WBPButtonsIconsInterfaces->SetTexts(TextCenter);
		WBPButtonsIconsInterfaces->SetButtonsVisibility(IconCenter);
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
	if (bDelayFavoriteClick && ENavigationButton == EButtonsGame::Y && !bScroll)
	{

		AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [=]()
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

					AsyncTask(ENamedThreads::GameThread, [=]()
					{

						UCard* Left;
						UCard* Center;
						UCard* Right;
						LoopScroll->GetCardReferences(IndexCard, Left, Center, Right);
						Left->SetFavorite(ToggleFavorite, true);
						Center->SetFavorite(ToggleFavorite, true);
						Right->SetFavorite(ToggleFavorite, true);
						SetButtonsIconInterfaces(EPositionY::CENTER);

						ShowMessage((ToggleFavorite) ? LOCTEXT("MessageAddFavorite", "Add game to favorite") : LOCTEXT("RemoveFavorite", "Remove game to favorite"), 3.5f);

						//FOutputDeviceNull OutputDeviceNull;
						//this->CallFunctionByNameWithArguments(TEXT("ResetCache"), OutputDeviceNull, nullptr, true);
					});
				}
			}
		});
	}
}


void UMainInterface::RunningGame(bool bIsRun)
{
	if (bIsRun)
	{
		ClassicMediaPlayerReference->PauseMusic();
		LoopScroll->SetVisibility(ESlateVisibility::Hidden);
		BtnSelectSystem->SetVisibility(ESlateVisibility::Hidden);
		BtnConfigurations->SetVisibility(ESlateVisibility::Hidden);
		BtnFavorites->SetVisibility(ESlateVisibility::Hidden);
		BtnInfo->SetVisibility(ESlateVisibility::Hidden);
		bInputEnable = false;
	}
	else
	{
		ClassicMediaPlayerReference->PlayMusic();
		LoopScroll->SetVisibility(ESlateVisibility::Visible);
		BtnSelectSystem->SetVisibility(ESlateVisibility::Visible);
		BtnConfigurations->SetVisibility(ESlateVisibility::Visible);
		BtnFavorites->SetVisibility(ESlateVisibility::Visible);
		BtnInfo->SetVisibility(ESlateVisibility::Visible);
		GameMode->GameSettingsInit();
		bInputEnable = true;
		ProcessID = 0;
		PlayAnimationReverse(FadeStartSystem);

	}
}

///////////////////////////////////////////////////////////////////////

//pressed delay
void UMainInterface::PressedDelayNavigation(float Delay)
{
	bDelayPressed = false;
	GetWorld()->GetTimerManager().SetTimer(DelayPressedTimerHandle, this, &UMainInterface::PressedTimerNavigation, Delay, false, -1);
}
//timer
void UMainInterface::PressedTimerNavigation()
{
	bDelayPressed = true;
}

///////////////////////////////////////////////////////////////////////

void UMainInterface::SetRenderOpacityList() {

	TextTitleGame->SetRenderOpacity(0.f);
	BgTitle->SetRenderOpacity(0.f);
	LoopScroll->SetRenderOpacity(0.f);
	WBPFrame->SetRenderOpacity(0.f);
	MessageCenter->SetVisibility(ESlateVisibility::Hidden);
	MessageCenter->SetText(FText::FromString(""));
}

void UMainInterface::ResetCards(const bool bAnimationBarTop, const bool bAnimationShowSystem)
{
	bInputEnable = false;

	MessageCenter->SetVisibility(ESlateVisibility::Hidden);
	MessageCenter->SetText(FText::FromString(""));

	PlayAnimationReverse(LoadListGame);

	Focus = EFocus::MAIN;
	GameData.Empty();
	IndexCard = 0;
	PositionY = EPositionY::CENTER;
	DescriptionScrollScale = 0.f;

	bUpDownPressed = true;
	bDelayPressed = true;

	if (bAnimationBarTop)
	{
		PlayAnimationReverse(BarTop);
	}
	if (bAnimationShowSystem)
	{
		PlayAnimationReverse(ShowSystem);
	}

	SetButtonsIconInterfaces(EPositionY::CENTER);
	GetWorld()->GetTimerManager().SetTimer(DelayPressedTimerHandle, this, &UMainInterface::LoadGamesList, 0.3f, false, -1);
}

void UMainInterface::Clear()
{
	IndexCard = 0;
	ProcessID = 0;
	ENavigationLastButton = EButtonsGame::NONE;
	ENavigationButton = EButtonsGame::NONE;
	ENavigationScroll = EButtonsGame::NONE;
	ENavigationBack = EButtonsGame::NONE;
	ENavigationA = EButtonsGame::NONE;
	PositionY = EPositionY::CENTER;
	Focus = EFocus::MAIN;
	bDelayPressed = true;
	bKeyPressed = false;
	bUpDownPressed = true;
	bInputEnable = false;
	bScroll = false;
	bDelayFavoriteClick = false;
	bDelayQuit = false;
	bIsRunning = false;
	FirstDelayNavigation = DEFAULTFIRSTDELAY;
	CountSystem = 0;
	CountLocationY = 0;
	DescriptionScrollScale = 0.f;
	SpeedScroll = DefaultSpeedScroll;

	GameData.Empty();
	GameDataIndex.Empty();
}

//bind buttons
void UMainInterface::OnFocusSelectSystem()
{
	ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas = GameDataIndex;
	FocusButtonsTop(1, WBPToolTipSystem, SlotToolTipSystem, WBPFrame->MoveLeftRightTop1, nullptr, EFocusTop::SYSTEM);
}

void UMainInterface::OnFocusConfigurations()
{
	FocusButtonsTop(2, WBPToolTipConfiguration, SlotToolTipConfiguration, WBPFrame->MoveLeftRightTop2, WBPFrame->MoveLeftRightTop1, EFocusTop::CONFIG);
}

void UMainInterface::OnFocusFavorites()
{
	ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave[CountSystem].GameDatas = GameDataIndex;
	FocusButtonsTop(3, WBPToolTipFavorites, SlotToolTipFavorites, WBPFrame->MoveLeftRightTop3, WBPFrame->MoveLeftRightTop2, EFocusTop::FAVORITE);
}

void UMainInterface::OnFocusInfo()
{
	WBPInfo->SetGameInfo(GameData[IndexCard]);
	FocusButtonsTop(4, WBPToolTipInfo, SlotToolTipInfo, nullptr, WBPFrame->MoveLeftRightTop3, EFocusTop::INFO);
}

void UMainInterface::FocusButtonsTop(const int32 PositionTopX, UToolTip* ToolTip, UCanvasPanelSlot* ToolTipSlot, UWidgetAnimation* Left, UWidgetAnimation* Right, const EFocusTop FocusTop)
{
	LoopScroll->PositionTopX = PositionTopX;
	SetZOrderToolTips(ToolTipSlot);
	ToolTip->SetToolTipVisibility(ESlateVisibility::Visible);
	ToolTip->SetVisibility(ESlateVisibility::Visible);
	if (ENavigationButton == EButtonsGame::LEFT && Left != nullptr)
	{
		WBPFrame->PlayAnimationReverse(Left);
	}
	else if (ENavigationButton == EButtonsGame::RIGHT && Right != nullptr)
	{
		WBPFrame->PlayAnimationForward(Right);
	}
	else if (ENavigationButton == EButtonsGame::UP && WBPFrame->ImageFrameCenter->RenderTransform.Translation.Y == 0)
	{
		WBPFrame->AnimationToTopDown(FocusTop, false);
	}
}

void UMainInterface::LostFocusButtonsTop(UToolTip* ToolTip, const EFocusTop FocusTop)
{
	ToolTip->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B && Focus == EFocus::MAIN)
	{
		WBPFrame->AnimationToTopDown(FocusTop, true);
	}
}

void UMainInterface::SetZOrderToolTips(UCanvasPanelSlot* ToolTipSlot) const
{
	SlotToolTipSystem->SetZOrder(50);
	SlotToolTipConfiguration->SetZOrder(50);
	SlotToolTipFavorites->SetZOrder(50);
	SlotToolTipInfo->SetZOrder(50);
	ToolTipSlot->SetZOrder(51);
}

void UMainInterface::OnLostFocusSelectSystem()
{
	LostFocusButtonsTop(WBPToolTipSystem, EFocusTop::SYSTEM);
}

void UMainInterface::OnLostFocusConfigurations()
{
	LostFocusButtonsTop(WBPToolTipConfiguration, EFocusTop::CONFIG);
}

void UMainInterface::OnLostFocusFavorites()
{
	LostFocusButtonsTop(WBPToolTipFavorites, EFocusTop::FAVORITE);
}

void UMainInterface::OnLostFocusInfo()
{
	LostFocusButtonsTop(WBPToolTipInfo, EFocusTop::INFO);
}

void UMainInterface::OnClickSelectSystem()
{
	Focus = EFocus::SYSTEM;
	PlayAnimationForward(ShowSystem);
	PositionY = EPositionY::TOP;
	WBPSystemsList->SetFocusItem(EButtonsGame::NONE, CountLocationY, ButtonSystemReferences);
}

void UMainInterface::SetLastPositions(bool bResetPositions)
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

void UMainInterface::OnClickConfigurations()
{
	Focus = EFocus::CONFIG;
	ENavigationButton = EButtonsGame::NONE;
	PlayAnimationForward(AnimationShowConfiguration);
	WBPClassicConfigurationsInterface->SetFocusSelect();
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
	PlayAnimationReverse(BarTop);
	BtnFavorites->SetFocusButton(false);
	ResetCards(false, false);
}

void UMainInterface::OnClickInfo()
{
	const float TranslationInfo = CanvasPanelInfo->RenderTransform.Translation.Y;
	if (TranslationInfo != 0)
	{
		PlayAnimationForward(ShowInfo);
		Focus = EFocus::INFO;
	}
}

void UMainInterface::OnClickBackAction()
{
	if (PositionY == EPositionY::TOP)
	{
		CloseMenus();
	}
	else if (PositionY == EPositionY::CENTER)
	{
		if (bDelayQuit)
		{
			SetLastPositions(false);
			if (UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0))
			{
				UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
			}
		}
		bDelayQuit = true;
		GetWorld()->GetTimerManager().SetTimer(DelayQuitTimerHandle, this, &UMainInterface::QuitGame, 3.f, false, -1);
		ShowMessage(LOCTEXT("PressAgainToQuit", "Press Again To Quit"), 3);
	}
	else if (PositionY == EPositionY::BOTTOM)
	{
		SetNavigationFocusUpBottom();
	}
}

void UMainInterface::QuitGame()
{
	bDelayQuit = false;
}

void UMainInterface::OnClickFavorite()
{
	if (bInputEnable && PositionY == EPositionY::CENTER && !bDelayFavoriteClick && !bScroll)
	{
		bDelayFavoriteClick = true;
		GetWorld()->GetTimerManager().SetTimer(DelayFavoriteTimerHandle, this, &UMainInterface::SetFavoriteToSave, 0.5f, false, -1);
	}
}

void UMainInterface::CloseMenus()
{
	const float TranslationSystemSelect = CanvasPanelSystemSelect->RenderTransform.Translation.Y;
	const float TranslationInfo = CanvasPanelInfo->RenderTransform.Translation.Y;
	const float TranslationConfiguration = CanvasPanelConfiguration->RenderTransform.Translation.Y;

	if (TranslationSystemSelect == 0)
	{
		PlayAnimationReverse(ShowSystem);
		BtnSelectSystem->BtButton->SetKeyboardFocus();
		Focus = EFocus::MAIN;
	}
	else if (TranslationConfiguration == 0)
	{
		ENavigationButton = EButtonsGame::NONE;
		if (WBPClassicConfigurationsInterface->bFocus)
		{
			WBPClassicConfigurationsInterface->CloseModal();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(BackButtonTimerHandle, this, &UMainInterface::CloseBackMenu, 0.1f, false, -1);
			Focus = EFocus::MAIN;
		}
	}
	else if (TranslationInfo == 0)
	{
		PlayAnimationReverse(ShowInfo);
		BtnInfo->BtButton->SetKeyboardFocus();
		Focus = EFocus::MAIN;
	}
	else
	{
		SetNavigationFocusDownBottom();
	}
}

void UMainInterface::CloseBackMenu()
{
	PlayAnimationReverse(AnimationShowConfiguration);
	BtnConfigurations->BtButton->SetKeyboardFocus();
}

void UMainInterface::ShowMessage(const FText Message, const float InRate)
{
	MessageDisplay->ShowMessage(Message, InRate);
}

void UMainInterface::SetVisibiltyDebugButton(UButton* Button)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Button->SetVisibility(ESlateVisibility::Visible);
#endif
}

void UMainInterface::SetImageBottom()
{
	if (ImgVideo == nullptr || ImgImageBottom == nullptr) return;

	const float TranslationPanelBottom = CanvasPanelBottom->RenderTransform.Translation.Y;
	if (TranslationPanelBottom != 0)
	{
		PlayAnimationReverse(FadeChangeImageToVideo);
	}
	else
	{
		//Change visibility ScaleBoxImage and ScaleBoxVideo
		PlayAnimationForward(ChangeVideoToImage);
	}

	if (!GameData.IsValidIndex(IndexCard)) return;

	const FString ImagePath = GameData[IndexCard].thumbnailFormated;

	if (FPaths::FileExists(ImagePath))
	{
		int32 Width = 640;
		int32 Height = 480;
		const EClassicImageFormat Format = UClassicFunctionLibrary::GetFormatImage(ImagePath);
		UTexture2D* ImageLoaded = UClassicFunctionLibrary::LoadTexture2DFromFile(ImagePath, Format, EClassicTextureFilter::DEFAULT, Width, Height);

		if (ImageLoaded != nullptr)
		{
			FSlateBrush NewBrush;
			NewBrush.SetImageSize(FVector2D(Width * 2, Height * 2));
			NewBrush.SetResourceObject(ImageLoaded);
			ImgImageBottom->SetBrush(NewBrush);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Image not Loaded nullptr"));
		}
	}
	else
	{
		FSlateBrush NewBrush;
		NewBrush.SetImageSize(FVector2D(640, 480));
		NewBrush.SetResourceObject(ImageBottomDefault);
		ImgImageBottom->SetBrush(NewBrush);
		UE_LOG(LogTemp, Warning, TEXT("Image not exists in %s"), *ImagePath);
	}
}

void UMainInterface::StartVideo()
{
	if (PositionY != EPositionY::BOTTOM) return;

	if (GameData.IsValidIndex(IndexCard))
	{
		const FString PathVideo = GameData[IndexCard].videoFormated;
		if (FPaths::FileExists(PathVideo))
		{
			PlayAnimationForward(FadeChangeImageToVideo);
			ClassicMediaPlayerReference->PlayVideo(PathVideo);
		}
	}
}

#undef LOCTEXT_NAMESPACE
