// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/GameData.h"
#include "ClassicGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	virtual void Init() override;

	UPROPERTY()
	class UClassicSaveGame* ClassicSaveGameInstance;

	UPROPERTY()
	FString SlotGame;

	UClassicGameInstance();

public:

	UFUNCTION(BlueprintPure, Category = "ClassicGameInstance|SaveGame|Functions")
	TArray<FGameSystem> GetGameSystemSave() const;
	
	UFUNCTION(BlueprintCallable, Category = "ClassicGameInstance|SaveGame|Functions")
	bool SaveGameSystem(const TArray<FGameSystem>& GameSystem, int32 Slot = 0);

	UFUNCTION(BlueprintCallable, Category = "ClassicGameInstance|SaveGame|Functions")
	bool DeleteGameSystemSave(int32 Slot = 0);
	
	UFUNCTION(Exec)
	void UpdateTheme();
};
