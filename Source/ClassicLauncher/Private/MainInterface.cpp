// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "MainInterface.h"
#include "Card.h"
#include "Cover.h"
#include "ClassicButtonSystem.h"
#include "ClassicSlide.h"
#include "Components/HorizontalBox.h"
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
#include "Components/MultiLineEditableTextBox.h"
#include "Misc/Paths.h"
#include "ClassicMediaPlayer.h"
#include "ClassicLibretroTV.h"
#include "ClassicConfigurations.h"
#include "RuntimeImageLoader.h"
#include "ToolTip.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "EngineUtils.h"
#include "MessageBalloon.h"
#include "Internationalization/StringTableRegistry.h"
#include "LoopScrollBox.h"
#include "Frame.h"

#define FRAME_SPEED 1.4f
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
	LoadConfigurationNative();
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

	ClassicMediaPlayerReference->MainInterfaceReference = this;
	ClassicMediaPlayerReference->SetMusics(TEXT(""));


	Super::NativeOnInitialized();
}

void UMainInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bKeyPressed) {
		OnNativeNavigationGame(UClassicFunctionLibrary::GetInputButton(KeyEvent));
		if (PositionY == EPositionY::TOP)
		{
			WBPInfo->ScrollTopEnd(UClassicFunctionLibrary::GetInputButton(KeyEvent));
		}
	}

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
		if (!bHover)
		{
			UClassicFunctionLibrary::PauseProcess(1.5f);
			RunningGame(true);
		}
		bHover = true;
	}
	else
	{
		if (bHover)
		{
			RunningGame(false);
		}
		bHover = false;
	}

}

void UMainInterface::RestartWidget()
{
	TxtDebug->SetVisibility(ESlateVisibility::Hidden);
	TxtDebug->SetText(FText::FromString(""));
	Clear();
	LoadConfigSystemsNative();
}

void UMainInterface::SetTextErrorMessage(const FText Message)
{
	TxtDebug->SetVisibility(ESlateVisibility::Visible);
	TxtDebug->SetText(Message);
	OnError(Message);
}

void UMainInterface::LoadConfigurationNative()
{
	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (bool IsValidConfig = UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), ConfigurationData);
		UGameplayStatics::SetEnableWorldRendering(this, ConfigurationData.rendering);
		ConfigurationData.pathmedia = (ConfigurationData.pathmedia != TEXT("")) ? ConfigurationData.pathmedia : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");
		WBPClassicConfigurationsInterface->SlideVolume->SetSlideValue(FMath::Clamp(ConfigurationData.volume, 0, 100));
		LoadConfigSystemsNative();
	}
	else
	{
		FFormatNamedArguments Args;
		Args.Add("GameRoot", FText::FromString(GameRoot));
		SetTextErrorMessage(FText::Format(LOCTEXT("LogNotFound", "{GameRoot} Not Found"), Args));
		UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
	}
}

void UMainInterface::LoadConfigSystemsNative()
{

	const TArray<FConfigSystem> Systems = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave;
	if (Systems.Num() > 0)
	{
		//LoadGameSystems
		CreateGameSystems();

		for (int32 i = 0; i < Systems.Num(); i++)
		{
			if (Systems[i].SystemName == ConfigurationData.defaultstartsystem)
			{
				CountSystem = i;
				UE_LOG(LogTemp, Warning, TEXT("%s ConfigurationData"), *ConfigurationData.defaultstartsystem);
			}
		}

		LoadListNative();
		CreateFolders();

		//BlueprintImplementableEvent
		LoadConfigSystems();
	}
	else
	{
		SetTextErrorMessage(LOCTEXT("LogUpdateGameList", "Update game list, loading"));
		//create and save GameSystems
		GetWorld()->GetTimerManager().SetTimer(DelayCreateGameListTimerHandle, this, &UMainInterface::CreateGameListNative, 0.5f, false, -1);
	}
}

