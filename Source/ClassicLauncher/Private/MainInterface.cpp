// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "MainInterface.h"
#include "Card.h"
#include "Cover.h"
#include "ClassicButtonSystem.h"
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
#include "RuntimeImageLoader.h"
#include "ToolTip.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "EngineUtils.h"


UMainInterface::UMainInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CountGarbageCollection = 0;
	IndexCard = 0;
	FirstIndex = 0;
	LastIndex = 1;
	IndexAsyncImage = 0;
	ProcessID = 0;
	PositionCenterX = 1;
	PositionTopX = 1;
	ENavigationButton = EButtonsGame::NONE;
	ENavigationScroll = EButtonsGame::NONE;
	ENavigationBack = EButtonsGame::NONE;
	ENavigationA = EButtonsGame::NONE;
	PositionY = EPositionY::CENTRAL;
	Focus = EFocus::MAIN;
	bDelayPressed = true;
	bKeyPressed = false;
	bUpDownPressed = true;
	bKeyTriggerLeft = false;
	bKeyTriggerRight = false;
	bInputEnable = true;
	bScroll = false;
	bFilterFavorites = false;
	bDelayFavoriteClick = false;
	CorePath = TEXT("");
	TimerDelayAnimation = 0.15f;
	TriggerDelayPressed = 1.0f;
	SpeedScroll = 28.0f;
	CountSystem = 0;
	CountLocationY = 0;
	MaxFrameMove = 4;

	TextTop.Add(TEXT(""));
	TextTop.Add(TEXT("Game List"));
	TextTop.Add(TEXT("Select"));
	TextTop.Add(TEXT("Ok"));
	TextTop.Add(TEXT("Back"));
	TextTop.Add(TEXT(""));
	TextTop.Shrink();

	TextCenter.Add(TEXT("Menu"));
	TextCenter.Add(TEXT("Details"));
	TextCenter.Add(TEXT("Select"));
	TextCenter.Add(TEXT("Start Game"));
	TextCenter.Add(TEXT("Exit"));
	TextCenter.Add(TEXT("Favorite"));
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

	//UGameplayStatics::GetActorOfClass

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

	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UMainInterface::TimerTick, 0.016f, true, -1);

	Super::NativeOnInitialized();
}

void UMainInterface::TimerTick()
{
	ScrollCards();

	if (bKeyPressed) {
		OnNativeNavigationGame(UClassicFunctionLibrary::GetInputButton(KeyEvent));
		if (PositionY == EPositionY::TOP)
		{
			WBPInfo->ScrollTopEnd(UClassicFunctionLibrary::GetInputButton(KeyEvent));
		}
	}

	if (ENavigationBack == EButtonsGame::SELECT && ENavigationA == EButtonsGame::A && ProcessID != 0)
	{
		TArray<FString> TextArguments;
		TextArguments.Add(TEXT("  /PID   "));
		TextArguments.Add(FString::FromInt(ProcessID));
		int32 Proc;
		UClassicFunctionLibrary::CreateProcess(Proc, TEXT("taskkill  "), TextArguments, false, true, 0, TEXT(""));
	}

}

void UMainInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainInterface::RestartWidget()
{
	UGameplayStatics::OpenLevel(this, FName("map"), true);
}

void UMainInterface::OnErrorMessage(const FString& ErrorMessage)
{
	SetDebugMessage(ErrorMessage);
	OnError(ErrorMessage);
}

void UMainInterface::SetDebugMessage(const FString Message)
{
	TxtDebug->SetVisibility(ESlateVisibility::Visible);
	TxtDebug->SetText(FText::FromString(Message));
}

void UMainInterface::LoadConfigurationNative()
{
	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (bool IsValidConfig = UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		ConfigurationData = UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")));
		UGameplayStatics::SetEnableWorldRendering(this, ConfigurationData.rendering);
		ConfigurationData.pathmedia = (ConfigurationData.pathmedia != TEXT("")) ? ConfigurationData.pathmedia + TEXT("\\media") : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");
		LoadConfigSystemsNative();
	}
	else
	{
		OnErrorMessage(GameRoot + TEXT(" Not Found"));
		UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
	}

}

