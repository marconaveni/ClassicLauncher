// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY()
	FTimerHandle DelayTimerHandle;



public:

	void Init(TSubclassOf<class UMainScreen> MainScreenClass, TSubclassOf<class ULoadingScreen> LoadingScreenClass);

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
