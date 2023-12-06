// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextImageBlock.h"
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
	class UWidgetAnimation* FadeInFadeOutAnimation;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgImageTop;

	UPROPERTY()
	bool bEnableVisibility = false;
	UPROPERTY()
	bool bFocus = false;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip|Variables")
	FText Text;

	UFUNCTION(BlueprintCallable, Category = "ToolTip|Functions")
	void SetToolTipVisibility(ESlateVisibility Visible);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void AlternateToTextImage(bool bEnable, float Size = 24);

	
};