void UMainInterface::LoadConfigSystemsNative()
{
	bInputEnable = false;
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
			}
		}

		LoadListNative();
		CreateFolders();

		//BlueprintImplementableEvent
		LoadConfigSystems();
	}
	else
	{
		SetDebugMessage(TEXT("Creating games list, loading..."));
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
			MaxFrameMove = 4;
		}
		SetPaddingCovers();
		CreateCardsCoversWidget(0, GameData.Num());
		LoadFirstImages();
		SetPaddingCovers();

		ImgFrame->SetBrushFromTexture(ImageFrameCenter);
		cardReference[0]->SetFocusCard(true);

		//Timer
		GetWorld()->GetTimerManager().SetTimer(DelayLoadListTimerHandle, this, &UMainInterface::ViewList, 0.25f, false, -1);

		//BlueprintImplementableEvent
		LoadList();
	}
	else
	{
		if (bFilterFavorites)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Favorites"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No GameList"));
			SetDebugMessage(TEXT("gamelist.xml not found in " + GameSystems[CountSystem].RomPath));
		}
	}

}

void UMainInterface::ViewList()
{
	UUserWidget::PlayAnimationForward(LoadListGame);
	ImgFrame->SetRenderOpacity(1.0f);
	ScrollListGame->ScrollWidgetIntoView(coverReference[IndexCard], false, EDescendantScrollDestination::Center, 0);
	OnNavigationFocus(cardReference[IndexCard]);
	bInputEnable = true;

}

void UMainInterface::SetPaddingCovers()
{
	for (int32 i = 0; i < 15; i++) {
		UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), coverClass);
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
		GameSystems = UClassicFunctionLibrary::SetConfigSystem(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("config.system")));
		GameSystems = UClassicFunctionLibrary::SortConfigSystem(GameSystems);
		for (int32 i = 0; i < GameSystems.Num(); i++)
		{
			GameRoot = GameSystems[i].RomPath + TEXT("\\gamelist.xml");
			IsValidConfig = UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot);
			if (IsValidConfig)
			{
				GameData = UClassicFunctionLibrary::SetGameData(UClassicFunctionLibrary::LoadXML(ConfigResult, TEXT("gameList.game")));
				GameData = UClassicFunctionLibrary::SortGameDate(GameData);
				GameData = UClassicFunctionLibrary::FormatGameData(GameData, ConfigurationData, GameSystems[i]);
				GameSystems[i].GameDatas = GameData;
				GameData.Empty();
			}
			else
			{
				OnErrorMessage(GameRoot + TEXT(" Not Found"));
				UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
			}
		}
		ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave = GameSystems;
		SaveGame();
		SetDebugMessage(TEXT("Game list created successfully wait..."));
		GetWorld()->GetTimerManager().SetTimer(DelayReloadTimerHandle, this, &UMainInterface::RestartWidget, 3.0f, false, -1);
	}
	else
	{
		OnErrorMessage(GameRoot + TEXT(" Not Found"));
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

		bKeyPressed = (Input != EButtonsGame::A && Input != EButtonsGame::NONE);

		if (!bScroll && !bKeyTriggerLeft && !bKeyTriggerRight)
		{
			if (Input == EButtonsGame::B || Input == EButtonsGame::Y)
			{
				ENavigationButton = Input;
			}
		}

		if (Input == EButtonsGame::LB)
		{
			ENavigationButton = Input;
			bKeyTriggerLeft = true;
		}
		else if (Input == EButtonsGame::RB)
		{
			ENavigationButton = Input;
			bKeyTriggerRight = true;
		}
	}
	else
	{
		switch (UClassicFunctionLibrary::GetInputButton(InKeyEvent))
		{
		case EButtonsGame::SELECT: ENavigationBack = EButtonsGame::SELECT; break;
		case EButtonsGame::A: ENavigationA = EButtonsGame::A; break;
		}
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UMainInterface::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bInputEnable)
	{
		KeyEvent = InKeyEvent;
		bKeyPressed = false;
		bUpDownPressed = true;
		bDelayFavoriteClick = false;
		bKeyTriggerLeft = false;
		bKeyTriggerRight = false;

		if (ENavigationButton == EButtonsGame::B)
		{
			OnClickBackAction();
		}
	}
	else
	{
		ENavigationBack = EButtonsGame::NONE;
		ENavigationA = EButtonsGame::NONE;
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}


