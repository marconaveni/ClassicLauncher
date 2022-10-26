// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicConfigurations.h"
#include "MainInterface.h"
#include "EngineUtils.h"
#include "ClassicButton.h"
#include "ClassicSlide.h"
#include "ClassicButtonSystem.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "ClassicMediaPlayer.h"
#include "ClassicGameinstance.h"
#include "ClassicSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ClassicFunctionLibrary.h"
#include "Kismet/KismetInternationalizationLibrary.h"

#define LOCTEXT_NAMESPACE "ButtonsConfiguration"

void UClassicConfigurations::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UClassicConfigurations::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bFocus = false;
	IndexSelect = 0;

	GetLanguageText();

	SlideVolume->OnSlide.AddDynamic(this, &UClassicConfigurations::OnSlideVolume);
	SlideVolume->OnFocusLostTriggerSlide.AddDynamic(this, &UClassicConfigurations::OnSlideLostFocus);
	BtnUpdateGameList->OnClickTrigger.AddDynamic(this, &UClassicConfigurations::OnClickUpdate);
	BtnDeviceInfo->OnClickTrigger.AddDynamic(this, &UClassicConfigurations::OnClickDevice);
	BtnLicenseInfo->OnClickTrigger.AddDynamic(this, &UClassicConfigurations::OnClickLicense);
	BtnLanguage->OnClickTrigger.AddDynamic(this, &UClassicConfigurations::OnClickLanguage);

	
	for (TObjectIterator<UMainInterface> ObjectIterator; ObjectIterator; ++ObjectIterator)
	{
		if (ObjectIterator->GetWorld() != GetWorld())
		{
			continue;
		}
		MainInterfaceReference = *ObjectIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference UMainInterface Founds: %s "), *MainInterfaceReference->GetName());
	}
	for (TActorIterator<AClassicMediaPlayer> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		ClassicMediaPlayerReference = *ActorIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference AClassicMediaPlayer classicconfigurations Founds: %s "), *ClassicMediaPlayerReference->GetName());
	}
}

FReply UClassicConfigurations::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);

	if (Input == EButtonsGame::B)
	{
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UClassicConfigurations::OnSlideVolume(int32 Value)
{
	if (IsValid(ClassicMediaPlayerReference))
	{
		ClassicMediaPlayerReference->ChangeMasterVolume(Value);
	}
}

void UClassicConfigurations::OnSlideLostFocus()
{
	if (!IsValid(MainInterfaceReference) && !IsValid(ClassicMediaPlayerReference))
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference ClassicMediaPlayerReference error"));
		return;
	}

	FConfig ConfigData = MainInterfaceReference->ConfigurationData;
	const int32 ConfigurationDataVolume = ConfigData.volume;
	const int32 MediaPlayerVolume = ClassicMediaPlayerReference->GetMasterVolume();

	if (MediaPlayerVolume == ConfigurationDataVolume)
	{
		UE_LOG(LogTemp, Warning, TEXT("MediaPlayerVolume %d ConfigurationDataVolume %d are equal"), MediaPlayerVolume, ConfigurationDataVolume);
		return;
	}

	FString RootDir = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");
	ConfigData.pathmedia = (RootDir == ConfigData.pathmedia) ? TEXT("$(remove)") : ConfigData.pathmedia;
	ConfigData.volume = MediaPlayerVolume;
	FString XmlConfig = UClassicFunctionLibrary::CreateXMLConfigFile(ConfigData);
	XmlConfig = XmlConfig.Replace(TEXT("$(remove)"), TEXT(""), ESearchCase::IgnoreCase);
	const FString PathToSave = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config");
	MainInterfaceReference->ConfigurationData = ConfigData;
	const bool Saved = (UClassicFunctionLibrary::SaveStringToFile(PathToSave, TEXT("config.xml"), XmlConfig, true, false));

	UE_LOG(LogTemp, Warning, TEXT("%s"), (Saved) ? TEXT("Saved File") : TEXT("Not Saved File"));
}