void UMainInterface::LoadListNative()
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
		if (GameData.Num() < 4)
		{
			MaxFrameMove = GameData.Num();
		}
		else
		{
			MaxFrameMove = 4; //to remove
		}
		SetPaddingCovers();
		CreateCardsCoversWidget(0, GameData.Num());
		LoadFirstImages();
		SetPaddingCovers();

		ImgFrame->SetBrushFromTexture(ImageFrameCenter);


		ConfigurationData.defaultstartsystem = GameSystems[CountSystem].SystemName;
		FString XmlConfig = UClassicFunctionLibrary::CreateXMLConfigFile(ConfigurationData);
		XmlConfig = XmlConfig.Replace(TEXT("$(remove)"), TEXT(""), ESearchCase::IgnoreCase);
		const FString PathToSave = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config");
		const bool Saved = (UClassicFunctionLibrary::SaveStringToFile(PathToSave, TEXT("config.xml"), XmlConfig, true, false));

		UE_LOG(LogTemp, Warning, TEXT("%s"), (Saved) ? TEXT("Saved File") : TEXT("Not Saved File"));



		//Timer
		GetWorld()->GetTimerManager().SetTimer(DelayLoadListTimerHandle, this, &UMainInterface::ViewList, 0.25f, false, -1);

		//BlueprintImplementableEvent
		LoadList();
	}
	else
	{
		if (bFilterFavorites)
		{
			//if no favorites
		}
		else
		{
			//if No gamelist
			FFormatNamedArguments Args;
			Args.Add("GameRoot", FText::FromString(GameSystems[CountSystem].RomPath));
			SetTextErrorMessage(FText::Format(LOCTEXT("LogGameListNotFound", "gamelist.xml not found in {GameRoot}"), Args));
		}
	}
}

void UMainInterface::ViewList()
{
	UUserWidget::PlayAnimationForward(LoadListGame);
	ImgFrame->SetRenderOpacity(1.0f);
	ScrollListGame->ScrollWidgetIntoView(CoverReference[IndexCard], false, EDescendantScrollDestination::Center, 0);
	CountLocationY = CountSystem;
	bInputEnable = true;
	PrepareThemes();
}

void UMainInterface::PrepareThemes()
{
	Themes();
}

void UMainInterface::SetPaddingCovers()
{
	for (int32 i = 0; i < 15; i++) {
		UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), CoverClass);
		Cover->SetVisibility(ESlateVisibility::Hidden);
		ScrollListGame->AddChild(Cover);
	}
}

//timer DelayCreateGameListTimerHandle
void UMainInterface::CreateGameListNative()
{
	FString ConfigResult;
	FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\configsys.xml");

	if (bool IsValidConfig = UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		UClassicFunctionLibrary::SetConfigSystem(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("config.system")), GameSystems);
		GameSystems = UClassicFunctionLibrary::SortConfigSystem(GameSystems);
		for (int32 i = 0; i < GameSystems.Num(); i++)
		{
			GameRoot = GameSystems[i].RomPath + TEXT("\\gamelist.xml");
			IsValidConfig = UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot);
			if (IsValidConfig)
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
		SetTextErrorMessage(LOCTEXT("LogSuccessfullyGameList", "Game list update successfully wait..."));
		GetWorld()->GetTimerManager().SetTimer(DelayReloadTimerHandle, this, &UMainInterface::RestartWidget, 3.0f, false, -1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
	}

	//BlueprintImplementableEvent
	CreateGameList();
}

void UMainInterface::SaveGame()
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

		if (ENavigationLastButton == EButtonsGame::NONE)
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
			if (Input == EButtonsGame::SCROLLDOWN || Input == EButtonsGame::SCROLLUP)
			{
				SetScrollDescription(Input);
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
	if (ScrollScale > 0)
	{
		SetScrollDescription(EButtonsGame::SCROLLUP);
	}
	else if (ScrollScale < 0)
	{
		SetScrollDescription(EButtonsGame::SCROLLDOWN);
	}

	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}


void UMainInterface::OnAnimationStartedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationStartedPlaying(Player);
	if (PositionY == EPositionY::CENTRAL)
	{
		WBPFrame->ChangeTexture(false);
	}
	const UWidgetAnimation* AnimationGet = Player.GetAnimation();
}

void UMainInterface::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	if (PositionY == EPositionY::TOP)
	{
		WBPFrame->ChangeTexture(true);
	}
	const UWidgetAnimation* AnimationGet = Player.GetAnimation();
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

void UMainInterface::CreateCardsCoversWidget(int32 Min, int32 Max)
{
	for (int32 i = Min; i < Max; i++)
	{
		if (!GameData.IsValidIndex(i))
		{
			break;
		}
		CreateCardCoverWidget(GameData[i]);

	}
}

void UMainInterface::CreateCardCoverWidget(FGameData Data)
{
	UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), CoverClass);
	ScrollListGame->AddChild(Cover);
	CoverReference.Add(Cover);
}

