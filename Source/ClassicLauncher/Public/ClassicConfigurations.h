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

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UMainInterface* MainInterfaceReference;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ShowConfigurations;

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

	UFUNCTION(BlueprintCallable, Category = "ClassicConfigurations|Functions")
	void ShowConfiguration();
};
