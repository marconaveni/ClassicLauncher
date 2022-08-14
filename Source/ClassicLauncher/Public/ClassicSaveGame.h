// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
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
	TArray<FConfigSystem> ConfigSystemsSave;

	UClassicSaveGame();
	
};
