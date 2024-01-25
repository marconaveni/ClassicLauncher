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
class UScreenManager;
class AClassicGameMode;

/*dynamic */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageDelegate,const FText&, Message);
/*dynamic */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoneGameListNative);


enum class EGameDataStatus { None ,Done ,Failed };
enum class EConfigurationStatus { None ,Done ,Failed };

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

private:

	UPROPERTY()
	UMainScreen* MainScreenReference;

	UPROPERTY()
	ULoadingScreen* LoadingScreenReference;

	UPROPERTY()
	AClassicMediaPlayer* ClassicMediaPlayerReference;
	
	UPROPERTY()
	UScreenManager* ScreenManager;

	EGameDataStatus GameDataStatus;
	
	EConfigurationStatus ConfigurationStatus;
	
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDoneGameListNative OnDoneGameListNative;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnMessageDelegate OnMessageShow;
	
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

	UFUNCTION(BlueprintPure, Category = "DataManager|GameData|Getters")
	FGameData GetGameData(int32 Index = -1) const;

	UFUNCTION(BlueprintPure, Category = "DataManager|GameData|Getters")
	FGameSystem GetGameSystem(int32 Index = -1) const;

	UFUNCTION(BlueprintPure, Category = "DataManager|GameData|Getters")
	FGameData GetGameSystemGameData(int32 Index = -1) const;

private:
	
	UFUNCTION()
	bool Save();

public:

	UFUNCTION(BlueprintCallable, Category = "DataManager|Save")
	void SetCountPlayerToSave();

	UFUNCTION(BlueprintCallable, Category = "DataManager|Save")
	void SetFavoriteToSave();
	
	UFUNCTION(BlueprintCallable, Category = "DataManager|GameData|Functions")
	void InitGameData();
	
	UFUNCTION(BlueprintCallable, Category = "DataManager|GameData|Functions")
	void LoadConfiguration();

	UFUNCTION(BlueprintCallable, Category = "DataManager|GameData|Functions")
	void LoadGamesList();

private:
	
	UFUNCTION()
	void LoadGameSystems();

	UFUNCTION()
	void CreateNewGameList();
	
	UFUNCTION()
	void PrepareToSaveNewGameList();
	
public:

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	UScreenManager* GetScreenManager() const;

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	AClassicGameMode* GetGameMode() const;

	UFUNCTION(BlueprintPure, Category = "DataManager|Getters")
	APlayerController* GetGameplayStatics() const;

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
	void SetClassicMediaPlayerReference(AClassicMediaPlayer* ClassicMediaPlayerRef);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetLoadingScreenReference(ULoadingScreen* LoadingScreenRef);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Setters")
	void SetMainScreenReference(UMainScreen* MainScreenRef);
	
};

