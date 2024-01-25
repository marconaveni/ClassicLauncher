// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClassicGameMode.generated.h"


class UMainScreen;
class ULoadingScreen;
class AClassicMediaPlayer;
class UScreenManager;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API AClassicGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	UScreenManager* ScreenManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<UMainScreen> MainInterfaceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<ULoadingScreen> LoadingScreenClass;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void Init();
	
};
