// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicFunctionLibrary.h"
#include "ClassicSystemListInterface.generated.h"


class UClassicButtonSystem;
class UScrollBoxEnhanced;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicSystemListInterface : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowUP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowDown;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonsIcons* WBPButtonsIconsInterfaces;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	FTimerHandle ArrowTimerHandle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextSelectSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxEnhanced* ScrollBox;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	UTexture2D* ArrowIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicSystemListInterface|Variables")
	UTexture2D* ArrowIconOutline;

	UFUNCTION(BlueprintCallable, Category = "ClassicSystemListInterface|Events")
	void SetFocusItem(const EButtonsGame Input,UPARAM(ref) int32& Index);
};
