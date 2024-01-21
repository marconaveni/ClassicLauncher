// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/GameData.h"
#include "ScreenManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageDelegate, FText, Message);

/**
 * 
 */
UCLASS(BlueprintType)
class CLASSICLAUNCHER_API UScreenManager : public UObject
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FMessageDelegate MessageShow;

	UPROPERTY()
	class UDataManager* DataManager;

	/*UPROPERTY()
	TSubclassOf<class UMainScreen> MainInterfaceClass;

	UPROPERTY()
	TSubclassOf<class ULoadingScreen> LoadingScreenClass;*/

	//UPROPERTY()
	//class UMainScreen* MainScreenReference;

	//UPROPERTY()
	//class ULoadingScreen* LoadingScreenReference;

	/*UPROPERTY()
	TArray<FGameData> GameData;*/

	/*UPROPERTY()
	TArray<FGameSystem>GameSystems;*/

	/*UPROPERTY()
	FConfig ConfigurationData;*/

	/*UPROPERTY()
	APlayerController* GameplayStatics;*/

	/*UPROPERTY()
	class UClassicGameInstance* ClassicGameInstance;*/

	/*UPROPERTY()
	int32 IndexGameSystem;*/

	UPROPERTY()
	FTimerHandle DelayTimerHandle;



public:

	void Init(TSubclassOf<class UMainScreen> MainInterface, TSubclassOf<class ULoadingScreen> LoadingScreen);

	/*UFUNCTION()
	void CreateWidgets();*/

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void LoadConfiguration();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void CreateNewGameList();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void PrepareToSaveNewGameList();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void LoadGameSystems();

	UFUNCTION()
	void AddSystems();

	UFUNCTION()
	void SetMainInterfaceData() const;

	UFUNCTION()
	void AddMainInterfaceToViewPort();

	UFUNCTION()
	void AddLoadingScreenToViewPort();

	UFUNCTION()
	void RemoveLoadingScreenToParent();

	UFUNCTION()
	void SetToRestartWidgets();

	UFUNCTION()
	void RestartWidgets();

	UFUNCTION()
	void Message(FText Message);
};