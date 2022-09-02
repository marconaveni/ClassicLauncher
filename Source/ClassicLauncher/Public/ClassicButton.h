// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusBT);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusLostBT);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateClickBT);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	FButtonStyle StyleButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	class USoundBase* SoundSelect;

	UFUNCTION(BlueprintCallable, Category = "ClassicButton|Functions")
	void SetFocusButton(bool Focus);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton|Functions")
	void ButtonClick();

private:

	UPROPERTY()
	bool hover;
};
