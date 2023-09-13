// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "MainInterface.h"
#include "Card.h"
#include "Cover.h"
#include "ClassicButtonSystem.h"
#include "ClassicSlide.h"
#include "Components/Scrollbox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "ClassicFunctionLibrary.h"
#include "ClassicGameinstance.h"
#include "ClassicSaveGame.h"
#include "Arrow.h"
#include "ClassicButton.h"
#include "ClassicButtonsIcons.h"
#include "ClassicInfoInterface.h"
#include "ClassicSystemListInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Misc/Paths.h"
#include "ClassicMediaPlayer.h"
#include "ClassicLibretroTV.h"
#include "ClassicConfigurations.h"
#include "ToolTip.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "EngineUtils.h"
#include "MessageBalloon.h"
#include "LoopScrollBox.h"
#include "Frame.h"
#include "TextBoxScroll.h"
#include "TextImageBlock.h"
#include "Misc/OutputDeviceNull.h"


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
	ClassicGameInstance = Cast<UClassicGameInstance>(GetGameInstance());

	SetRenderOpacityList();
	LoadConfiguration();
	GameSettingsInit();

	Super::NativeConstruct();
}

void UMainInterface::NativeOnInitialized()
{
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
	Super::NativeOnInitialized();
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

void UMainInterface::RestartWidget()
{
	MessageCenter->SetVisibility(ESlateVisibility::Hidden);
	MessageCenter->SetText(FText::FromString(""));
	Clear();
	LoadConfigSystems();
}

void UMainInterface::SetCenterText(const FText Message)
{
	MessageCenter->SetVisibility(ESlateVisibility::Visible);
	MessageCenter->SetText(Message);
	OnSetCenterText(Message);
}

void UMainInterface::LoadConfiguration()
{
	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), ConfigurationData);
		UGameplayStatics::SetEnableWorldRendering(this, ConfigurationData.rendering);
		ConfigurationData.pathmedia = (ConfigurationData.pathmedia != TEXT("")) ? ConfigurationData.pathmedia : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");
		WBPClassicConfigurationsInterface->SlideVolume->SetSlideValue(FMath::Clamp(ConfigurationData.volume, 0, 100));
		LoadConfigSystems();
	}
	else
	{
		FFormatNamedArguments Args;
		Args.Add("GameRoot", FText::FromString(GameRoot));
		SetCenterText(FText::Format(LOCTEXT("LogNotFound", "{GameRoot} Not Found"), Args));
		UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
	}
}

void UMainInterface::LoadConfigSystems()
{

	const TArray<FConfigSystem> Systems = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave;
	if (Systems.Num() > 0)
	{
		AddSystems();

		for (int32 i = 0; i < Systems.Num(); i++)
		{
			if (Systems[i].SystemName == ConfigurationData.defaultstartsystem)
			{
				CountSystem = i;
				UE_LOG(LogTemp, Warning, TEXT("%s ConfigurationData"), *ConfigurationData.defaultstartsystem);
			}
		}
		CountLocationY = CountSystem;
		ClassicMediaPlayerReference->SetMusics(TEXT(""));
		LoadGamesList();
		CreateFolders();

		OnLoadConfigSystems(); //BlueprintImplementableEvent
	}
	else
	{
		SetCenterText(LOCTEXT("LogUpdateGameList", "Update game list, loading"));
		GetWorld()->GetTimerManager().SetTimer(DelayCreateGameListTimerHandle, this, &UMainInterface::CreateNewGameList, 0.5f, false, -1); //create new list game and save GameSystems internal
	}
}

void UMainInterface::AddSystems()
{
	TArray<FConfigSystem> Systems = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave;

	if (ButtonSystemClass != nullptr)
	{
		UClassicButtonSystem* ButtonSystem = nullptr;
		for (int32 i = 0; i < Systems.Num(); i++) {
			ButtonSystem = CreateWidget<UClassicButtonSystem>(GetOwningPlayer(), ButtonSystemClass);
			ButtonSystem->OnClickTrigger.AddDynamic(this, &UMainInterface::OnClickSystem);
			ButtonSystem->SetText(Systems[i].SystemLabel);
			ButtonSystem->SetCount(i);
			ButtonSystemReferences.Add(ButtonSystem);
			WBPSystemsList->ScrollBoxSystems->AddChild(ButtonSystem);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("buttonSystemClass Not Found"));
	}
}

