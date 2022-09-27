// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicConfigurations.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicConfigurations : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UMainInterface* MainInterfaceReference;
};
