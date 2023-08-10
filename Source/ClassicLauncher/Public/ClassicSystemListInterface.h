// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicFunctionLibrary.h"
#include "ClassicSystemListInterface.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicSystemListInterface : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowUP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowDown;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonsIcons* WBPButtonsIconsInterfaces;

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextSelectSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBoxSystems;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	float ScrollCurrentOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	EButtonsGame Input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	UTexture2D* ArrowIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	UTexture2D* ArrowIconOutline;

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void SetScrollArrowIcons(const EButtonsGame Navigate);

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void OnUserScrolled(float CurrentOffset);

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void SetIconArrow();

};
