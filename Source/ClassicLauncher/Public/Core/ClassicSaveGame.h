// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/GameData.h"
#include "GameFramework/SaveGame.h"
#include "ClassicSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
	TArray<FGameSystem> GameSystemsSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
	FGameData GameDataHistorySave;

	UClassicSaveGame();
	
};
