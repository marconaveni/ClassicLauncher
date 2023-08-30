// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicFunctionLibrary.h"
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
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Classic Launcher|Components")
	class UScrollBox* Scroll;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Classic Launcher|Components")
	class UMultiLineEditableTextBox* Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Timer")
	FTimerHandle TickTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Timer")
	FTimerHandle RestartTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Text Appearance")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Text Appearance")
	FLinearColor TextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Text Appearance" , meta = (UIMin = 0, UIMax = 100))
	int32 TextSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	float SpeedScroll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	float ScrollOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	float DelayWaitRestart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	bool bAutoScroll;

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void StartScroll(float DelayStart = 2.5f);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void RestartScroll();

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void CancelAutoScroll();

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void PauseAutoScroll();
	
	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextString(FString NewText);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextAppearance(FLinearColor NewTextColor, int32 NewTextSize);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetNewScroll(EButtonsGame Input, float NewScroll = 0.015f);

	UFUNCTION()
	void ScrollTick();
};
