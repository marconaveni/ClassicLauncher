// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolTip.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UToolTip : public UUserWidget
{
	GENERATED_BODY()

public:

	UToolTip(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimationSetToInvisibility;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimationSetToVisibility;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgImage;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip|Variables")
	FText Text;

	UFUNCTION(BlueprintCallable, Category = "ToolTip|Functions")
	void SetToolTipVisibility(ESlateVisibility Visible);



	
};