void UMainInterface::LoadGamesList()
{
	GameSystems = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave;
	const TArray<FGameData> Data = UClassicFunctionLibrary::FilterFavoriteGameData(GameSystems[CountSystem].GameDatas, bFilterFavorites);
	if (Data.Num() > 0)
	{
		GameData = Data;
	}
	else
	{
		GameData = GameSystems[CountSystem].GameDatas;
		bFilterFavorites = false; //Instance GameData is not filtered by favorite so set bFilterFavorites to false
	}

	if (GameData.Num() > 0)
	{
		SetPaddingCovers();
		CreateCoversWidget(0, GameData.Num());

		ConfigurationData.defaultstartsystem = GameSystems[CountSystem].SystemName;
		FString XmlConfig = UClassicFunctionLibrary::CreateXMLConfigFile(ConfigurationData);
		XmlConfig = XmlConfig.Replace(TEXT("$(remove)"), TEXT(""), ESearchCase::IgnoreCase);
		const FString PathToSave = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config");
		const bool Saved = (UClassicFunctionLibrary::SaveStringToFile(PathToSave, TEXT("config.xml"), XmlConfig, true, false));

		UE_LOG(LogTemp, Warning, TEXT("%s"), (Saved) ? TEXT("Saved File") : TEXT("Not Saved File"));

		OnLoadGamesList(); //BlueprintImplementableEvent
	}
	else
	{
		if (!bFilterFavorites) //if No game list
		{
			FFormatNamedArguments Args;
			Args.Add("GameRoot", FText::FromString(GameSystems[CountSystem].RomPath));
			SetCenterText(FText::Format(LOCTEXT("LogGameListNotFound", "gamelist.xml not found in {GameRoot}"), Args));
		}
	}
}

void UMainInterface::ShowGames()
{
	PlayAnimationForward(LoadListGame);
	//WBPFrame->SetDefaultValues(1, DefaultFrameSpeed);
	ScrollListGame->ScrollWidgetIntoView(CoverReference[IndexCard], false, EDescendantScrollDestination::Center, 0);
	bInputEnable = true;
	PrepareThemes();
	OnShowGames(); //BlueprintImplementableEvent
}

void UMainInterface::PrepareThemes()
{
	OnPrepareThemes(); //BlueprintImplementableEvent
}

//timer DelayCreateGameListTimerHandle
void UMainInterface::CreateNewGameList()
{
	FString ConfigResult;
	FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\configsys.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{

		UClassicFunctionLibrary::SetConfigSystem(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("config.system")), GameSystems);
		GameSystems = UClassicFunctionLibrary::SortConfigSystem(GameSystems);
		for (int32 i = 0; i < GameSystems.Num(); i++)
		{
			GameRoot = GameSystems[i].RomPath + TEXT("\\gamelist.xml");
			if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
			{
				UClassicFunctionLibrary::SetGameData(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("gameList.game")), GameData);
				UClassicFunctionLibrary::SortGameDate(GameData);
				UClassicFunctionLibrary::FormatGameData(GameData, ConfigurationData, GameSystems[i]);
				GameSystems[i].GameDatas = GameData;
				GameData.Empty();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
			}
		}
		ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave = GameSystems;
		SaveGame();
		SetCenterText(LOCTEXT("LogSuccessfullyGameList", "Game list update successfully wait..."));
		GetWorld()->GetTimerManager().SetTimer(DelayReloadTimerHandle, this, &UMainInterface::RestartWidget, 3.0f, false, -1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
	}

	OnCreateNewGameList(); //BlueprintImplementableEvent
}

void UMainInterface::SaveGame() const
{
	if (UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Saved"));
	}
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
		}
		else
		{
			LoopScroll->BtnClick->SetKeyboardFocus();
		}
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

void UMainInterface::GameSettingsInit()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(60.0f);
	Settings->SetVSyncEnabled(true);
	Settings->SetResolutionScaleValueEx(25);
	Settings->SetViewDistanceQuality(0);
	Settings->SetAntiAliasingQuality(0);
	Settings->SetPostProcessingQuality(0);
	Settings->SetShadowQuality(0);
	Settings->SetGlobalIlluminationQuality(0);
	Settings->SetReflectionQuality(0);
	Settings->SetTextureQuality(0);
	Settings->SetVisualEffectQuality(0);
	Settings->SetFoliageQuality(0);
	Settings->SetShadingQuality(0);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}

