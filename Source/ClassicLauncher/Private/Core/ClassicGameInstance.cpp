// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Core/ClassicGameInstance.h"
#include "Core/ClassicSaveGame.h"
#include "UI/Screens/MainScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

UClassicGameInstance::UClassicGameInstance()
	: ClassicSaveGameInstance(nullptr)
	  , SlotGame(TEXT("save_c2"))
{}

void UClassicGameInstance::Init()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotGame, 0))
	{
		ClassicSaveGameInstance = Cast<UClassicSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotGame, 0));
	}
	else
	{
		ClassicSaveGameInstance = Cast<UClassicSaveGame>(UGameplayStatics::CreateSaveGameObject(UClassicSaveGame::StaticClass()));
	}
	Super::Init();
}

TArray<FGameSystem> UClassicGameInstance::GetGameSystemSave() const
{
	return ClassicSaveGameInstance->GameSystemsSave;
}

bool UClassicGameInstance::SaveGameSystem(const TArray<FGameSystem>& GameSystem, const int32 Slot)
{
	ClassicSaveGameInstance->GameSystemsSave = GameSystem;
	return UGameplayStatics::SaveGameToSlot(ClassicSaveGameInstance, SlotGame, Slot);
}

bool UClassicGameInstance::DeleteGameSystemSave(const int32 Slot)
{
	ClassicSaveGameInstance->GameSystemsSave.Empty();
	return UGameplayStatics::DeleteGameInSlot(SlotGame, Slot);
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