void UMainInterface::CreateGameSystems()
{
	TArray<FConfigSystem> Systems = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave;

	if (ButtonSystemClass != nullptr)
	{
		UClassicButtonSystem* ButtonSystem = nullptr;
		for (int32 i = 0; i < Systems.Num(); i++) {
			ButtonSystem = CreateWidget<UClassicButtonSystem>(GetOwningPlayer(), ButtonSystemClass);
			ButtonSystem->OnClickTrigger.AddDynamic(this, &UMainInterface::OnNativeClickSystem);
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

/////////////////////////////////////////////////
///navigation area

void UMainInterface::OnNativeNavigationGame(EButtonsGame Navigate)
{
	//UE_LOG(LogTemp, Warning, TEXT("broadcast navigate"));	

	if (Navigate == EButtonsGame::UP || Navigate == EButtonsGame::DOWN || Navigate == EButtonsGame::LEFT || Navigate == EButtonsGame::RIGHT || Navigate == EButtonsGame::LB || Navigate == EButtonsGame::RB)
	{
		if (!bScroll && bInputEnable && bDelayPressed && bUpDownPressed)
		{
			OnNavigationGame(Navigate); //Call Event Blueprint
			if (Focus == EFocus::MAIN) {
				OnNativeNavigationMain(Navigate);
			}
			else if (Focus == EFocus::SYSTEM) {
				OnNativeNavigationSystem(Navigate);
			}
			else if (Focus == EFocus::INFO) {
				OnNativeNavigationInfo(Navigate);
			}
			else if (Focus == EFocus::CONFIG) {
				OnNativeNavigationConfiguration(Navigate);
			}
		}
	}
}

void UMainInterface::OnNativeNavigationMain(EButtonsGame Navigate)
{
	PressedDelayNavigation(TimerDelayAnimation);
	ENavigationButton = Navigate;
	if (ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::RIGHT)
	{
		if (PositionY == EPositionY::TOP)
		{
			SetNavigationFocusTop();
		}
		else if (PositionY == EPositionY::CENTRAL)
		{
			SetDirection(ENavigationButton, 30.0f);
		}
	}
	else if (ENavigationButton == EButtonsGame::LB || ENavigationButton == EButtonsGame::RB)
	{
		if (PositionY == EPositionY::CENTRAL)
		{
			SetDirection(ENavigationButton, 80.0f);
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

void UMainInterface::OnNativeNavigationSystem(EButtonsGame Navigate)
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

void UMainInterface::OnNativeNavigationInfo(EButtonsGame Navigate)
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

void UMainInterface::OnNativeNavigationConfiguration(EButtonsGame Navigate)
{
	WBPClassicConfigurationsInterface->SetIndexFocus(Navigate);
}

void UMainInterface::SetTitle(int32 Index)
{
	IndexCard = Index;

	const FString Title = GameData[IndexCard].nameFormated;
	TxtTitleGame->SetJustification(ETextJustify::Left);
	TxtTitleGame->SetJustification((Title.Len() > 51) ? ETextJustify::Left : ETextJustify::Center);
	TxtTitleGame->SetText(FText::FromString(Title));
	TxtDescription->SetText(FText::FromString(GameData[IndexCard].descFormated));
	ScrollListGame->ScrollWidgetIntoView(CoverReference[IndexCard], true, EDescendantScrollDestination::Center, 0);
	SetButtonsIconInterfaces(PositionY);
}

void UMainInterface::SetDirection(EButtonsGame Navigate, float Speed)
{
	LoopScroll->Speed = Speed;
	if (Navigate == EButtonsGame::RIGHT || Navigate == EButtonsGame::RB)
	{
		LoopScroll->StartScrollTo(EButtonsGame::RIGHT);
	}
	else if (Navigate == EButtonsGame::LEFT  || Navigate == EButtonsGame::LB)
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
		if (ImgVideo->RenderTransform.Translation.X == 0)
		{
			UUserWidget::PlayAnimationReverse(ShowDescBottomInfo);
			PositionY = EPositionY::CENTRAL;
			ClassicMediaPlayerReference->PauseVideo();
			ClassicMediaPlayerReference->ResumeMusic();
			UE_LOG(LogTemp, Warning, TEXT("Close frame bottom"));
		}
		else
		{
			UUserWidget::PlayAnimationReverse(VideoAnimation);
		}
	}
	else if (PositionY == EPositionY::CENTRAL)
	{
		PositionY = EPositionY::TOP;
		UUserWidget::PlayAnimationForward(BarTop);
		SetButtonsIconInterfaces(PositionY);

		SetTopButtonFocus();
	}
}

void UMainInterface::SetTopButtonFocus()
{
	switch (PositionTopX)
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

void UMainInterface::SetNavigationFocusDownBottom()
{
	if (PositionY == EPositionY::CENTRAL)
	{
		if (PositionY != EPositionY::BOTTOM)
		{
			PositionY = EPositionY::BOTTOM;
			UE_LOG(LogTemp, Warning, TEXT("Open frame bottom"));
			UUserWidget::PlayAnimationForward(ShowDescBottomInfo);

			if (GameData.IsValidIndex(IndexCard))
			{
				const FString PathVideo = GameData[IndexCard].videoFormated;
				if (FPaths::FileExists(PathVideo))
				{
					ImgVideo->SetVisibility(ESlateVisibility::Visible);
					ImgImageBottom->SetVisibility(ESlateVisibility::Hidden);
					ClassicMediaPlayerReference->PlayVideo(PathVideo);
				}
				else
				{
					SetImageBottom();
				}
			}

		}
	}
	else if (PositionY == EPositionY::BOTTOM)
	{
		if (ImgVideo->RenderTransform.Translation.X == 0)
		{
			UUserWidget::PlayAnimationForward(VideoAnimation);
		}
	}
	else
	{
		PositionY = EPositionY::CENTRAL;
		LoopScroll->BtnClick->SetKeyboardFocus();
		UUserWidget::PlayAnimationReverse(BarTop);
		SetButtonsIconInterfaces(PositionY);
	}
}


//end navigate area
///////////////////////////////////////////////////

void UMainInterface::OnNativeClick()
{
	if (PositionY == EPositionY::CENTRAL && bInputEnable)
	{
		UUserWidget::PlayAnimationForward(FadeStartSystem);
		SetCountPlayerToSave();
		LoopScroll->OpenCard();
		GetWorld()->GetTimerManager().SetTimer(LauncherTimerHandle, this, &UMainInterface::ClassicLaunch, 1.0f, false, -1);
	}
	//this function is BlueprintImplementableEvent
	OnClickPath();
}

void UMainInterface::ClassicLaunch()
{
	FString PathRomFormated = UClassicFunctionLibrary::HomeDirectoryReplace(GameData[IndexCard].PathFormated);
	FString ExecutablePath = (GameData[IndexCard].Executable == TEXT("")) ? GameSystems[CountSystem].Executable : GameData[IndexCard].Executable;
	FString Arguments = (GameData[IndexCard].Arguments == TEXT("")) ? GameSystems[CountSystem].Arguments : GameData[IndexCard].Arguments;
	bool CanUnzip = false;
	FString CoreFormated;

	if (UClassicFunctionLibrary::SwitchOnDefaultLibreto(ExecutablePath, CoreFormated, CanUnzip))
	{
		OpenLibretro(CoreFormated, PathRomFormated, CanUnzip);
		UE_LOG(LogTemp, Warning, TEXT("RomPath %s , CorePath %s , CanUnzip %s"), *PathRomFormated, *CoreFormated, (CanUnzip ? TEXT("true") : TEXT("false")));
	}
	else
	{
		TArray<FString> Commands;
		if (!Arguments.IsEmpty()) {
			Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(Arguments));
		}
		if (!Arguments.IsEmpty()) {
			Commands.Add(UClassicFunctionLibrary::HomeDirectoryReplace(PathRomFormated));
		}
		OpenExternalProcess(ExecutablePath, Commands);
		UE_LOG(LogTemp, Warning, TEXT("OpenExternalProcess: %s %s %s"), *ExecutablePath, *Arguments, *PathRomFormated);
	}
}

void UMainInterface::OpenLibretro(FString CorePath, FString RomPath, bool CanUnzip)
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

void UMainInterface::OnNativeClickSystem(int32 Value)
{
	if (bInputEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("The OnNativeClickSystem parameter value is: %d"), Value);
		CountSystem = Value;   //CountSystem = CountLocationY;
		ResetCards(true, true);

		//this function is BlueprintImplementableEvent
		OnClickSystem(Value);
	}
}


void UMainInterface::SetButtonsIconInterfaces(EPositionY GetPosition)
{
	if (GetPosition == EPositionY::TOP)
	{
		WBPButtonsIconsInterfaces->SetButtonsText(TextTop);
		WBPButtonsIconsInterfaces->SetButtonsVisibility(IconTop);
	}
	else if (GetPosition == EPositionY::CENTRAL)
	{
		if (GameData.IsValidIndex(IndexCard) && TextCenter.IsValidIndex(5))
		{
			TextCenter[5] = (GameData[IndexCard].favorite) ? LOCTEXT("buttonRemoveFavorite", "Remove Favorite") : LOCTEXT("buttonRemoveFavoriteAddFavorite", "Add Favorite");
		}

		WBPButtonsIconsInterfaces->SetButtonsText(TextCenter);
		WBPButtonsIconsInterfaces->SetButtonsVisibility(IconCenter);
	}
}

void UMainInterface::LoadFirstImages()
{
	const int32 Length = FMath::Clamp(GameData.Num(), 0, 15);
	for (int32 i = 0; i < Length; i++)
	{
		//ImageCard = UClassicFunctionLibrary::LoadTexture(GameData[i].imageFormated);
		//AddImagesCardCover(ImageCard, i);
		CoverReference[i]->SetVisibility(ESlateVisibility::Visible);
		LastIndex = i;
	}

	OnLoopStartAsyncImage(LastIndex);
}

void UMainInterface::LoadImages()
{

	if (ENavigationButton == EButtonsGame::RIGHT || ENavigationButton == EButtonsGame::RB || ENavigationButton == EButtonsGame::LEFT || ENavigationButton == EButtonsGame::LB)
	{
		if (ENavigationButton == EButtonsGame::RIGHT || ENavigationButton == EButtonsGame::RB)
		{
			LastIndex = FMath::Clamp(IndexCard + 14, 0, GameData.Num());//14
			FirstIndex = FMath::Clamp(IndexCard - 15, -1, GameData.Num());//15
		}
		else
		{
			LastIndex = FMath::Clamp(IndexCard - 14, -1, GameData.Num());//14
			FirstIndex = FMath::Clamp(IndexCard + 15, 0, GameData.Num());//15
		}

		if (GameData.IsValidIndex(LastIndex))
		{
			//ASyncLoadCard(GameData[IndexCard].imageFormated, LastIndex);
			LoadImageSync(LastIndex);

			CoverReference[LastIndex]->SetVisibility(ESlateVisibility::Visible);
		}

		if (CoverReference.IsValidIndex(FirstIndex)) {
			CoverReference[FirstIndex]->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}

void UMainInterface::ClearAllVisibilityCovers()
{
	for (int32 i = 0; i < GameData.Num(); i++)
	{
		CoverReference[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UMainInterface::SetImagesCover(UTexture2D* Texture, UCover* Cover, int32 Index)
{
	if (CoverReference.IsValidIndex(Index))
	{
		Cover->LoadCoverImage(Texture, GameData[Index].ImageX, GameData[Index].ImageY);
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

			LoopScroll->CardReferenceLeft[IndexCard]->SetFavorite(ToggleFavorite, true);
			LoopScroll->CardReferenceCenter[IndexCard]->SetFavorite(ToggleFavorite, true);
			LoopScroll->CardReferenceRight[IndexCard]->SetFavorite(ToggleFavorite, true);

			FString Path = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].RomPath;
			SaveGameListXML(Path, ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas);
			SaveGameList();

			SetButtonsIconInterfaces(EPositionY::CENTRAL);
			ShowMessage((ToggleFavorite) ? LOCTEXT("MessageAddFavorite", "Add game to favorite") : LOCTEXT("RemoveFavorite", "Remove game to favorite"), 3.5f);
		}
	}
}

bool UMainInterface::SaveGameListXML(FString& GameListPath, TArray<FGameData>& NewGameDatas)
{
	if (FPaths::FileExists(GameListPath + TEXT("\\gamelist.xml")))
	{
		const int32 ImageX = GameSystems[CountSystem].ImageX;
		const int32 ImageY = GameSystems[CountSystem].ImageY;
		const FString NewXMLFile = UClassicFunctionLibrary::CreateXMLGameFile(NewGameDatas, FVector2D(ImageX, ImageY));
		return UClassicFunctionLibrary::SaveStringToFile(GameListPath, TEXT("gamelist.xml"), NewXMLFile, true, false);
	}
	else
	{
		return false;
	}
}

bool UMainInterface::SaveGameList()
{
	return UGameplayStatics::SaveGameToSlot(ClassicGameInstance->ClassicSaveGameInstance, ClassicGameInstance->SlotGame, 0);
}

void UMainInterface::RunningGame(bool IsRun)
{
	if (IsRun)
	{
		ClassicMediaPlayerReference->PauseMusic();
		HBListGame->SetVisibility(ESlateVisibility::Hidden);
		BtnSelectSystem->SetVisibility(ESlateVisibility::Hidden);
		BtnConfigurations->SetVisibility(ESlateVisibility::Hidden);
		BtnFavorites->SetVisibility(ESlateVisibility::Hidden);
		BtnInfo->SetVisibility(ESlateVisibility::Hidden);
		bInputEnable = false;
	}
	else
	{
		ClassicMediaPlayerReference->PlayMusic();
		HBListGame->SetVisibility(ESlateVisibility::Visible);
		BtnSelectSystem->SetVisibility(ESlateVisibility::Visible);
		BtnConfigurations->SetVisibility(ESlateVisibility::Visible);
		BtnFavorites->SetVisibility(ESlateVisibility::Visible);
		BtnInfo->SetVisibility(ESlateVisibility::Visible);
		GameSettingsInit();
		bInputEnable = true;
		ProcessID = 0;
		UUserWidget::PlayAnimationReverse(FadeStartSystem);
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

	TxtTitleGame->SetRenderOpacity(0.f);
	BgTitle->SetRenderOpacity(0.f);
	ScrollListGame->SetRenderOpacity(0.f);
	ImgFrame->SetRenderOpacity(0.f);
	HBListGame->SetRenderOpacity(0.f);
	WBPArrow->SetRenderOpacity(0.f);
	TxtDebug->SetVisibility(ESlateVisibility::Hidden);
	TxtDebug->SetText(FText::FromString(""));
}

void UMainInterface::ResetCards(bool bAnimationBarTop, bool bAnimationShowSystem)
{
	bInputEnable = false;

	TxtDebug->SetVisibility(ESlateVisibility::Hidden);
	TxtDebug->SetText(FText::FromString(""));

	ImgFrame->SetRenderOpacity(0.f);
	UUserWidget::PlayAnimationReverse(LoadListGame);

	ScrollListGame->ClearChildren();
	ScrollListGame->ScrollToStart();

	Focus = EFocus::MAIN;
	CoverReference.Empty();
	GameData.Empty();
	NewGameData.Empty();
	IndexCard = 0;
	PositionCenterX = 1;
	PositionY = EPositionY::CENTRAL;
	DescriptionScrollScale = 0.f;

	bUpDownPressed = true;
	bDelayPressed = true;

	//ImgFrame->SetBrushFromTexture(ImageFrameCenter);
	WBPFrame->SetDefaultValues(1, FRAME_SPEED);
	AnimationFrameMoveLeft();

	if (bAnimationBarTop)
	{
		UUserWidget::PlayAnimationReverse(BarTop);
	}
	if (bAnimationShowSystem)
	{
		UUserWidget::PlayAnimationReverse(ShowSystem);
	}

	SetButtonsIconInterfaces(EPositionY::CENTRAL);
	GetWorld()->GetTimerManager().SetTimer(DelayPressedTimerHandle, this, &UMainInterface::LoadListNative, 0.3f, false, -1);

}

void UMainInterface::Clear()
{
	IndexCard = 0;
	FirstIndex = 0;
	LastIndex = 1;
	IndexAsyncImage = 0;
	ProcessID = 0;
	PositionCenterX = 1;
	PositionTopX = 1;
	ENavigationLastButton = EButtonsGame::NONE;
	ENavigationButton = EButtonsGame::NONE;
	ENavigationScroll = EButtonsGame::NONE;
	ENavigationBack = EButtonsGame::NONE;
	ENavigationA = EButtonsGame::NONE;
	PositionY = EPositionY::CENTRAL;
	Focus = EFocus::MAIN;
	bDelayPressed = true;
	bKeyPressed = false;
	bUpDownPressed = true;
	bInputEnable = false;
	bScroll = false;
	bFilterFavorites = false;
	bDelayFavoriteClick = false;
	bDelayQuit = false;
	bHover = false;
	TimerDelayAnimation = 0.18f;
	TriggerDelayPressed = 0.15f;
	CountSystem = 0;
	CountLocationY = 0;
	MaxFrameMove = 4;
	DescriptionScrollScale = 0.f;

	CoverReference.Empty();
	GameData.Empty();
	NewGameData.Empty();
}

//Animations
void UMainInterface::AnimationFrameMoveRight()
{
	FrameMoveRight();

	if (GameData.Num() < 5) return;

	const float TranslationFrame = ImgFrame->RenderTransform.Translation.X;

	switch (PositionCenterX)
	{
	case 2:
		if (TranslationFrame != 385)
		{
			UUserWidget::PlayAnimationForward(FrameAnimationX1);
		}
		break;
	case 3:
		if (TranslationFrame != 770)
		{
			UUserWidget::PlayAnimationForward(FrameAnimationX2);
		}
		break;
	case 4:
		if (TranslationFrame != 1155)
		{
			UUserWidget::PlayAnimationForward(FrameAnimationX3);
		}
		break;
	}

}

void UMainInterface::AnimationFrameMoveLeft()
{
	FrameMoveLeft();

	if (GameData.Num() < 5) return;
	
	const float TranslationFrame = ImgFrame->RenderTransform.Translation.X;
	switch (PositionCenterX)
	{
	case 1:
		if (TranslationFrame != 0)
		{
			UUserWidget::PlayAnimationReverse(FrameAnimationX1);
		}
		break;
	case 2:
		if (TranslationFrame != 385)
		{
			UUserWidget::PlayAnimationReverse(FrameAnimationX2);
		}
		break;
	case 3:
		if (TranslationFrame != 770)
		{
			UUserWidget::PlayAnimationReverse(FrameAnimationX3);
		}
		break;
	}
}

void UMainInterface::AnimationFrameToTop(UWidgetAnimation* Animation1, UWidgetAnimation* Animation2,
	UWidgetAnimation* Animation3, UWidgetAnimation* Animation4, bool Reverse)
{
	if (Reverse)
	{
		switch (PositionCenterX)
		{
		case 1: UUserWidget::PlayAnimationReverse(Animation1); break;
		case 2: UUserWidget::PlayAnimationReverse(Animation2); break;
		case 3: UUserWidget::PlayAnimationReverse(Animation3); break;
		case 4: UUserWidget::PlayAnimationReverse(Animation4); break;
		}
	}
	else
	{
		switch (PositionCenterX)
		{
		case 1: UUserWidget::PlayAnimationForward(Animation1); break;
		case 2:	UUserWidget::PlayAnimationForward(Animation2); break;
		case 3: UUserWidget::PlayAnimationForward(Animation3); break;
		case 4: UUserWidget::PlayAnimationForward(Animation4); break;
		}
	}

}

//bind buttons
void UMainInterface::OnFocusSelectSystem()
{
	PositionTopX = 1;
	SetToolTip(WBPToolTipSystem);
	WBPToolTipSystem->SetToolTipVisibility(ESlateVisibility::Visible);
	const int32 FramePosition = WBPFrame->ImageFrame->RenderTransform.Translation.Y;
	UE_LOG(LogTemp, Warning, TEXT("Position Frame Y %d"), FramePosition);
	if (ENavigationButton == EButtonsGame::LEFT)
	{
		WBPFrame->UUserWidget::PlayAnimationReverse(WBPFrame->MoveLeftRightTop1);
	}
	else if (ENavigationButton == EButtonsGame::UP && FramePosition == 0)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::SYSTEM, false);
	}
}

void UMainInterface::OnFocusConfigurations()
{
	PositionTopX = 2;
	SetToolTip(WBPToolTipConfiguration);
	WBPToolTipConfiguration->SetToolTipVisibility(ESlateVisibility::Visible);
	const int32 FramePosition = WBPFrame->ImageFrame->RenderTransform.Translation.Y;
	if (ENavigationButton == EButtonsGame::LEFT)
	{
		WBPFrame->UUserWidget::PlayAnimationReverse(WBPFrame->MoveLeftRightTop2);
	}
	else if (ENavigationButton == EButtonsGame::RIGHT)
	{
		WBPFrame->UUserWidget::PlayAnimationForward(WBPFrame->MoveLeftRightTop1);
	}
	else if (ENavigationButton == EButtonsGame::UP && FramePosition == 0)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::CONFIG, false);
	}
}

void UMainInterface::OnFocusFavorites()
{
	PositionTopX = 3;
	SetToolTip(WBPToolTipFavorites);
	WBPToolTipFavorites->SetToolTipVisibility(ESlateVisibility::Visible);
	const int32 FramePosition = WBPFrame->ImageFrame->RenderTransform.Translation.Y;
	if (ENavigationButton == EButtonsGame::LEFT)
	{
		WBPFrame->UUserWidget::PlayAnimationReverse(WBPFrame->MoveLeftRightTop3);
	}
	else if (ENavigationButton == EButtonsGame::RIGHT)
	{
		WBPFrame->UUserWidget::PlayAnimationForward(WBPFrame->MoveLeftRightTop2);
	}
	else if (ENavigationButton == EButtonsGame::UP && FramePosition == 0)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::FAVORITE, false);
	}
}

void UMainInterface::OnFocusInfo()
{
	WBPInfo->SetGameInfo(GameData[IndexCard]);
	PositionTopX = 4;
	SetToolTip(WBPToolTipInfo);
	WBPToolTipInfo->SetToolTipVisibility(ESlateVisibility::Visible);
	const int32 FramePosition = WBPFrame->ImageFrame->RenderTransform.Translation.Y;
	if (ENavigationButton == EButtonsGame::RIGHT)
	{
		WBPFrame->UUserWidget::PlayAnimationForward(WBPFrame->MoveLeftRightTop3);
	}
	else if (ENavigationButton == EButtonsGame::UP && FramePosition == 0)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::INFO, false);
	}
}