void UMainInterface::GameSettingsRunning()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(1.0f);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}

void UMainInterface::GameSettingsRunningInternal()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(60.0f);
	Settings->SetResolutionScaleValueEx(100);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}

#pragma region CoverFunctions

void UMainInterface::SetPaddingCovers()
{
	if (GameData.Num() > 30) return;
	UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), CoverClass);
	UCanvasPanelSlot* ToolTipCover = Cast<UCanvasPanelSlot>(Cover->WBPFocusArrow->Slot);
	const FVector2D CoverSize = ToolTipCover->GetSize();
	const int32 PaddingSize = (30 - GameData.Num()) / 2;
	ToolTipCover->SetSize(FVector2D(CoverSize.X * PaddingSize, CoverSize.Y));
	Cover->SetVisibility(ESlateVisibility::Hidden);
	ScrollListGame->AddChild(Cover);
}

void UMainInterface::CreateCoversWidget(const int32 Min, const int32 Max)
{
	for (int32 i = Min; i < Max; i++)
	{
		if (!GameData.IsValidIndex(i))
		{
			break;
		}
		AddCoverWidget();
	}
}

void UMainInterface::AddCoverWidget()
{
	UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), CoverClass);
	ScrollListGame->AddChild(Cover);
	CoverReference.Add(Cover);
}

#pragma endregion CoverFunctions


/////////////////////////////////////////////////
///navigation area

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
	PressedDelayNavigation(TimerDelayNavigation);
	ENavigationButton = Navigate;
	if (ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::RIGHT)
	{
		if (PositionY == EPositionY::TOP)
		{
			SetNavigationFocusTop();
		}
		else if (PositionY == EPositionY::CENTER)
		{
			SetDirection(ENavigationButton, SpeedScroll);
		}
	}
	else if (ENavigationButton == EButtonsGame::LB || ENavigationButton == EButtonsGame::RB)
	{
		if (PositionY == EPositionY::CENTER)
		{
			SetDirection(ENavigationButton, SpeedScroll);
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
	PressedDelayNavigation(0.18f);
	ENavigationButton = Navigate;
	if (ENavigationButton == EButtonsGame::UP)
	{
		CountLocationY = FMath::Clamp(CountLocationY - 1, 0, ButtonSystemReferences.Num() - 1);
	}
	else if (ENavigationButton == EButtonsGame::DOWN)
	{
		CountLocationY = FMath::Clamp(CountLocationY + 1, 0, ButtonSystemReferences.Num() - 1);
	}

	if (ButtonSystemReferences.IsValidIndex(CountLocationY))
	{
		ButtonSystemReferences[CountLocationY]->Click->SetKeyboardFocus();
	}
}

void UMainInterface::NavigationInfo(EButtonsGame Navigate)
{
	PressedDelayNavigation(0.18f);
	ENavigationButton = Navigate;
	const float CurrentOffSet = WBPInfo->CurrentOffSet;
	if (ENavigationButton == EButtonsGame::UP)
	{
		WBPInfo->Scrolled(CurrentOffSet - 200);
	}
	else if (ENavigationButton == EButtonsGame::DOWN)
	{
		WBPInfo->Scrolled(CurrentOffSet + 200);
	}
}

void UMainInterface::NavigationConfiguration(EButtonsGame Navigate)
{
	WBPClassicConfigurationsInterface->SetIndexFocus(Navigate);
}

void UMainInterface::SetTitle(int32 Index)
{
	if (!GameData.IsValidIndex(IndexCard)) return;

	IndexCard = Index;
	const FString Title = GameData[IndexCard].nameFormated;
	TextTitleGame->SetText(FText::FromString(Title));
	WBPTextBoxScroll->SetText(GameData[IndexCard].descFormated);
	SetButtonsIconInterfaces(PositionY);

	if (CoverReference.IsValidIndex(IndexCard))
	{
		ScrollListGame->ScrollWidgetIntoView(CoverReference[IndexCard], false, EDescendantScrollDestination::Center, 0);
		CoverReference[IndexCard]->FocusCover(true);
	}
	if (CoverReference.IsValidIndex(IndexCard + 1))
	{
		CoverReference[IndexCard + 1]->FocusCover(false);
	}
	if (CoverReference.IsValidIndex(IndexCard - 1))
	{
		CoverReference[IndexCard - 1]->FocusCover(false);
	}
}

void UMainInterface::SetDirection(EButtonsGame Navigate, float Speed)
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
			UE_LOG(LogTemp, Warning, TEXT("Open frame bottom"));
			GetWorld()->GetTimerManager().SetTimer(StartVideoTimerHandle, this, &UMainInterface::StartVideo, 5.0f, false, -1);
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
		GameSettingsRunningInternal();
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
	GameSettingsRunning();
}

