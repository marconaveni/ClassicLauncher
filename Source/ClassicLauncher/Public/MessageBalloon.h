// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageBalloon.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UMessageBalloon : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimationShowMessage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextMessage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Background;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MessageBalloon|Timers")
	FTimerHandle TimerHandle;

public:

	UFUNCTION(BlueprintCallable, Category = "MessageBalloon|Functions")
	void ShowMessage(FString Message, float InRate);

private:

	UFUNCTION()
	void EndMessage();
};
