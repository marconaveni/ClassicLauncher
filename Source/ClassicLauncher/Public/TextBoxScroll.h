// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBoxScroll.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UTextBoxScroll : public UUserWidget
{
	GENERATED_BODY()

public:

	UTextBoxScroll(const FObjectInitializer& ObjectInitializer);
	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Classic Launcher|Components")
	class UScrollBox* Scroll;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Classic Launcher|Components")
	class UMultiLineEditableTextBox* Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Timer")
	FTimerHandle TickTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Timer")
	FTimerHandle RestartTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Values")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Values")
	float SpeedScroll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Values")
	float ScrollOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Values")
	float DelayWaitRestart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Values")
	bool bAutoScroll;

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void StartScroll(float DelayStart = 2.5f);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void ScrollTick();

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void RestartScroll();

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void CancelAutoScroll();
	
	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextString(FString NewText);
};
