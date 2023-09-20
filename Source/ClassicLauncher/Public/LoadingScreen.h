// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMessageDelegate);

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FMessageDelegate MessageShow;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Message;

	UFUNCTION()
	void ShowMessage(FText NewMessage) const;
};
