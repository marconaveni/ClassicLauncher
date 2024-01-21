// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "Subsystems/WorldSubsystem.h"
#include "DataManager.generated.h"


class UMainScreen;
class ULoadingScreen;
class ULoopScroll;
class UFrame;
class UClassicGameInstance;
class AClassicMediaPlayer;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UDataManager : public UWorldSubsystem
{
	GENERATED_BODY()

protected:

	UDataManager();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|Game")
	APlayerController* GameplayStatics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|Game")
	UClassicGameInstance* ClassicGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|Subclass")
	TSubclassOf<UMainScreen> MainScreenClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|Subclass")
	TSubclassOf<ULoadingScreen> LoadingScreenClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|References")
	UMainScreen* MainScreenReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|References")
	ULoadingScreen* LoadingScreenReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|References")
	ULoopScroll* LoopScrollReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|References")
	ULoopScroll* FrameReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|References")
	AClassicMediaPlayer* ClassicMediaPlayerReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|GameData")
	TArray<FGameData> GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|GameData")
	TArray<FGameData> GameDataIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|GameData")
	TArray<FGameSystem> GameSystems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|GameData")
	FConfig ConfigurationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|GameData")
	int32 IndexGameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|GameData")
	int32 IndexGameSystem;

	UFUNCTION(BlueprintCallable, Category = "DataManager|Functions")
	void CreateWidgets();
};
