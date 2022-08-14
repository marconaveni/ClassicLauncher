// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicSystemListInterface.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicSystemListInterface : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBoxSystems;

	
	
};
