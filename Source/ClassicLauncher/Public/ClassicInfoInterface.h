// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData.h"
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

	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowUP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ArrowDown;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UStar* WBPStar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonsIcons* WBPButtonsIconsInterfaces;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextDeveloper;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextFavorite;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextGenre;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextLastPlayed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPath;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPathImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPathThumbnail;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPathVideos;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPlayCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPublisher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextRating;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextReleaseDate;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtDeveloper;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtFavorite;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtGenre;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtLastPlayed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtMoreInformation;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtPath;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtPlayCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtPlayers;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtPublisher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtRating;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtReleaseDate;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtThumbnail;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtTitle;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtVideo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfoInterface|Variables")
	float CurrentOffSet;

	UFUNCTION(BlueprintCallable, Category = "InfoInterface")
	void ScrollTopEnd(EButtonsGame Navigate);

	UFUNCTION(BlueprintCallable, Category = "InfoInterface")
	void SetGameInfo(FGameData GameData);

	UFUNCTION(BlueprintCallable, Category = "InfoInterface")
	void Scrolled(float NewValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfoInterface|Variables")
	UTexture2D* ArrowIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfoInterface|Variables")
	UTexture2D* ArrowIconOutline;

	
};
