// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/GameData.h"
#include "UI/Screens/MainScreen.h"
#include "Blueprint/UserWidget.h"
#include "FooterDetails.generated.h"

class UImage;
class UScaleBox;
class UTextBoxScroll;
class UCanvasPanel;
class UMainScreen;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UFooterDetails : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY()
	FGameData GameData;

	UPROPERTY()
	int32 IndexCard;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundVideo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScaleBox* ScaleBoxImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScaleBox* ScaleBoxVideo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Image;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Video;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBoxScroll* TextBoxScroll;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasFooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	int32 IndexFooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTexture2D* ImageDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	FTimerHandle StartVideoTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void Clear();
	
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void OpenFooter(const FGameData Data, const int32 Index, EPositionY& PositionY, AClassicMediaPlayer* ClassicMediaPlayerReference);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void CloseFooter(EPositionY& PositionY, AClassicMediaPlayer* ClassicMediaPlayerReference);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void ExpandVideo();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void SetImage();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void StartVideo(EPositionY PositionY, AClassicMediaPlayer* ClassicMediaPlayerReference);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void ImageOutThumb(UTexture2D* TextureOut, int32 Index, bool Sucesseful);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetPlayAnimation(FName AnimationName);

	UFUNCTION(BlueprintCallable, Category = "Themes")
	void SetTheme(UTexture2D* TextureBackground);
};
