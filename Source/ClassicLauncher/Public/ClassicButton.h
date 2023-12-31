// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateFocusBT, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateFocusLostBT, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateClickBT, int32, Index);
/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicButton : public UUserWidget
{
	GENERATED_BODY()

public:

	UClassicButton(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FocusButton;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusBT OnFocusTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusLostBT OnFocusLostTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateClickBT OnClickTrigger;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BtButton;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	int32 Index;

	UFUNCTION(BlueprintCallable, Category = "ClassicButton|Functions")
	void ButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ClassicButton|Functions")
	bool HasFocusButton();

	UFUNCTION(BlueprintCallable, Category = "Themes")
	void SetTheme(UTexture2D* TextureIcon, FSlateBrush BackgroundColor);

	UFUNCTION()
	void EnableButton(const bool bEnable);
	
private:

	UFUNCTION()
	void SetFocusButton(bool bEnable);

	UPROPERTY()
	bool bFocus;

};
