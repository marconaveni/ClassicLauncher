// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/GameData.h"
#include "TextImageBlock.h"
#include "MoreInformationsLayout.generated.h"


class UTextImageBlock;
class UModal;
class UStarRating;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UMoreInformationsLayout : public UUserWidget
{
	GENERATED_BODY()


protected:

	UMoreInformationsLayout(const FObjectInitializer& ObjectInitializer);

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UModal* Modal;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UStarRating* StarRating;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextDeveloper;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextFavorite;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextGenre;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextLastPlayed;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextPath;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextPathImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextPathThumbnail;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextPathVideos;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextPlayCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextPublisher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextRating;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextReleaseDate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtDeveloper;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtFavorite;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtGenre;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtLastPlayed;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtPath;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtPlayCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtPublisher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtRating;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtReleaseDate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtThumbnail;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TxtVideo;

	UFUNCTION(BlueprintCallable, Category = "MoreInformation")
	void SetGameInfo(FGameData GameData);
	
	UFUNCTION(BlueprintCallable, Category = "MoreInformation|Functions")
	void SetTextAppearance(FTextStyle NewTextStyle);

	UFUNCTION(BlueprintCallable, Category = "MoreInformation|Functions")
	void AlternateToTextImage(bool bEnable, float Size = 24);
	
	
};
