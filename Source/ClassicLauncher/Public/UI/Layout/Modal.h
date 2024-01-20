// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Modal.generated.h"

class UImage;
class UTextImageBlock;
class UButtonsPrompt;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UModal : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* Title;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonsPrompt* Prompt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UNamedSlot* Content;

public:
	
	UPROPERTY(EditAnywhere, meta = (DisplayName ="Title"))
	FText TitleText;

	UPROPERTY(EditAnywhere, meta = (DisplayName ="Icon"))
	UTexture2D* IconTexture;

	UPROPERTY(EditAnywhere, meta = (DisplayName ="Background"))
	UTexture2D* BackgroundTexture;

	UFUNCTION(BlueprintCallable, Category = "Modal|Functions")
	void SetTitleText(FText NewTitle);

	UFUNCTION(BlueprintCallable, Category = "Modal|Functions")
	void SetIconTexture(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "Modal|Functions")
	void SetBackgroundTexture(UTexture2D* Texture);

	UFUNCTION(BlueprintPure, Category = "Modal|Functions")
	UButtonsPrompt* GetPrompt() const;

	UFUNCTION(BlueprintPure, Category = "Modal|Functions")
	UTextImageBlock* GetTitle() const;
	
};
