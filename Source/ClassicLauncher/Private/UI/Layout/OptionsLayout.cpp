// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/OptionsLayout.h"
#include "UI/Screens/MainScreen.h"
#include "UI/Components/ButtonSlide.h"
#include "Components/WidgetSwitcher.h"
#include "Audio/ClassicMediaPlayer.h"
#include "Core/ClassicGameinstance.h"
#include "Core/ScreenManager.h"
#include "Engine/World.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "TextImageBlock.h"
#include "Data/DataManager.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "UI/Components/ButtonCheckBox.h"
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
	if (DataManager->GetClassicMediaPlayerReference() != nullptr)
	{
		DataManager->GetClassicMediaPlayerReference()->ChangeMasterVolume(Value);
	}
}

void UOptionsLayout::OnSlideVolumeMusic(int32 Value)
{
	if (DataManager->GetClassicMediaPlayerReference() != nullptr)
	{
		DataManager->GetClassicMediaPlayerReference()->ChangeMusicVolume(Value);
	}
}

void UOptionsLayout::OnSlideVolumeVideo(int32 Value)
{
	if (DataManager->GetClassicMediaPlayerReference() != nullptr)
	{
		DataManager->GetClassicMediaPlayerReference()->ChangeVideoVolume(Value);
	}
}

void UOptionsLayout::OnSlideLostFocus()
{
	if (DataManager->GetClassicMediaPlayerReference() != nullptr)
	{
		DataManager->GetClassicMediaPlayerReference()->SetVolumeSave();
	}
}

void UOptionsLayout::OnClickUpdate(int32 Value)
{
	UClassicGameInstance* ClassicGameInstance = Cast<UClassicGameInstance>(GetGameInstance());
	if (ClassicGameInstance->DeleteGameSystemSave())
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleted Saved"));
		WSScreens->SetActiveWidgetIndex(0);
		DataManager->GetScreenManager()->SetToRestartWidgets();
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
	if (bShowMessage && DataManager->GetMainScreenReference() != nullptr)
	{
		DataManager->GetMainScreenReference()->ShowMessage(Message, 2.5f);
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
