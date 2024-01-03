// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MusicInterface.h"
#include "TextImageBlock.h"
#include "UI/BaseButton.h"
#include "ClassicButtonSystem.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicButtonSystem : public UBaseButton, public IMusicInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UArrow* WBPArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* Text;
	
	UClassicButtonSystem(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void SetFocusButton(bool bEnable) override;
	
	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetIndex(int32 NewIndex);

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

	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;
	
};
