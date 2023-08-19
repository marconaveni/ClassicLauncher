// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicGameMode.h"
#include "Blueprint/UserWidget.h"
#include "MainInterface.h"
#include "ClassicConfigurations.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (MainInterfaceClass) // Check if the Asset is assigned in the blueprint.
	{
		APlayerController* GameplayStatics = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		// Create the widget and store it.
		MainInterfaceReference = CreateWidget<UMainInterface>(GameplayStatics, MainInterfaceClass);

		if (MainInterfaceReference /*&& ClassicConfigurationsReference*/)
		{
			//let add it to the view port
			MainInterfaceReference->AddToViewport(0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference Not Found or Null"));
		}
	
		//Show the Cursor.
		GameplayStatics->bShowMouseCursor = false;
		

		//Input mode settings. 
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);	

		//set input mode
		GameplayStatics->SetInputMode(InputMode);

		//Mouse focus mode 
		UGameplayStatics::SetViewportMouseCaptureMode(GameplayStatics, EMouseCaptureMode::CaptureDuringRightMouseDown);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceClass or ClassicConfigurationsClass Not Found or Null"));
	}
}
