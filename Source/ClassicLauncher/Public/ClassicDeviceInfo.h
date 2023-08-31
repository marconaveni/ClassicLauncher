// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicDeviceInfo.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicDeviceInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextDeviceInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextAppInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextCPU;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextCpuCore;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextGpuDriver;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextGpuName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextSystemOS;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextAppName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextAppVersion;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtCPU;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtCpuCore;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtGpuDriver;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtGpuName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtSystemOS;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtAppName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtAppVersion;
};