void UMainInterface::OnAnimationStartedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationStartedPlaying(Player);
	if (PositionY == EPositionY::CENTRAL)
	{
		ImgFrame->SetBrushFromTexture(ImageFrameCenter);
	}
	const UWidgetAnimation* AnimationGet = Player.GetAnimation();
}

void UMainInterface::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	if (PositionY == EPositionY::TOP)
	{
		ImgFrame->SetBrushFromTexture(ImageFrameTop);
	}

	const UWidgetAnimation* AnimationGet = Player.GetAnimation();
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
		if (GameData.IsValidIndex(i))
		{
			CreateCardCoverWidget(GameData[i]);
		}
		else
		{
			break;
		}
	}
}

void UMainInterface::CreateCardCoverWidget(FGameData Data)
{
	UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), coverClass);
	UCard* Card = CreateWidget<UCard>(GetOwningPlayer(), cardClass);

	Card->SetPlayers(Data.players);
	Card->SetPath(Data.PathFormated);
	Card->SetFavorite(Data.favorite, false);
	Card->OnClickTrigger.AddDynamic(this, &UMainInterface::OnNativeClick);

	cardReference.Add(Card);
	coverReference.Add(Cover);

	HBListGame->AddChild(Card);
	ScrollListGame->AddChild(Cover);

	Cover->SetVisibility(ESlateVisibility::Hidden);
	Card->SetVisibility(ESlateVisibility::Hidden);

}

void UMainInterface::CreateGameSystems()
{
	TArray<FConfigSystem> Systems = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave;

	if (buttonSystemClass != nullptr)
	{
		UClassicButtonSystem* ButtonSystem = nullptr;
		for (int32 i = 0; i < Systems.Num(); i++) {
			ButtonSystem = CreateWidget<UClassicButtonSystem>(GetOwningPlayer(), buttonSystemClass);
			ButtonSystem->SetText(Systems[i].SystemLabel, i);
			ButtonSystem->OnClickTrigger.AddDynamic(this, &UMainInterface::OnNativeClickSystem);
			ButtonSystemReferences.Add(ButtonSystem);
			WBPSystemsList->ScrollBoxSystems->AddChild(ButtonSystem);
		}
	}
	else
	{
		OnErrorMessage(TEXT("buttonSystemClass Not Found"));
		UE_LOG(LogTemp, Warning, TEXT("buttonSystemClass Not Found"));
	}
}

void UMainInterface::OnNativeNavigationGame(EButtonsGame Navigate)
{
	//UE_LOG(LogTemp, Warning, TEXT("broadcast navigate"));	

	if (Navigate == EButtonsGame::LEFT || Navigate == EButtonsGame::RIGHT)
	{
		if (bInputEnable && bDelayPressed && bUpDownPressed && !bKeyTriggerLeft && !bKeyTriggerRight)
		{
			OnNavigationGame(Navigate);
			//if (Focus == EFocus::MAIN) { OnNativeNavigationMain(Navigate); }
			//if (Focus == EFocus::SYSTEM) { OnNativeNavigationSystem(Navigate); }
			//if (Focus == EFocus::INFO) { OnNativeNavigationInfo(Navigate); }
		}

	}
	else if (Navigate == EButtonsGame::UP || Navigate == EButtonsGame::DOWN)
	{
		if (!bScroll && bInputEnable && bDelayPressed && bUpDownPressed && !bKeyTriggerLeft && !bKeyTriggerRight)
		{
			OnNavigationGame(Navigate);
			//if (Focus == EFocus::MAIN) { OnNativeNavigationMain(Navigate); }
			//if (Focus == EFocus::SYSTEM) { OnNativeNavigationSystem(Navigate); }
			//if (Focus == EFocus::INFO) { OnNativeNavigationInfo(Navigate); }
		}
	}


}