void UClassicConfigurations::OnClickUpdate(int32 Value)
{
	bFocus = true;
	UClassicGameInstance* ClassicGameInstance = Cast<UClassicGameInstance>(GetGameInstance());

	if (UGameplayStatics::DeleteGameInSlot(ClassicGameInstance->SlotGame, 0))
	{
		ClassicGameInstance->ClassicSaveGameInstance->ConfigSystemsSave.Empty();
		UE_LOG(LogTemp, Warning, TEXT("Deleted Saved"));
		MainInterfaceReference->bInputEnable = false;
		GetWorld()->GetTimerManager().SetTimer(RestartMapTimerHandle, this, &UClassicConfigurations::RestartMap, 2.5f, false, -1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference or ClassicMediaPlayerReference references error"));
	}
}

void UClassicConfigurations::OnClickDevice(int32 Value)
{
	bFocus = true;
	WSButtons->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceInfo->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogTemp, Warning, TEXT("OnClickDevice"));
}

void UClassicConfigurations::OnClickLicense(int32 Value)
{
	bFocus = true;
	WSButtons->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceLicense->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogTemp, Warning, TEXT("OnClickLicense"));
}

void UClassicConfigurations::OnClickLanguage(int32 Value)
{
	const FString CurrentLanguage = UKismetInternationalizationLibrary::GetCurrentCulture();

	if (CurrentLanguage == TEXT("en"))
	{
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("pt-BR"), true);
		GetLanguageText();
	}
	else
	{
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("en"), true);
		GetLanguageText();
	}
}

void UClassicConfigurations::GetLanguageText()
{
	const FString CurrentLanguage = UKismetInternationalizationLibrary::GetCurrentCulture();
	FText TextLanguage = (CurrentLanguage == TEXT("en")) ? LOCTEXT("LogLanguageen", "English") : LOCTEXT("LogLanguageptbr", "Portuguese");
	BtnLanguage->SetText(TextLanguage.ToString());
}

void UClassicConfigurations::CloseModal()
{
	bFocus = false;
	WSButtons->SetVisibility(ESlateVisibility::Visible);
	WSDeviceInfo->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceLicense->SetVisibility(ESlateVisibility::Hidden);
}

void UClassicConfigurations::SetIndexFocus(EButtonsGame Input)
{
	if (bFocus || bDelayInput) return;

	if (Input == EButtonsGame::UP || Input == EButtonsGame::DOWN)
	{
		bDelayInput = true;
		if (Input == EButtonsGame::DOWN)
		{
			IndexSelect = FMath::Clamp(IndexSelect + 1, 0, 4);
		}
		else if (Input == EButtonsGame::UP)
		{
			IndexSelect = FMath::Clamp(IndexSelect - 1, 0, 4);
		}
		SetFocusSelect();
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UClassicConfigurations::Delay, 0.18f, false, -1);
	}

	const int32 SlideValue = SlideVolume->SlideValue;

	if (IndexSelect == 0)
	{
		if (Input == EButtonsGame::LEFT)
		{
			SlideVolume->SetSlideValue(SlideValue - 1);
		}
		else if (Input == EButtonsGame::RIGHT)
		{
			SlideVolume->SetSlideValue(SlideValue + 1);
		}
	}

}

void UClassicConfigurations::SetFocusSelect()
{
	switch (IndexSelect)
	{
	case 0:
		SlideVolume->SetFocusSlide();
		break;
	case 1:
		BtnUpdateGameList->Click->SetKeyboardFocus();
		break;
	case 2:
		BtnDeviceInfo->Click->SetKeyboardFocus();
		break;
	case 3:
		BtnLicenseInfo->Click->SetKeyboardFocus();
		break;
	case 4:
		BtnLanguage->Click->SetKeyboardFocus();
		break;
	default:
		break;
	}

}

void UClassicConfigurations::Delay()
{
	bDelayInput = false;
}

void UClassicConfigurations::RestartMap()
{
	UGameplayStatics::OpenLevel(this, FName("map"), true);
}

#undef LOCTEXT_NAMESPACE