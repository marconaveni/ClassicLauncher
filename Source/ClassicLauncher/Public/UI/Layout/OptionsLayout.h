// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "OptionsLayout.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UOptionsLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UMainScreen* MainInterfaceReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Variables")
	bool bFocus;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Variables")
	bool bDelayInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Variables")
	int32 Index;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBoxEnhanced* ScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VerticalBoxContent;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Background;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonSlide* SlideVolume;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonCommon* BtnUpdateGameList;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonCommon* BtnDeviceInfo;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonCommon* BtnLicenseInfo;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonCommon* BtnLanguage;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WSButtons;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WSDeviceInfo;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WSDeviceLicense;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonsPrompt* WBPButtonsIconsInterfaces;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextConfiguration;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOptionsDeviceLayout* WBPDeviceInfo;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOptionsLicenceLayout* WBPLicenseInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class AClassicMediaPlayer* ClassicMediaPlayerReference;

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnSlideVolume(int32 Value);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnSlideLostFocus();
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickUpdate(int32 Value);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickDevice(int32 Value);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickLicense(int32 Value);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickLanguage(int32 Value);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void GetLanguageText(bool bShowMessage);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void CloseModal();
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void SetFocusItem(EButtonsGame Input);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void SetFocusSelect(const bool bIsSound = true);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void Delay();
	
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void RestartMap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Timers")
	FTimerHandle RestartMapTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Timers")
	FTimerHandle DelayTimerHandle;
};