void UMainInterface::OnNativeNavigationMain(EButtonsGame Navigate)
{
	PressedDelayNavigation(TimerDelayAnimation);
	ENavigationButton = Navigate;
}

void UMainInterface::OnNativeNavigationSystem(EButtonsGame Navigate)
{
	PressedDelayNavigation(0.13f);
	ENavigationButton = Navigate;
}

void UMainInterface::OnNativeNavigationInfo(EButtonsGame Navigate)
{
	PressedDelayNavigation(0.13f);
	ENavigationButton = Navigate;
}

void UMainInterface::OnNativeClick(FString Value)
{
	//this function is BlueprintImplementableEvent
	OnClickPathEvent(Value);
}

void UMainInterface::OnNativeClickSystem(int32 Value)
{
	CountSystem = Value;
	ClearData(true, true);

	//this function is BlueprintImplementableEvent
	OnClickSystem(Value);
}

void UMainInterface::OnNavigationFocus(UCard* Card)
{
	IndexCard = cardReference.Find(Card);

	const FString Title = GameData[IndexCard].nameFormated;
	if (Title.Len() > 51)
	{
		TxtTitleGame->SetJustification(ETextJustify::Left);
	}
	else
	{
		TxtTitleGame->SetJustification(ETextJustify::Center);
	}
	TxtTitleGame->SetText(FText::FromString(Title));
	TxtDescription->SetText(FText::FromString(GameData[IndexCard].descFormated));
	ScrollListGame->ScrollWidgetIntoView(coverReference[IndexCard], true, EDescendantScrollDestination::Center, 0);
	SetButtonsIconInterfaces(PositionY);
	LoadImages();
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
		if (GameData.IsValidIndex(IndexCard))
		{
			TextCenter[5] = (GameData[IndexCard].favorite) ? TEXT("Remove Favorite") : TEXT("Add Favorite");
		}
		else
		{
			TextCenter[5] = TEXT("Favorite");
		}
		WBPButtonsIconsInterfaces->SetButtonsText(TextCenter);
		WBPButtonsIconsInterfaces->SetButtonsVisibility(IconCenter);
	}
}

UTexture2D* UMainInterface::SetImageFromPath(FString PathImage)
{
	bool isValid = false;
	int32 Size = 64;
	return  UClassicFunctionLibrary::LoadTexture2DFromFile(PathImage, isValid, EClassicImageFormat::PNG, Size, Size);

}

void UMainInterface::ForceGarbageCollectionBP(float Count)
{
	if (CountGarbageCollection > Count) {
		if (GEngine) {
			GEngine->ForceGarbageCollection(true);
			CountGarbageCollection = 0;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Collect Garbage collection");
		}
	}
}

void UMainInterface::ASyncLoadCard(FString PathImage, int32 Index)
{
	FLoadImageDelegate ImgDelegate;
	ImgDelegate.BindUFunction(this, FName("ImageOut"));
	UClassicFunctionLibrary::AsyncLoadTexture2DFromFile(ImgDelegate, PathImage, Index);
}

void UMainInterface::ImageOut(UTexture2D* TextureOut, int32 Index)
{
	if (cardReference.IsValidIndex(Index) && coverReference.IsValidIndex(Index))
	{
		UCard* Card = cardReference[Index];
		UCover* Cover = coverReference[Index];
		int32 ImageX = GameData[Index].ImageX;
		int32 ImageY = GameData[Index].ImageY;

		AsyncTask(ENamedThreads::GameThread_Local, [Card, Cover, TextureOut, Index, ImageX, ImageY]()
			{
				Cover->LoadCoverImage(TextureOut, ImageX, ImageY);
				Card->LoadImageCard(TextureOut, ImageX, ImageY);
				//AddImagesCardCover(TextureOut, Index);
			});
	}
}

