// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Core/ClassicGameInstance.h"
#include "Core/ClassicSaveGame.h"
#include "UI/Screens/MainScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

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


void UClassicGameInstance::SetSystemSave(TArray<FGameSystem> Systems)
{
	ClassicSaveGameInstance->GameSystemsSave = Systems;
}

void UClassicGameInstance::UpdateTheme()
{
	UE_LOG(LogTemp, Warning, TEXT("ForceUpdateTheme"));

	for (TObjectIterator<UMainScreen> ObjectIterator; ObjectIterator; ++ObjectIterator)
	{
		if (ObjectIterator->GetWorld() != GetWorld())
		{
			continue;
		}
		
		FOutputDeviceNull OutputDeviceNull;
		ObjectIterator->CallFunctionByNameWithArguments(TEXT("SetTheme"), OutputDeviceNull, nullptr, true);
		break;
	}


}