void UMainInterface::OnClickSystem(int32 Value)
{
	if (bInputEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("The OnClickSystem parameter value is: %d"), Value);
		if(CountSystem == Value)
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

	if (UClassicFunctionLibrary::FindGameData(ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas, GameData[IndexCard], Find))
	{
		// Find = return inline found index
		ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas[Find].playcount++;
		ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas[Find].lastplayed = UClassicFunctionLibrary::FormatDateToXml();

		GameData[IndexCard].playcount++;
		GameData[IndexCard].lastplayed = UClassicFunctionLibrary::FormatDateToXml();

		FString Path = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].RomPath;
		SaveGameListXML(Path, ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas);
		SaveGameList();
	}
}

void UMainInterface::SetFavoriteToSave()
{
	if (bDelayFavoriteClick && ENavigationButton == EButtonsGame::Y && !bScroll)
	{
		int32 Find;

		if (UClassicFunctionLibrary::FindGameData(ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas, GameData[IndexCard], Find))
		{
			// Find = return inline found index
			const bool ToggleFavorite = !ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas[Find].favorite;
			ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas[Find].favorite = ToggleFavorite;

			GameData[IndexCard].favorite = ToggleFavorite;

			UCard* Left;
			UCard* Center;
			UCard* Right;
			LoopScroll->GetCardReferences(IndexCard, Left, Center, Right);

			Left->SetFavorite(ToggleFavorite, true);
			Center->SetFavorite(ToggleFavorite, true);
			Right->SetFavorite(ToggleFavorite, true);

			FString Path = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].RomPath;
			SaveGameListXML(Path, ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas);
			SaveGameList();

			SetButtonsIconInterfaces(EPositionY::CENTER);
			ShowMessage((ToggleFavorite) ? LOCTEXT("MessageAddFavorite", "Add game to favorite") : LOCTEXT("RemoveFavorite", "Remove game to favorite"), 3.5f);

			FOutputDeviceNull OutputDeviceNull;
			this->CallFunctionByNameWithArguments(TEXT("ResetCache"), OutputDeviceNull, nullptr, true);
		}
	}
}

bool UMainInterface::SaveGameListXML(FString& GameListPath, TArray<FGameData>& NewGames)
{
	if (FPaths::FileExists(GameListPath + TEXT("\\gamelist.xml")))
	{
		const int32 ImageX = GameSystems[CountSystem].ImageX;
		const int32 ImageY = GameSystems[CountSystem].ImageY;
		const FString NewXMLFile = UClassicFunctionLibrary::CreateXMLGameFile(NewGames, FVector2D(ImageX, ImageY));
		return UClassicFunctionLibrary::SaveStringToFile(GameListPath, TEXT("gamelist.xml"), NewXMLFile, true, false);
	}

	return false;
}

bool UMainInterface::SaveGameList()
{
	return UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0);
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
		GameSettingsInit();
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
	ScrollListGame->SetRenderOpacity(0.f);
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

	ScrollListGame->ClearChildren();
	ScrollListGame->ScrollToStart();

	Focus = EFocus::MAIN;
	CoverReference.Empty();
	GameData.Empty();
	NewGameData.Empty();
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
	bFilterFavorites = false;
	bDelayFavoriteClick = false;
	bDelayQuit = false;
	bIsRunning = false;
	TimerDelayNavigation = 0.18f;
	CountSystem = 0;
	CountLocationY = 0;
	DescriptionScrollScale = 0.f;
	SpeedScroll = DefaultSpeedScroll;

	CoverReference.Empty();
	GameData.Empty();
	NewGameData.Empty();
}

//bind buttons
void UMainInterface::OnFocusSelectSystem()
{
	FocusButtonsTop(1, WBPToolTipSystem, WBPFrame->MoveLeftRightTop1, nullptr, EFocusTop::SYSTEM);
}

