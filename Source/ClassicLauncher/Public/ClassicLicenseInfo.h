// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicLicenseInfo.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicLicenseInfo : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//class UTextBlock* Name;
	
};
