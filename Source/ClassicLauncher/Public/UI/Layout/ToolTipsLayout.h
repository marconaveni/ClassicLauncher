// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TextImageBlock.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipsLayout.generated.h"


class UToolTip;
class UCanvasPanelSlot;
/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API UToolTipsLayout : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* ToolTipSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* ToolTipConfiguration;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* ToolTipFilter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* ToolTipMoreInformation;

	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipSystem;

	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipConfiguration;

	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipFilter;

	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipMoreInformation;

public:

	UFUNCTION(BlueprintCallable, Category = "Themes")
	void SetToolTipThemes(UTexture2D* TextureBackground, UTexture2D* TextureDetail, bool TextImage, float Size, FTextStyle NewTextStyle);

	void OnFocus(int32 Index) const;

	void OnLostFocus(int32 Index) const;
};