void UMainInterface::OnFocusConfigurations()
{
	FocusButtonsTop(2, WBPToolTipConfiguration, WBPFrame->MoveLeftRightTop2, WBPFrame->MoveLeftRightTop1, EFocusTop::CONFIG);
}

void UMainInterface::OnFocusFavorites()
{
	FocusButtonsTop(3, WBPToolTipFavorites, WBPFrame->MoveLeftRightTop3, WBPFrame->MoveLeftRightTop2, EFocusTop::FAVORITE);
}

void UMainInterface::OnFocusInfo()
{
	FocusButtonsTop(4, WBPToolTipInfo, nullptr, WBPFrame->MoveLeftRightTop3, EFocusTop::INFO);
	WBPInfo->SetGameInfo(GameData[IndexCard]);
}

void UMainInterface::FocusButtonsTop(const int32 PositionTopX, UToolTip* ToolTip, UWidgetAnimation* Left, UWidgetAnimation* Right, const EFocusTop FocusTop)
{
	LoopScroll->PositionTopX = PositionTopX;
	SetZOrderToolTips(ToolTip);
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

void UMainInterface::SetZOrderToolTips(const UToolTip* ToolTip)
{
	UCanvasPanelSlot* SlotToolTipSystem = Cast<UCanvasPanelSlot>(WBPToolTipSystem->Slot);
	UCanvasPanelSlot* ToolTipConfiguration = Cast<UCanvasPanelSlot>(WBPToolTipConfiguration->Slot);
	UCanvasPanelSlot* ToolTipFavorites = Cast<UCanvasPanelSlot>(WBPToolTipFavorites->Slot);
	UCanvasPanelSlot* ToolTipInfo = Cast<UCanvasPanelSlot>(WBPToolTipInfo->Slot);
	SlotToolTipSystem->SetZOrder(50);
	ToolTipConfiguration->SetZOrder(50);
	ToolTipFavorites->SetZOrder(50);
	ToolTipInfo->SetZOrder(50);

	UCanvasPanelSlot* ToolTipSlot = Cast<UCanvasPanelSlot>(ToolTip->Slot);
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
	ButtonSystemReferences[CountLocationY]->Click->SetKeyboardFocus();
	GetWorld()->GetTimerManager().SetTimer(SetArrowsTimerHandle, this, &UMainInterface::SetArrows, 0.02f, false, -1);
}

void UMainInterface::SetArrows()
{
	WBPSystemsList->SetIconArrow();
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
	if (bFilterFavorites)
	{
		bFilterFavorites = false;
		NavigationGame(EButtonsGame::DOWN);
		PlayAnimationReverse(BarTop);
		BtnFavorites->SetFocusButton(false);
		ResetCards(false, false);
		ShowMessage(LOCTEXT("MessageAllGames", "Show all games"), 3.5f);
	}
	else
	{
		if (UClassicFunctionLibrary::FilterFavoriteGameData(GameData, true).Num() > 0)
		{
			bFilterFavorites = true;
			NavigationGame(EButtonsGame::DOWN);
			PlayAnimationReverse(BarTop);
			BtnFavorites->SetFocusButton(false);
			ResetCards(false, false);
			ShowMessage(LOCTEXT("MessageOnlyFavorites", "Show only favorites"), 3.5f);
		}
		else
		{
			ShowMessage(LOCTEXT("MessageNoFavorites", "No favorites to show"), 3.5f);
		}
	}
}

void UMainInterface::OnClickInfo()
{
	PlayAnimationForward(ShowInfo);
	Focus = EFocus::INFO;
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
			UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
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

void UMainInterface::CreateFolders()
{
	const FString PathMedia = (ConfigurationData.pathmedia != TEXT("")) ? ConfigurationData.pathmedia : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");

	UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia);

	for (FConfigSystem& ConfigElement : GameSystems)
	{
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName);
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName + TEXT("\\covers"));
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName + TEXT("\\screenshots"));
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName + TEXT("\\videos"));
	}
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
		PlayAnimationForward(ChangeVideoToImage);
	}

	if (!GameData.IsValidIndex(IndexCard)) return;

	const FString ImagePath = GameData[IndexCard].thumbnailFormated;

	if (FPaths::FileExists(ImagePath))
	{
		int32 Width = 640;
		int32 Height = 480;
		UTexture2D* ImageLoaded = UClassicFunctionLibrary::LoadTexture(ImagePath, Width, Height);

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
