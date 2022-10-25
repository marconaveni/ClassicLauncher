// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicButtonsIcons.generated.h"


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


	//construtor
	FButtonsIcons()
	{
		ButtonText = FText::FromString(TEXT(""));
	}
};



/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicButtonsIcons : public UUserWidget
{
	GENERATED_BODY()

public:

	UClassicButtonsIcons(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* IconB1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* IconB2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* IconB3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* IconB4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* IconB5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* IconB6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	TArray<FButtonsIcons> ButtonsIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButton|Variables")
	FSlateColor TextColor;

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetButtons(TArray<FButtonsIcons> ButtonsIcon);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetButtonsStyle(TArray<UTexture2D*> BgTextures , TArray<UTexture2D*> Textures);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetButtonsText(TArray<FText> ButtonsText);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetColorsText(FSlateColor newColor);

	UFUNCTION(BlueprintCallable, Category = "ClassicButton")
	void SetButtonsVisibility(TArray<ESlateVisibility> EVisibility);
};
