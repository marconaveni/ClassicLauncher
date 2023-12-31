// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MusicInterface.h"
#include "Blueprint/UserWidget.h"
#include "TextImageBlock.h"
#include "ClassicButtonSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusLost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateClick, int32, value);


/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicButtonSystem : public UUserWidget, public IMusicInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Click;
	
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocus OnFocusTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusLost OnFocusLostTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateClick OnClickTrigger;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UArrow* WBPArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* Text;
	
	UClassicButtonSystem(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetFocusButton(const bool bIsSound = true);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetCount(int32 NewValue);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void ButtonClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	int32 CountSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	class USoundBase* SoundSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	class USoundBase* SoundClick;

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void AlternateToTextImage(bool bEnable, float Size = 24);


private:

	UPROPERTY()
	bool Hover;

public:

	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;
};
