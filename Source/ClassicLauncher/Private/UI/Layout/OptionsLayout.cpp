// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/OptionsLayout.h"
#include "UI/Screens/MainScreen.h"
#include "UI/Components/ButtonSlide.h"
#include "Components/WidgetSwitcher.h"
#include "Audio/ClassicMediaPlayer.h"
#include "Core/ClassicGameinstance.h"
#include "Core/ClassicGameMode.h"
#include "Core/ScreenManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "TextImageBlock.h"
#include "Data/DataManager.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "UI/Components/ButtonCheckBox.h"
#include "UI/Layout/Header.h"
#include "UI/Components/ButtonCommon.h"
#include "UI/Components/ButtonCommonAlternative.h"
#include "UI/Components/ScrollBoxEnhanced.h"
#include "UI/Layout/Modal.h"

#define LOCTEXT_NAMESPACE "ButtonsConfiguration"

void UOptionsLayout::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UOptionsLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);

	DataManager = GetWorld()->GetSubsystem<UDataManager>();

	SlideVolumeSystem->OnSlideTrigger.AddDynamic(this, &UOptionsLayout::OnSlideVolumeMaster);
	SlideVolumeSystem->OnFocusLostTriggerSlide.AddDynamic(this, &UOptionsLayout::OnSlideLostFocus);
	SlideVolumeMusic->OnSlideTrigger.AddDynamic(this, &UOptionsLayout::OnSlideVolumeMusic);
	SlideVolumeMusic->OnFocusLostTriggerSlide.AddDynamic(this, &UOptionsLayout::OnSlideLostFocus);
	SlideVolumeVideo->OnSlideTrigger.AddDynamic(this, &UOptionsLayout::OnSlideVolumeVideo);
	SlideVolumeVideo->OnFocusLostTriggerSlide.AddDynamic(this, &UOptionsLayout::OnSlideLostFocus);
	BtnUpdateGameList->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickUpdate);
	BtnChangeSystems->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickChangeSystems);
	BtnDeviceInfo->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickDevice);
	BtnLicenseInfo->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickLicense);
	BtnLanguage->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickLanguage);
	BtnChangeTheme->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickChangeTheme);
	BtnPrompt->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickPrompt);
	CbVideoAudio->CheckDelegate.AddDynamic(this, &UOptionsLayout::OnSoundVideo);
	BtnEnglish->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickChangeLanguage);
	BtnPortuguese->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickChangeLanguage);

	SetFolderThemes();
}

FReply UOptionsLayout::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UOptionsLayout::SetSlide(FConfig& Configuration)
{
	SlideVolumeSystem->SetSlideValue(FMath::Clamp(Configuration.VolumeMaster, 0, 100));
	SlideVolumeMusic->SetSlideValue(FMath::Clamp(Configuration.VolumeMusic, 0, 100));
	SlideVolumeVideo->SetSlideValue(FMath::Clamp(Configuration.VolumeVideo, 0, 100));
}

void UOptionsLayout::OnSlideVolumeMaster(int32 Value)
{
	if (IsValid(ClassicMediaPlayerReference))
	{
		ClassicMediaPlayerReference->ChangeMasterVolume(Value);
	}
}

void UOptionsLayout::OnSlideVolumeMusic(int32 Value)
{
	if (IsValid(ClassicMediaPlayerReference))
	{
		ClassicMediaPlayerReference->ChangeMusicVolume(Value);
	}
}

void UOptionsLayout::OnSlideVolumeVideo(int32 Value)
{
	if (IsValid(ClassicMediaPlayerReference))
	{
		ClassicMediaPlayerReference->ChangeVideoVolume(Value);
	}
}

