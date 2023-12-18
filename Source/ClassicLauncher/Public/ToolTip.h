// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextImageBlock.h"
#include "ToolTip.generated.h"


class UOverlay;
/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API UToolTip : public UUserWidget
{
	GENERATED_BODY()

protected:

	UToolTip(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInFadeOutAnimation;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Overlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgImageTop;

	UPROPERTY()
	bool bFocus = false;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip|Variables")
	FName IndexName;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowToolTip(bool bShow);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip|Variables")
	int32 Index;

	UFUNCTION(BlueprintCallable, Category = "ToolTip|Functions")
	void SetFocusToolTip(bool bEnable);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolTip|Variables")
	FText Text;

	UFUNCTION(BlueprintPure, Category = "Classic Launcher|Functions")
	bool GetToolTipFocus();

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void AlternateToTextImage(bool bEnable, float Size = 24);

	UFUNCTION(BlueprintCallable, Category = "Themes")
	void SetToolTipThemes(UTexture2D* TextureBackground, UTexture2D* TextureDetail, bool TextImage, float Size, FTextStyle NewTextStyle);

};