void UMainInterface::LoadFirstImages()
{
	const int32 Length = FMath::Clamp(GameData.Num(), 0, 15);
	for (int32 i = 0; i < Length; i++)
	{
		ImageCard = SetImageFromPath(GameData[i].imageFormated);
		AddImagesCardCover(ImageCard, i);
		coverReference[i]->SetVisibility(ESlateVisibility::Visible);
		cardReference[i]->SetVisibility(ESlateVisibility::Visible);
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

		if (GameData.IsValidIndex(LastIndex) && cardReference.IsValidIndex(LastIndex))
		{
			if (/*bKeyTriggerLeft || bKeyTriggerRight &&*/ IndexAsyncImage <= LastIndex)
			{
				ASyncLoadCard(GameData[LastIndex].imageFormated, LastIndex);
				LoadImageSync(LastIndex);
			}
			else
			{
				LoadImageAsync(LastIndex);
			}
			coverReference[LastIndex]->SetVisibility(ESlateVisibility::Visible);
			cardReference[LastIndex]->SetVisibility(ESlateVisibility::Visible);
		}

		if (cardReference.IsValidIndex(FirstIndex)) {
			coverReference[FirstIndex]->SetVisibility(ESlateVisibility::Hidden);
			cardReference[FirstIndex]->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}

void UMainInterface::SetImagesCard(UTexture2D* Texture, UCard* Card, int32 Index)
{
	Card->LoadImageCard(Texture, GameData[Index].ImageX, GameData[Index].ImageY);
}

void UMainInterface::SetImagesCover(UTexture2D* Texture, UCover* Cover, int32 Index)
{
	Cover->LoadCoverImage(Texture, GameData[Index].ImageX, GameData[Index].ImageY);
}

void UMainInterface::AddImagesCardCover(UTexture2D* Texture, int32 Index)
{
	if (cardReference.IsValidIndex(Index) && coverReference.IsValidIndex(Index))
	{
		SetImagesCard(Texture, cardReference[Index], Index);
		SetImagesCover(Texture, coverReference[Index], Index);
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

			cardReference[IndexCard]->SetFavorite(ToggleFavorite, true);

			FString Path = ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].RomPath;
			SaveGameListXML(Path, ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave[CountSystem].GameDatas);
			SaveGameList();

			SetButtonsIconInterfaces(EPositionY::CENTRAL);
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
		if (cardReference.IsValidIndex(IndexCard))
		{
			cardReference[IndexCard]->SetFocusCard(true);
		}
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
}

void UMainInterface::ClearData(bool bAnimationBarTop, bool bAnimationShowSystem)
{
	bInputEnable = false;

	TxtDebug->SetVisibility(ESlateVisibility::Hidden);
	TxtDebug->SetText(FText::FromString(""));

	ImgFrame->SetRenderOpacity(0.f);
	UUserWidget::PlayAnimationReverse(LoadListGame);

	HBListGame->ClearChildren();
	HBListGame->SetRenderTranslation(FVector2D(385.0f, 0.f));

	ScrollListGame->ClearChildren();
	ScrollListGame->ScrollToStart();

	Focus = EFocus::MAIN;
	cardReference.Empty();
	coverReference.Empty();
	GameData.Empty();
	NewGameData.Empty();
	IndexCard = 0;
	PositionCenterX = 1;
	PositionY = EPositionY::CENTRAL;

	bUpDownPressed = true;
	bDelayPressed = true;

	ImgFrame->SetBrushFromTexture(ImageFrameCenter);
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

//Animations
void UMainInterface::AnimationFrameMoveRight()
{
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
	FrameMoveRight();

}

void UMainInterface::AnimationFrameMoveLeft()
{
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
		UUserWidget::PlayAnimationReverse(FrameAnimationX2);
		break;
	case 3:
		UUserWidget::PlayAnimationReverse(FrameAnimationX3);
		break;
	}
	FrameMoveLeft();
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
	if (ENavigationButton == EButtonsGame::LEFT)
	{
		UUserWidget::PlayAnimationReverse(FrameAnimationXTop1);
	}
	else if (ENavigationButton == EButtonsGame::UP)
	{
		AnimationFrameToTop(FrameAnimationY1ToSystems, FrameAnimationY2ToSystems, FrameAnimationY3ToSystems, FrameAnimationY4ToSystems, false);
	}
}

void UMainInterface::OnFocusConfigurations()
{
	PositionTopX = 2;
	SetToolTip(WBPToolTipConfiguration);
	WBPToolTipConfiguration->SetToolTipVisibility(ESlateVisibility::Visible);
	if (ENavigationButton == EButtonsGame::LEFT)
	{
		UUserWidget::PlayAnimationReverse(FrameAnimationXTop2);
	}
	else if (ENavigationButton == EButtonsGame::RIGHT)
	{
		UUserWidget::PlayAnimationForward(FrameAnimationXTop1);
	}
	else if (ENavigationButton == EButtonsGame::UP)
	{
		AnimationFrameToTop(FrameAnimationY1ToConfig, FrameAnimationY2ToConfig, FrameAnimationY3ToConfig, FrameAnimationY4ToConfig, false);
	}
}

void UMainInterface::OnFocusFavorites()
{
	PositionTopX = 3;
	SetToolTip(WBPToolTipFavorites);
	WBPToolTipFavorites->SetToolTipVisibility(ESlateVisibility::Visible);
	if (ENavigationButton == EButtonsGame::LEFT)
	{
		UUserWidget::PlayAnimationReverse(FrameAnimationXTop3);
	}
	else if (ENavigationButton == EButtonsGame::RIGHT)
	{
		UUserWidget::PlayAnimationForward(FrameAnimationXTop2);
	}
	else if (ENavigationButton == EButtonsGame::UP)
	{
		AnimationFrameToTop(FrameAnimationY1ToFavorite, FrameAnimationY2ToFavorite, FrameAnimationY3ToFavorite, FrameAnimationY4ToFavorite, false);
	}
}

void UMainInterface::OnFocusInfo()
{
	WBPInfo->SetGameInfo(GameData[IndexCard]);
	PositionTopX = 4;
	SetToolTip(WBPToolTipInfo);
	WBPToolTipInfo->SetToolTipVisibility(ESlateVisibility::Visible);
	if (ENavigationButton == EButtonsGame::RIGHT)
	{
		UUserWidget::PlayAnimationForward(FrameAnimationXTop3);
	}
	else if (ENavigationButton == EButtonsGame::UP)
	{
		AnimationFrameToTop(FrameAnimationY1ToInfo, FrameAnimationY2ToInfo, FrameAnimationY3ToInfo, FrameAnimationY4ToInfo, false);
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
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B)
	{
		AnimationFrameToTop(FrameAnimationY1ToSystems, FrameAnimationY2ToSystems, FrameAnimationY3ToSystems, FrameAnimationY4ToSystems, true);
	}
}

void UMainInterface::OnLostFocusConfigurations()
{
	WBPToolTipConfiguration->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B)
	{
		AnimationFrameToTop(FrameAnimationY1ToConfig, FrameAnimationY2ToConfig, FrameAnimationY3ToConfig, FrameAnimationY4ToConfig, true);
	}
}

void UMainInterface::OnLostFocusFavorites()
{
	WBPToolTipFavorites->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B)
	{
		AnimationFrameToTop(FrameAnimationY1ToFavorite, FrameAnimationY2ToFavorite, FrameAnimationY3ToFavorite, FrameAnimationY4ToFavorite, true);
	}
}

