// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/BaseButton.h"
#include "ButtonIcon.generated.h"


/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API UButtonIcon : public UBaseButton
{
	GENERATED_BODY()

public:

	UButtonIcon(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FocusButton;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgImage;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	FButtonStyle StyleButton;

	UFUNCTION(BlueprintCallable, Category = "Themes")
	void SetTheme(UTexture2D* TextureIcon, FSlateBrush BackgroundColor);

	UFUNCTION()
	void EnableButton(const bool bEnable);
	
private:
	
	virtual void SetFocusButton(bool bEnable) override;
	
};
