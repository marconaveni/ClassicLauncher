// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClassicGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API AClassicGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UScreenManager* LoadingGameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class UMainScreen> MainInterfaceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class ULoadingScreen> LoadingScreenClass;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void Init();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void GameSettingsInit();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void GameSettingsRunningInternal();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void GameSettingsRunning();
};