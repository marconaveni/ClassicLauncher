// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MusicInterface.h"
#include "TextImageBlock.h"
#include "Blueprint/UserWidget.h"
#include "Header.generated.h"


class UToolTip;
class UImage;
class UClassicButton;
class UOverlay;
class UCanvasPanelSlot;
class UHorizontalBox;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateClickButton, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateFocusButton, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateLostFocusButton, int32, Index);

/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API UHeader : public UUserWidget, public IMusicInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateClickButton OnClickDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusButton OnFocusDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateLostFocusButton OnLostFocusDelegate;

protected:

	virtual void NativeOnInitialized() override;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Main;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MainOpacity;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* SelectSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* Configurations;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* Filter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* MoreInformation;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Buttons;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Images;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxButtons;

	UFUNCTION()
	void OnClick(int32 Index);

	UFUNCTION()
	void OnFocusClick(int32 Index);

	UFUNCTION()
	void OnLostFocusClick(int32 Index);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Variables")
	USoundBase* SoundNavigation;

	UFUNCTION()
	void SetFocusButton(const int32 Index = 0, const bool bIsSound = false);

	UFUNCTION()
	void HasButtonFocus();

	UFUNCTION()
	void EnableButtonsHeader(const int32 Index) const;

	UFUNCTION(BlueprintImplementableEvent)
	void AnimationFocus(const bool bFocus);

	UFUNCTION(BlueprintCallable, Category = "Themes")
	void SetThemeButton(UTexture2D* TextureMain, UTexture2D* TextureConfig, UTexture2D* TextureFilter, UTexture2D* TextureInformation, FSlateBrush BackgroundColor);

	UFUNCTION(BlueprintCallable, Category = "Themes")
	void SetThemeBackground(UTexture2D* TextureBackground, UTexture2D* TextureMain, UTexture2D* TextureMainOpacity);

	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;
};
