// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScreenManager.generated.h"

class UDataManager;
class ULoadingScreen;
class UMainScreen;

/**
 * 
 */
UCLASS(BlueprintType)
class CLASSICLAUNCHER_API UScreenManager : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UDataManager* DataManager;

	UPROPERTY()
	FTimerHandle DelayTimerHandle;

public:

	void Init(TSubclassOf<UMainScreen> MainScreenClass, TSubclassOf<ULoadingScreen> LoadingScreenClass);

	UFUNCTION(BlueprintCallable, Category = "DataManager|Functions")
	void CreateWidgetsAndActors(TSubclassOf<UMainScreen> MainScreenClass, TSubclassOf<ULoadingScreen> LoadingScreenClass);

	UFUNCTION()
	void ShowMainScreenToViewPort();

	UFUNCTION()
	void SetVisibilityLoadingScreen(const ESlateVisibility Visibility) const;

	UFUNCTION()
	void SetToRestartWidgets();

	UFUNCTION()
	void RestartConfigurations();

	UFUNCTION()
	void ShowMessage(const FText& Message) const;
};
