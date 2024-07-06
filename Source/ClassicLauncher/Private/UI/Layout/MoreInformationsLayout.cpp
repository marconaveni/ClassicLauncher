// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/MoreInformationsLayout.h"
#include "UI/Components/StarRating.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"

UMoreInformationsLayout::UMoreInformationsLayout(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

FReply UMoreInformationsLayout::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UMoreInformationsLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);
}

FReply UMoreInformationsLayout::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

FString IsDateNull(FString value)
{
	return (value == TEXT("")) ? TEXT("Never") : UClassicFunctionLibrary::FormatDateTimeToView(value);
}

void UMoreInformationsLayout::SetGameInfo(FGameData GameData)
{
	TextPath->SetText(FText::FromString(GameData.PathFormatted.Replace(TEXT("\""),TEXT(""), ESearchCase::IgnoreCase)));
	TextName->SetText(FText::FromString(GameData.NameFormatted));
	TextRating->SetText(FText::FromString(GameData.Rating));
	TextReleaseDate->SetText(FText::FromString(GameData.ReleaseDate.FormatDateTime(false)));
	TextDeveloper->SetText(FText::FromString(GameData.Developer));
	TextPublisher->SetText(FText::FromString(GameData.Publisher));
	TextGenre->SetText(FText::FromString(GameData.Genre));
	TextPathImage->SetText(FText::FromString(GameData.ImageFormatted));
	TextPathThumbnail->SetText(FText::FromString(GameData.ScreenshotFormatted));
	TextPathVideos->SetText(FText::FromString(GameData.VideoFormatted));
	TextPlayers->SetText(FText::FromString(GameData.Players));
	TextFavorite->SetText(FText::FromString(GameData.bFavorite ? TEXT("Yes") : TEXT("No")));
	TextPlayCount->SetText(FText::FromString(FString::FromInt(GameData.PlayCount)));
	TextLastPlayed->SetText(FText::FromString(GameData.LastPlayed.FormatDateTime()));
	StarRating->SetUpdateRatingStar(FCString::Atof(*GameData.Rating));
}

void UMoreInformationsLayout::SetTextAppearance(FTextStyle NewTextStyle)
{
	for (TObjectIterator<UTextImageBlock> ObjectIterator; ObjectIterator; ++ObjectIterator)
	{
		if (ObjectIterator->GetWorld() != GetWorld())
		{
			continue;
		}
		ObjectIterator->SetTextStyle(NewTextStyle);
		UE_LOG(LogTemp, Warning, TEXT("Attemped text update %s"), *ObjectIterator->GetName());
	}
}

void UMoreInformationsLayout::AlternateToTextImage(bool bEnable, float Size)
{
	for (TObjectIterator<UTextImageBlock> ObjectIterator; ObjectIterator; ++ObjectIterator)
	{
		if (ObjectIterator->GetWorld() != GetWorld())
		{
			continue;
		}
		ObjectIterator->SetTextImageSize(Size);
		ObjectIterator->DefaultToImageText(bEnable, true);
		UE_LOG(LogTemp, Warning, TEXT("Attemped text update %s"), *ObjectIterator->GetName());
	}
}
