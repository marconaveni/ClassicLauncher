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
	
protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UArrow* Arrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundFocus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Image;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* OverlayMain;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* Text;
	
	
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
	virtual void ButtonClick() override;
	
public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	USoundBase* SoundSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	USoundBase* SoundClick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	TEnumAsByte<ESlateBrushDrawType::Type> BackgroundDrawAs = ESlateBrushDrawType::RoundedBox;

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetBackgroundAppearance(
		float CornerRadius = 20,
		FString Color = TEXT("00baaf00"),
		float BorderWidth = 5,
		FString BorderColor = TEXT("00baaf"),
		float MarginLeft = 0,
		float MarginTop = 8,
		float MarginRight = 0,
		float MarginBottom = 8);
	
	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin = 90, float Size = 24);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetSize(float Width = 900, float Height = 108);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetArrow(UTexture2D* Texture0, UTexture2D* Texture1, UTexture2D* Texture2, float Angle, float Margin);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetIcon(UTexture2D* Texture, float Margin, bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetImage(UTexture2D* Texture, float Margin, bool bEnable);
	
	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;

private:

	UPROPERTY()
	bool bMouseFocus;
};
