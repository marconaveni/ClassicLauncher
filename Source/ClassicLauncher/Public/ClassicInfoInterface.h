// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData.h"
#include "TextImageBlock.h"
#include "ClassicFunctionLibrary.h"
#include "ClassicInfoInterface.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicInfoInterface : public UUserWidget
{
	GENERATED_BODY()


protected:

	UClassicInfoInterface(const FObjectInitializer& ObjectInitializer);

private:

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowUP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowDown;*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UStar* WBPStar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonsIcons* WBPButtonsIconsInterfaces;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextDeveloper;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextFavorite;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextGenre;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextLastPlayed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextPath;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextPathImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextPathThumbnail;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextPathVideos;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextPlayCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextPublisher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextRating;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextReleaseDate;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtDeveloper;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtFavorite;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtGenre;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtLastPlayed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtMoreInformation;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtPath;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtPlayCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtPlayers;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtPublisher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtRating;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtReleaseDate;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtThumbnail;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtTitle;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TxtVideo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfoInterface|Variables")
	float CurrentOffSet;

	/*UFUNCTION(BlueprintCallable, Category = "InfoInterface")
	void ScrollTopEnd(EButtonsGame Navigate);*/

	UFUNCTION(BlueprintCallable, Category = "InfoInterface")
	void SetGameInfo(FGameData GameData);

	UFUNCTION(BlueprintCallable, Category = "InfoInterface")
	void Scrolled(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle);

	UFUNCTION(BlueprintCallable, Category = "Classic Launcher|Functions")
	void AlternateToTextImage(bool bEnable, float Size = 24);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfoInterface|Variables")
	UTexture2D* ArrowIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfoInterface|Variables")
	UTexture2D* ArrowIconOutline;

	
};
