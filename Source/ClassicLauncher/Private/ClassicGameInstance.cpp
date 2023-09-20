// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicGameInstance.h"
#include "ClassicSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UClassicGameInstance::Init()
{

	if (UGameplayStatics::DoesSaveGameExist(SlotGame, 0)) 
	{
		ClassicSaveGameInstance = Cast<UClassicSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotGame, 0));
	}
	else 
	{
		ClassicSaveGameInstance = Cast<UClassicSaveGame>(UGameplayStatics::CreateSaveGameObject(UClassicSaveGame::StaticClass()) );
	}
	Super::Init();

}

UClassicGameInstance::UClassicGameInstance()
{
	SlotGame = TEXT("save_c1");
}

TArray<FGameSystem> UClassicGameInstance::GetSystemSave()
{
	return ClassicSaveGameInstance->GameSystemsSave;
}

void UClassicGameInstance::SetSystemSave(TArray<FGameSystem> Systems)
{
	ClassicSaveGameInstance->GameSystemsSave = Systems;
}
