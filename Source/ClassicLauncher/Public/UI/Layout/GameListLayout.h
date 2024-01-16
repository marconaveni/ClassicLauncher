// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "GameListLayout.generated.h"


class UButtonCommon;
class UScrollBoxEnhanced;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UGameListLayout : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonsPrompt* WBPButtonsIconsInterfaces;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	FTimerHandle ArrowTimerHandle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextSelectSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxEnhanced* ScrollBox;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void SetFocusItem(const EButtonsGame Input,UPARAM(ref) int32& Index);
};
