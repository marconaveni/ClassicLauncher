// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicGameMode.h"
#include "Blueprint/UserWidget.h"
#include "MainInterface.h"
#include "ClassicConfigurations.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AClassicGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (MainInterfaceClass && ClassicConfigurationsClass) // Check if the Asset is assigned in the blueprint.
	{
		APlayerController* GameplayStatics = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		// Create the widget and store it.
		MainInterfaceReference = CreateWidget<UMainInterface>(GameplayStatics, MainInterfaceClass);
		ClassicConfigurationsReference = CreateWidget<UClassicConfigurations>(GameplayStatics, ClassicConfigurationsClass);

		if (MainInterfaceReference && ClassicConfigurationsReference)
		{
			//let add it to the view port
			MainInterfaceReference->AddToViewport(0);
			ClassicConfigurationsReference->AddToViewport(1);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MainInterfaceReference or ClassicConfigurationsReference Not Found or Null"));
		}
	
		//Show the Cursor.
		GameplayStatics->bShowMouseCursor = false;

		//Input mode settings. 
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(true);

		//set input mode
		GameplayStatics->SetInputMode(InputMode);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainInterfaceClass or ClassicConfigurationsClass Not Found or Null"));
	}
}
