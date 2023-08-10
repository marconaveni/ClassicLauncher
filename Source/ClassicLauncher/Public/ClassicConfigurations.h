// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicConfigurations.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicConfigurations : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UMainInterface* MainInterfaceReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Variables")
	bool bFocus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Variables")
	bool bDelayInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Variables")
	int32 IndexSelect;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Background;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicSlide* SlideVolume;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonSystem* BtnUpdateGameList;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonSystem* BtnDeviceInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonSystem* BtnLicenseInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonSystem* BtnLanguage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WSButtons;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WSDeviceInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WSDeviceLicense;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DeviceInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DeviceLicense;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonsIcons* WBPButtonsIconsInterfaces;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextConfiguration;

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
	void GetLanguageText();
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void CloseModal();
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void SetIndexFocus(EButtonsGame Input);
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void SetFocusSelect();
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void Delay();
	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void RestartMap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Timers")
	FTimerHandle RestartMapTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Timers")
	FTimerHandle DelayTimerHandle;
};