void UOptionsLayout::OnSlideLostFocus()
{
	if (!IsValid(MainInterfaceReference) && !IsValid(ClassicMediaPlayerReference))
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference ClassicMediaPlayerReference error"));
		return;
	}
	bool bSave = false;
	FConfig Config = MainInterfaceReference->ConfigurationData;

	if (Config.VolumeMaster != ClassicMediaPlayerReference->GetMasterVolume())
	{
		Config.VolumeMaster = ClassicMediaPlayerReference->GetMasterVolume();
		bSave = true;
	}
	if (Config.VolumeMusic != ClassicMediaPlayerReference->GetMusicVolume())
	{
		Config.VolumeMusic = ClassicMediaPlayerReference->GetMusicVolume();
		bSave = true;
	}
	if (Config.VolumeVideo != ClassicMediaPlayerReference->GetVideoVolume())
	{
		Config.VolumeVideo = ClassicMediaPlayerReference->GetVideoVolume();
		bSave = true;
	}

	if (bSave)
	{
		MainInterfaceReference->ConfigurationData = Config;
		UClassicFunctionLibrary::SaveConfig(Config);
		UE_LOG(LogTemp, Warning, TEXT("saving config"));
	}
}

void UOptionsLayout::OnClickUpdate(int32 Value)
{
	UClassicGameInstance* ClassicGameInstance = Cast<UClassicGameInstance>(GetGameInstance());
	if (ClassicGameInstance->DeleteGameSystemSave())
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleted Saved"));
		MainInterfaceReference->SetInputEnable(false);
		MainInterfaceReference->Header->SetFocusButton();
		GetWorld()->GetTimerManager().SetTimer(RestartMapTimerHandle, this, &UOptionsLayout::RestartMap, 3.0f, false, -1);
		if (MainInterfaceReference != nullptr)
		{
			const FText Message = LOCTEXT("UpdateGame", "Update game wait");
			MainInterfaceReference->ShowMessage(Message, 2.5f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference or ClassicMediaPlayerReference references error"));
	}
}

void UOptionsLayout::OnClickChangeSystems(int32 Value)
{
	//open new screen 
}

void UOptionsLayout::CloseModal()
{
	SetActiveWidgetIndex(0);
	SetFocusOptionsItem(EButtonsGame::NONE);
}

void UOptionsLayout::OnClickPrompt(int32 Value)
{
	SetActiveWidgetIndex(1);
	SetFocusOptionsItem(EButtonsGame::NONE);
}

void UOptionsLayout::OnClickChangeTheme(int32 Value)
{
	SetActiveWidgetIndex(2);
	SetFocusOptionsItem(EButtonsGame::NONE);
}

void UOptionsLayout::OnClickLanguage(int32 Value)
{
	SetActiveWidgetIndex(3);
	SetFocusOptionsItem(EButtonsGame::NONE);
}

void UOptionsLayout::OnClickDevice(int32 Value)
{
	SetActiveWidgetIndex(4);
	SetFocus();
}

void UOptionsLayout::OnClickLicense(int32 Value)
{
	SetActiveWidgetIndex(5);
	SetFocus();
}

void UOptionsLayout::OnSoundVideo(bool bCheck)
{
	UE_LOG(LogTemp, Warning, TEXT("bcheck is %s"), (bCheck)? TEXT("true"): TEXT("false"));
}

void UOptionsLayout::OnClickChangeLanguage(int32 Value)
{
	const FString CurrentLanguage = UKismetInternationalizationLibrary::GetCurrentCulture();

	if (Value == 0)
	{
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("en"), true);
		GetLanguageText(true);
	}
	else if (Value == 1)
	{
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("pt-BR"), true);
		GetLanguageText(true);
	}

	for (TObjectIterator<UTextImageBlock> ObjectIterator; ObjectIterator; ++ObjectIterator)
	{
		if (ObjectIterator->GetWorld() != GetWorld())
		{
			continue;
		}
		ObjectIterator->UpdateText();
		UE_LOG(LogTemp, Warning, TEXT("Attemped text update %s"), *ObjectIterator->GetName());
	}
}

