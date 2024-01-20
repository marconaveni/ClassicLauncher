// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "OptionsLayout.generated.h"


class UScrollBoxEnhanced;
class UButtonCommon;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UOptionsLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UModal* Modal;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UMainScreen* MainInterfaceReference;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxEnhanced* ScrollBoxOptions;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxEnhanced* ScrollBoxThemes;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxEnhanced* ScrollBoxPrompt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxEnhanced* ScrollBoxLanguage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* OptionsContent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* PromptContent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* LanguageContent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonSlide* SlideVolumeSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonSlide* SlideVolumeMusic;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonSlide* SlideVolumeVideo;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnChangeTheme;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnPrompt;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnDeviceInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnLicenseInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnLanguage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnEnglish;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnPortuguese;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnPromptTypeOne;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnPromptTypeTwo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonCommon* BtnPromptTypeThree;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonCheckBox* CbVideoAudio;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class ULabel* LabelAppearance;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class ULabel* LabelAudio;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class ULabel* LabelOptions;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class ULabel* LabelSystem;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonCommonAlternative* BtnUpdateGameList;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButtonCommonAlternative* BtnChangeSystems;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WSScreens;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOptionsDeviceLayout* OptionsDeviceLayout;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOptionsLicenceLayout* OptionsLicenceLayout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class AClassicMediaPlayer* ClassicMediaPlayerReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FText> TitleList;

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnSlideVolumeMaster(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnSlideVolumeMusic(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnSlideVolumeVideo(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnSlideLostFocus();

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickUpdate(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickChangeSystems(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickDevice(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickLicense(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnSoundVideo(bool bCheck);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickChangeLanguage(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickLanguage(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickPrompt(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void OnClickChangeTheme(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void GetLanguageText(bool bShowMessage);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void CloseModal();

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void SetFocusOptionsItem(EButtonsGame Input);

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Events")
	void RestartMap();

protected:


	
	UFUNCTION()
	bool GetScrollBoxEnhancedWidgetSwitcherIndex(UScrollBoxEnhanced*& Scroll) const;

	UFUNCTION()
	void SetActiveWidgetIndex(int32 Index);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicConfigurations|Timers")
	FTimerHandle RestartMapTimerHandle;
};
