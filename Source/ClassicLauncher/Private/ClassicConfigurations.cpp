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

void UClassicConfigurations::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UClassicConfigurations::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bFocus = false;
	IndexSelect = 0;

	SlideVolume->OnSlide.AddDynamic(this, &UClassicConfigurations::OnSlideVolume);
	BtnUpdateGameList->OnClickTrigger.AddDynamic(this, &UClassicConfigurations::OnClickUpdate);
	BtnDeviceInfo->OnClickTrigger.AddDynamic(this, &UClassicConfigurations::OnClickDevice);
	BtnLicenseInfo->OnClickTrigger.AddDynamic(this, &UClassicConfigurations::OnClickLicense);

	for (TObjectIterator<UMainInterface> ObjectIterator; ObjectIterator; ++ObjectIterator)
	{
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
	ClassicMediaPlayerReference->ChangeMasterVolume(Value);
	//UE_LOG(LogTemp, Warning, TEXT("The slide value is: %d"), Value);
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
		UE_LOG(LogTemp, Warning, TEXT("Cannot Delete Save"));
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

void UClassicConfigurations::CloseModal()
{
	bFocus = false;
	WSButtons->SetVisibility(ESlateVisibility::Visible);
	WSDeviceInfo->SetVisibility(ESlateVisibility::Hidden);
	WSDeviceLicense->SetVisibility(ESlateVisibility::Hidden);

}

void UClassicConfigurations::SetIndexFocus(EButtonsGame Input)
{
	//UE_LOG(LogTemp, Warning, TEXT("bDelayInput %s"), (bDelayInput)? TEXT("true") : TEXT("false"));
	if (!bFocus && !bDelayInput)
	{

		if (Input == EButtonsGame::UP || Input == EButtonsGame::DOWN)
		{
			bDelayInput = true;
			if (Input == EButtonsGame::DOWN)
			{
				IndexSelect = FMath::Clamp(IndexSelect + 1, 0, 3);
			}
			else if(Input == EButtonsGame::UP)
			{
				IndexSelect = FMath::Clamp(IndexSelect - 1, 0, 3);
			}
			SetFocusSelect();	
			GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UClassicConfigurations::Delay, 0.18f, false, -1);
		}

		const int32 SlideValue = SlideVolume->SlideValue;

		UE_LOG(LogTemp, Warning, TEXT("slidevalue %d"), SlideValue);

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
	//MainInterfaceReference->RestartWidget();
	//MainInterfaceReference->CloseBackMenu();
	//CloseModal();
}