void UMainInterface::OnLostFocusInfo()
{
	WBPToolTipInfo->SetToolTipVisibility(ESlateVisibility::Collapsed);
	if (ENavigationButton == EButtonsGame::DOWN || ENavigationButton == EButtonsGame::B)
	{
		AnimationFrameToTop(FrameAnimationY1ToInfo, FrameAnimationY2ToInfo, FrameAnimationY3ToInfo, FrameAnimationY4ToInfo, true);
	}
}

void UMainInterface::OnClickSelectSystem()
{
	UUserWidget::PlayAnimationForward(ShowSystem);
	PositionY = EPositionY::TOP;
	ButtonSystemReferences[CountLocationY]->Click->SetKeyboardFocus();
	Focus = EFocus::SYSTEM;
}

void UMainInterface::OnClickConfigurations()
{
}

void UMainInterface::OnClickFavorites()
{
	if (bFilterFavorites)
	{
		bFilterFavorites = false;
		OnNativeNavigationGame(EButtonsGame::DOWN);
		UUserWidget::PlayAnimationForward(LoadListGame);
		ClearData(false, false);
	}
	else
	{
		const int32 Length = UClassicFunctionLibrary::FilterFavoriteGameData(GameData, true).Num();
		if (Length > 0)
		{
			bFilterFavorites = true;
			OnNativeNavigationGame(EButtonsGame::DOWN);
			UUserWidget::PlayAnimationForward(LoadListGame);
			ClearData(false, false);
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
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
		break;
	case EPositionY::BOTTOM:
		OnNativeNavigationGame(EButtonsGame::UP);
		break;
	default:
		break;
	}
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

	if (TranslationSystemSelect == 0)
	{
		UUserWidget::PlayAnimationReverse(ShowSystem);
		BtnSelectSystem->BtButton->SetKeyboardFocus();
		Focus = EFocus::MAIN;
	}
	else if (TranslationInfo == 0)
	{
		UUserWidget::PlayAnimationReverse(ShowInfo);
		BtnInfo->BtButton->SetKeyboardFocus();
		Focus = EFocus::MAIN;
	}
	else
	{
		OnNativeNavigationGame(EButtonsGame::DOWN);
	}
}

void UMainInterface::CreateFolders()
{
	const FString PathMedia = (ConfigurationData.pathmedia != TEXT("")) ? ConfigurationData.pathmedia + TEXT("\\media") : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");

	UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia);

	for (FConfigSystem& ConfigElement : GameSystems)
	{
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName);
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName + TEXT("\\covers"));
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName + TEXT("\\screenshots"));
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + ConfigElement.SystemName + TEXT("\\videos"));
	}

}

