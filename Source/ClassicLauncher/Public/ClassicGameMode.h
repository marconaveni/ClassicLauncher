// Copyright 2022 Marco Naveni. All Rights Reserved.

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

	UPROPERTY()
	class UMainInterface* MainInterfaceReference;
	UPROPERTY()
	class UClassicConfigurations* ClassicConfigurationsReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class UMainInterface> MainInterfaceClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class UClassicConfigurations> ClassicConfigurationsClass;

	virtual void BeginPlay() override;
};
