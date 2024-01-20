// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageBalloon.generated.h"


class UTextImageBlock;
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
	class UTextImageBlock* TextMessage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasMessage;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	FTimerHandle DelayTimerHandle;

	UPROPERTY()
	float InRate;

public:

	UFUNCTION(BlueprintCallable, Category = "MessageBalloon|Functions")
	void ShowMessage(FText Message, float Time = 5.0f);

	UFUNCTION()
	void CancelMessage();
	
private:
	UFUNCTION()
	void StartMessage();

	UFUNCTION()
	void EndMessage();
	
};
