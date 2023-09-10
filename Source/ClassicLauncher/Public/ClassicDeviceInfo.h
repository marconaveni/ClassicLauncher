// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicDeviceInfo.generated.h"


class UTextImageBlock;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicDeviceInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextAppName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextCpuCore;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextCpuName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextDriver;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Classic Launcher|Components")
	UTextImageBlock* TextGpuName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextGpuBrand;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextMemory;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextStorage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextSystemOS;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TextVersion;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtAppInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtSystem;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtVideo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtAppName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtCpuCore;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtCpuName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtDriver;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtGpuBrand;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtGpuName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtMemory;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtStorage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtSystemOS;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget) , Category="Classic Launcher|Components")
	UTextImageBlock* TxtVersion;
};
