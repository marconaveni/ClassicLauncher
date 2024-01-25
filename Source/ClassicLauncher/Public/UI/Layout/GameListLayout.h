// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/GameData.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "UI/Screens/MainScreen.h"
#include "GameListLayout.generated.h"


class UButtonCommon;
class UScrollBoxEnhanced;
class UModal;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UGameListLayout : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UModal* Modal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<UButtonCommon> ButtonCommonClass;

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxEnhanced* ScrollBox;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void AddSystems(TArray<FGameSystem> GameSystem, UMainScreen* MainScreenRef);

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void SetFocusItem(const EButtonsGame Input);
};
