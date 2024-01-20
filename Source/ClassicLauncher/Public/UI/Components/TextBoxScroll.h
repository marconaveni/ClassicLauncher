// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextImageBlock.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
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
	class UTextImageBlock* DescriptionEnchanted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Timer")
	FTimerHandle TickTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Timer")
	FTimerHandle RestartTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Timer")
	FTimerHandle DelaySetText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Text Appearance")
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Text Appearance")
	FTextStyle TextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	float SpeedScroll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	float ScrollOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	float DelayWaitRestart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Launcher|Scroll Parameters")
	float DelayTextAppear;

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
	void SetText(FString NewText);

	UFUNCTION()
	void RenderText();

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void AlternateToTextImage(bool bEnable, float Size = 24);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetNewScroll(EButtonsGame Input, float NewScroll = 0.015f);

	UFUNCTION()
	void ScrollTick();


};