void UMainInterface::ScrollCards()
{
	if (PositionY == EPositionY::CENTRAL && HBListGame->RenderTransform.Translation.Y == 0)
	{

		const float FrameX = ImgFrame->RenderTransform.Translation.X;

		if (PositionCenterX == 1 && FrameX == 0)
		{
			ENavigationScroll = EButtonsGame::LEFT;
		}
		else if (PositionCenterX == 4 && FrameX == 1155)
		{
			ENavigationScroll = EButtonsGame::RIGHT;
		}

		if ((PositionCenterX == 1 || PositionCenterX == 4) && (FrameX == 0 || FrameX == 1155))
		{
			bScroll = false;
			const int32 HbGetPosition = HBListGame->RenderTransform.Translation.X;
			int32 HbNewPosition;
			int32 Min;
			int32 Max;
			if (ENavigationScroll == EButtonsGame::RIGHT)
			{
				Min = (IndexCard - PositionCenterX) * -385;
				Max = (IndexCard - PositionCenterX - 1) * -385;
				//UE_LOG(LogTemp, Warning, TEXT("Min %d  Max %d"), Min, Max);

				HbNewPosition = FMath::Clamp(HbGetPosition - SpeedScroll, Min, Max);
				HBListGame->SetRenderTranslation(FVector2D(HbNewPosition, 0));
				bScroll = HbGetPosition != HbNewPosition;
			}
			else if (ENavigationScroll == EButtonsGame::LEFT)
			{
				Min = IndexCard * -385;
				Max = (IndexCard - PositionCenterX) * -385;
				//UE_LOG(LogTemp, Warning, TEXT("Min %d  Max %d"), Min, Max);

				HbNewPosition = FMath::Clamp(HbGetPosition + SpeedScroll, Min, Max);
				HBListGame->SetRenderTranslation(FVector2D(HbNewPosition, 0));
				bScroll = HbGetPosition != HbNewPosition;
			}

		}
	}

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
		UTexture2D* ImageLoaded = UClassicFunctionLibrary::LoadTexture2DFromFile(ImagePath, IsValidTexture, EClassicImageFormat::PNG, Size, Size);
		if (IsValidTexture)
		{
			ImgImageBottom->SetBrushFromTexture(ImageLoaded);
		}
	}
	else
	{
		ImgImageBottom->SetBrushFromTexture(ImageNull);
	}

}