void UMainInterface::SetToolTip(UToolTip* ToolTip)
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
	WBPToolTipSystem->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B && Focus == EFocus::MAIN)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::SYSTEM, true);
		UE_LOG(LogTemp, Warning, TEXT("(MainInterface)(OnLostFocusSelectSystem)"));
	}
}

void UMainInterface::OnLostFocusConfigurations()
{
	WBPToolTipConfiguration->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::CONFIG, true);
	}
}

void UMainInterface::OnLostFocusFavorites()
{
	WBPToolTipFavorites->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::FAVORITE, true);
	}
}

void UMainInterface::OnLostFocusInfo()
{
	WBPToolTipInfo->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B)
	{
		WBPFrame->AnimationToTopDown(EFocusTop::INFO, true);
	}
}

void UMainInterface::OnClickSelectSystem()
{
	Focus = EFocus::SYSTEM;
	UUserWidget::PlayAnimationForward(ShowSystem);
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
	UUserWidget::PlayAnimationForward(AnimationShowConfiguration);
	WBPClassicConfigurationsInterface->SetFocusSelect();
}

void UMainInterface::OnClickFavorites()
{
	if (bFilterFavorites)
	{
		bFilterFavorites = false;
		OnNativeNavigationGame(EButtonsGame::DOWN);
		UUserWidget::PlayAnimationReverse(BarTop);
		BtnFavorites->SetFocusButton(false);
		ResetCards(false, false);
		ShowMessage(LOCTEXT("MessageAllGames", "Show all games"), 3.5f);
	}
	else
	{
		const int32 Length = UClassicFunctionLibrary::FilterFavoriteGameData(GameData, true).Num();
		if (Length > 0)
		{
			bFilterFavorites = true;
			OnNativeNavigationGame(EButtonsGame::DOWN);
			UUserWidget::PlayAnimationReverse(BarTop);
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
	UUserWidget::PlayAnimationForward(ShowInfo);
	Focus = EFocus::INFO;
}

void UMainInterface::OnClickBackAction()
{
	switch (PositionY)
	{
	case EPositionY::TOP:
		CloseMenus();
		break;
	case EPositionY::CENTRAL:
		if (bDelayQuit) {
			UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
		}
		GetWorld()->GetTimerManager().SetTimer(DelayQuitTimerHandle, this, &UMainInterface::QuitGame, 3.f, false, -1);
		ShowMessage(LOCTEXT("PressAgainToQuit", "Press Again To Quit"), 3);
		bDelayQuit = true;
		break;
	case EPositionY::BOTTOM:
		SetNavigationFocusUpBottom();
		break;
	default:
		break;
	}
}

void UMainInterface::QuitGame()
{
	bDelayQuit = false;
}

void UMainInterface::OnClickFavorite()
{
	if (bInputEnable && PositionY == EPositionY::CENTRAL && !bDelayFavoriteClick && !bScroll)
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
		UUserWidget::PlayAnimationReverse(ShowSystem);
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
		UUserWidget::PlayAnimationReverse(ShowInfo);
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
	UUserWidget::PlayAnimationReverse(AnimationShowConfiguration);
	BtnConfigurations->BtButton->SetKeyboardFocus();
}

void UMainInterface::ShowMessage(FText Message, float InRate)
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

void UMainInterface::SetScrollDescription(EButtonsGame Scroll)
{
	if (Scroll == EButtonsGame::SCROLLUP)
	{
		DescriptionScrollScale = FMath::Clamp(DescriptionScrollScale - 25.0f, 0, ScrollDescription->GetScrollOffsetOfEnd());
		ScrollDescription->SetScrollOffset(DescriptionScrollScale);
	}
	else if (Scroll == EButtonsGame::SCROLLDOWN)
	{
		DescriptionScrollScale = FMath::Clamp(DescriptionScrollScale + 25.0f, 0, ScrollDescription->GetScrollOffsetOfEnd());
		ScrollDescription->SetScrollOffset(DescriptionScrollScale);
	}
}

void UMainInterface::SetVisibiltyDebugButton(UButton* button)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	button->SetVisibility(ESlateVisibility::Visible);
#endif
}

void UMainInterface::SetImageBottom()
{

	if (ImgVideo == nullptr || ImgImageBottom == nullptr) return;

	ImgVideo->SetVisibility(ESlateVisibility::Hidden);
	ImgImageBottom->SetVisibility(ESlateVisibility::Visible);

	if (!GameData.IsValidIndex(IndexCard)) return;

	const FString ImagePath = GameData[IndexCard].thumbnailFormated;

	if (FPaths::FileExists(ImagePath))
	{
		int32 Size = 32;
		bool IsValidTexture = false;
		UTexture2D* ImageLoaded = UClassicFunctionLibrary::LoadTexture(ImagePath);

		if (ImageLoaded != nullptr)
		{
			ImgImageBottom->SetBrushFromTexture(ImageLoaded);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Image not Loaded"));
		}
	}
	else
	{
		ImgImageBottom->SetBrushFromTexture(ImageNull);
	}

}

#undef LOCTEXT_NAMESPACE
