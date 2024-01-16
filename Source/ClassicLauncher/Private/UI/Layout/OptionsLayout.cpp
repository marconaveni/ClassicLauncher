// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/OptionsLayout.h"
#include "UI/Screens/MainScreen.h"
#include "EngineUtils.h"
#include "UI/Components/ButtonSlide.h"
#include "Components/WidgetSwitcher.h"
#include "Audio/ClassicMediaPlayer.h"
#include "Core/ClassicGameinstance.h"
#include "Core/ClassicGameMode.h"
#include "Core/ClassicSaveGame.h"
#include "Core/ScreenManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "TextImageBlock.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "UI/Layout/Header.h"
#include "UI/Components/ButtonCommon.h"
#include "UI/Components/ScrollBoxEnhanced.h"

#define LOCTEXT_NAMESPACE "ButtonsConfiguration"

void UOptionsLayout::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UOptionsLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bFocus = false;
	Index = 0;

	SetIsFocusable(true);

	SlideVolume->OnSlideTrigger.AddDynamic(this, &UOptionsLayout::OnSlideVolume);
	SlideVolume->OnFocusLostTriggerSlide.AddDynamic(this, &UOptionsLayout::OnSlideLostFocus);
	BtnUpdateGameList->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickUpdate);
	BtnDeviceInfo->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickDevice);
	BtnLicenseInfo->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickLicense);
	BtnLanguage->OnClickTrigger.AddDynamic(this, &UOptionsLayout::OnClickLanguage);
	
	
	for (TObjectIterator<UMainScreen> ObjectIterator; ObjectIterator; ++ObjectIterator)
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

	GetLanguageText(false);
}

FReply UOptionsLayout::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);

	if (Input == EButtonsGame::B)
	{
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UOptionsLayout::OnSlideVolume(int32 Value)
{
	if (IsValid(ClassicMediaPlayerReference))
	{
		ClassicMediaPlayerReference->ChangeMasterVolume(Value);
	}
}

void UOptionsLayout::OnSlideLostFocus()
{
	if (!IsValid(MainInterfaceReference) && !IsValid(ClassicMediaPlayerReference))
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference ClassicMediaPlayerReference error"));
		return;
	}

	if (MainInterfaceReference->ConfigurationData.Volume != ClassicMediaPlayerReference->GetMasterVolume())
	{
		MainInterfaceReference->ConfigurationData.Volume = ClassicMediaPlayerReference->GetMasterVolume();
		UClassicFunctionLibrary::SaveConfig(MainInterfaceReference->ConfigurationData);
		UE_LOG(LogTemp, Warning, TEXT("saving config"));
	}
}

void UOptionsLayout::OnClickUpdate(int32 Value)
{
	bFocus = true;
	UClassicGameInstance* ClassicGameInstance = Cast<UClassicGameInstance>(GetGameInstance());
	ClassicGameInstance->ClassicSaveGameInstance->GameSystemsSave.Empty();

	if (UGameplayStatics::DeleteGameInSlot(ClassicGameInstance->SlotGame, 0))
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

void UOptionsLayout::OnClickDevice(int32 Value)
{
	bFocus = true;
	WSButtons->SetRenderOpacity(0);
	WSDeviceInfo->SetRenderOpacity(1);
	WSButtons->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceLicense->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetFocus();
	UE_LOG(LogTemp, Warning, TEXT("OnClickDevice"));
	
}

void UOptionsLayout::OnClickLicense(int32 Value)
{
	bFocus = true;
	WSButtons->SetRenderOpacity(0);
	WSDeviceLicense->SetRenderOpacity(1);
	WSButtons->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceInfo->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceLicense->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetFocus();
	UE_LOG(LogTemp, Warning, TEXT("OnClickLicense"));
}

void UOptionsLayout::OnClickLanguage(int32 Value)
{
	const FString CurrentLanguage = UKismetInternationalizationLibrary::GetCurrentCulture();

	if (CurrentLanguage == TEXT("en"))
	{
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("pt-BR"), true);
		GetLanguageText(true);
	}
	else
	{
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("en"), true);
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

void UOptionsLayout::CloseModal()
{
	bFocus = false;
	WSButtons->SetRenderOpacity(1);
	WSDeviceInfo->SetRenderOpacity(0);
	WSDeviceLicense->SetRenderOpacity(0);
	WSButtons->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	WSDeviceInfo->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceLicense->SetVisibility(ESlateVisibility::Hidden);
	SetFocusItem(EButtonsGame::NONE);
}

void UOptionsLayout::SetFocusItem(const EButtonsGame Input)
{
	if (bFocus) return;
	
	if (Input == EButtonsGame::UP)
	{
		Index = ScrollBox->SetFocusScroll(EScrollTo::UP);
	}
	else if (Input == EButtonsGame::DOWN)
	{
		Index = ScrollBox->SetFocusScroll(EScrollTo::DOWN);
	}
	else
	{
		Index = ScrollBox->SetFocusScroll(EScrollTo::NONE);
	}
}

void UOptionsLayout::SetFocusSelect(const bool bIsSound)
{
	switch (Index)
	{
	case 0:
		SlideVolume->SetFocus();
		break;
	case 1:
		BtnUpdateGameList->SetFocus();
		break;
	case 2:
		BtnDeviceInfo->SetFocus();
		break;
	case 3:
		BtnLicenseInfo->SetFocus();
		break;
	case 4:
		BtnLanguage->SetFocus();
		break;
	default:
		break;
	}

}

void UOptionsLayout::Delay()
{
	bDelayInput = false;
}

void UOptionsLayout::RestartMap()
{
	const AClassicGameMode* GameMode = Cast<AClassicGameMode>(UGameplayStatics::GetGameMode(this));
	Index = 0;
	bFocus = false;
	bDelayInput = false;
	GameMode->LoadingGameData->SetToRestartWidgets();
}

#undef LOCTEXT_NAMESPACE