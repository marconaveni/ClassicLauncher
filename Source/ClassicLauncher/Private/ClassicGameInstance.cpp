// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicGameInstance.h"
#include "ClassicSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UClassicGameInstance::Init()
{
	//if (GEngine) {
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Iniciou Game Instance");
	//}

	if (UGameplayStatics::DoesSaveGameExist(SlotGame, 0)) {
		ClassicSaveGameInstance = Cast<UClassicSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotGame, 0));
	}
	else {
		ClassicSaveGameInstance = Cast<UClassicSaveGame>(UGameplayStatics::CreateSaveGameObject(UClassicSaveGame::StaticClass()) );
	}
	Super::Init();

}

UClassicGameInstance::UClassicGameInstance()
{
	SlotGame = TEXT("save_c");
}
