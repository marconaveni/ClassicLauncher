// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextImageBlock.h"
#include "ButtonsPrompt.generated.h"

class UImage;
class UTextImageBlock;
class USpacer;

/**
 *
 */
USTRUCT(BlueprintType)
struct FButtonsIcons
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility EVisibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ButtonTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BgButtonTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTextStyle TextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DefaultToImageText;

	UPROPERTY()
	UImage* IconReference;
	UPROPERTY()
	UImage* IconBackgroundReference;
	UPROPERTY()
	UTextImageBlock* TextImageBlockReference;

	//construtor
	FButtonsIcons()
	{
		EVisibility = ESlateVisibility::Visible;
		ButtonText = FText::FromString(TEXT(""));
		DefaultToImageText = false;
	}
};

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UButtonsPrompt : public UUserWidget
{
	GENERATED_BODY()

public:


	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* IconB1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* IconB2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* IconB3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* IconB4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* IconB5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* IconB6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextBlock1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextBlock2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextBlock3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextBlock4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextBlock5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextBlock6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USpacer* SpacerLeft;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USpacer* SpacerRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	TArray<FButtonsIcons> ButtonsIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	FSlateColor TextColor;

	/** alignment text  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Default Values")
	TEnumAsByte<ETextJustify::Type>Justification = ETextJustify::Center;

	/**
	*  determines the alignment bar icons
	*  @param Justify Bar alignment 
	*/
	UFUNCTION(BlueprintCallable, Category = "TextImageBlock|Functions")
	void SetJustification(ETextJustify::Type Justify);

	UFUNCTION()
	void SetComponents(TArray<FButtonsIcons>& ButtonsIcon);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetButtonsIcons(TArray<FButtonsIcons> ButtonsIcon);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetIconsStyle(TArray<UTexture2D*> BgTextures , TArray<UTexture2D*> Textures);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetTexts(TArray<FText> ButtonsText);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetColorsText(FSlateColor NewColor);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetTextDefaultStyle(FTextStyle TextStyle);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetButtonsVisibility(TArray<ESlateVisibility> EVisibility);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void AlternateToTextImage(bool bEnable,float Size = 24);
};
