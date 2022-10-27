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

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBoxSystems;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	float ScrollCurrentOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	EButtonsGame Input;

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void SetScrollArrowIcons(const EButtonsGame Navigate);

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void OnUserScrolled(float CurrentOffset);

};
