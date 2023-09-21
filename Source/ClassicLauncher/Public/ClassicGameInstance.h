// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData.h"
#include "ClassicGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame|Variables")
	class UClassicSaveGame* ClassicSaveGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame|Variables")
	FString SlotGame;

	UClassicGameInstance();


	UFUNCTION()
	void SetSystemSave(TArray<FGameSystem> Systems);
	
};
