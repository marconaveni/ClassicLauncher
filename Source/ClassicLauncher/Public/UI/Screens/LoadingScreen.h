// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"



/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Message;

	UFUNCTION()
	void ShowMessage(const FText& NewMessage);
};
