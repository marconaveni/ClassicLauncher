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
	AClassicMediaPlayer* ClassicMediaPlayerReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataManager|GameData")
	TArray<FGameData> GameData;

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

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	UClassicGameInstance* GetClassicGameInstance() const;

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	AClassicMediaPlayer* GetClassicMediaPlayerReference() const;

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	ULoadingScreen* GetLoadingScreenReference() const;

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	UMainScreen* GetMainScreenReference() const;

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	ULoopScroll* GetLoopScrollReference() const;

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	UFrame* GetFrameReference() const;

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetClassicGameInstance(UPARAM(ref) UClassicGameInstance*& ClassicGameInstanceRef);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetClassicMediaPlayerReference(UPARAM(ref) AClassicMediaPlayer*& ClassicMediaPlayerRef);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetLoadingScreenReference(UPARAM(ref) ULoadingScreen*& LoadingScreenRef);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetMainScreenReference(UPARAM(ref) UMainScreen*& MainScreenRef);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetLoopScrollReference(UPARAM(ref) ULoopScroll*& LoopScrollRef);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetFrameReference(UPARAM(ref) UFrame*& FrameReferenceRef);
};