void UOptionsLayout::GetLanguageText(bool bShowMessage)
{
	const FString CurrentLanguage = UKismetInternationalizationLibrary::GetCurrentCulture();
	const FText TextCurrentLanguage = (CurrentLanguage == TEXT("en")) ? LOCTEXT("LogLanguageen", "English") : LOCTEXT("LogLanguageptbr", "Portuguese (Brazil)");
	const FText Message = FText::Format(LOCTEXT("ChangeLanguageTo", "Change language to {0}"), TextCurrentLanguage);
	if (bShowMessage && MainInterfaceReference != nullptr)
	{
		MainInterfaceReference->ShowMessage(Message, 2.5f);
	}
}

void UOptionsLayout::SetFocusOptionsItem(const EButtonsGame Input)
{
	UScrollBoxEnhanced* ToScroll;
	if (!GetScrollBoxEnhancedWidgetSwitcherIndex(ToScroll)) return;


	UE_LOG(LogTemp, Warning, TEXT("Reference scroll: %s ws index %d"), *ToScroll->GetName(), WSScreens->GetActiveWidgetIndex());

	if (Input == EButtonsGame::UP)
	{
		ToScroll->SetFocusScroll(EScrollTo::UP);
	}
	else if (Input == EButtonsGame::DOWN)
	{
		ToScroll->SetFocusScroll(EScrollTo::DOWN);
	}
	else
	{
		ToScroll->SetFocusScroll(EScrollTo::NONE);
	}
}

void UOptionsLayout::RestartMap()
{
	const AClassicGameMode* GameMode = Cast<AClassicGameMode>(UGameplayStatics::GetGameMode(this));
	WSScreens->SetActiveWidgetIndex(0);
	GameMode->LoadingGameData->SetToRestartWidgets();
}

bool UOptionsLayout::GetScrollBoxEnhancedWidgetSwitcherIndex(UScrollBoxEnhanced*& Scroll) const
{
	switch (WSScreens->GetActiveWidgetIndex())
	{
	case 0: Scroll = ScrollBoxOptions;
		break;
	case 1: Scroll = ScrollBoxPrompt;
		break;
	case 2: Scroll = ScrollBoxThemes;
		break;
	case 3: Scroll = ScrollBoxLanguage;
		break;
	default: Scroll = nullptr;
		break;
	}
	return Scroll != nullptr;
}

void UOptionsLayout::SetActiveWidgetIndex(int32 Index)
{
	WSScreens->SetActiveWidgetIndex(Index);
	const FText* NewTitle = TitleList.Find(Index);
	if (NewTitle != nullptr)
	{
		Modal->SetTitleText(*NewTitle);
	}
}

void UOptionsLayout::SetFolderThemes()
{
	if (ButtonCommonClass != nullptr)
	{
		FoldersThemes.Empty();
		const FString FullFilePath = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("themes");
		UE_LOG(LogTemp,Warning ,TEXT("folder %s") , *FullFilePath);
		UClassicFunctionLibrary::GetFolders(FoldersThemes, FullFilePath);

		for (int32 i = 0; i < FoldersThemes.Num(); i++)
		{
			UButtonCommon* ButtonThemes = CreateWidget<UButtonCommon>(GetOwningPlayer(), ButtonCommonClass);
			ButtonThemes->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnTheme);
			ButtonThemes->SetText(FText::FromString(FoldersThemes[i]));
			ScrollBoxThemes->SetContent(ButtonThemes);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ButtonCommonClass Not Found"));
	}
}

void UOptionsLayout::OnTheme(int32 Value)
{
	if(!FoldersThemes.IsValidIndex(Value)) return;
	
	UE_LOG(LogTemp, Warning, TEXT("theme name %s"), *FoldersThemes[Value]);
}

TArray<FString> UOptionsLayout::GetFoldersThemes()
{
	return FoldersThemes;
}

#undef LOCTEXT_NAMESPACE
