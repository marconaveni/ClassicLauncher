// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/MoreInformationsLayout.h"
#include "UI/Components/StarRating.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"

UMoreInformationsLayout::UMoreInformationsLayout(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
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
	TextPath->SetText(FText::FromString(GameData.PathFormated.Replace(TEXT("\""),TEXT(""), ESearchCase::IgnoreCase)));
	TextName->SetText(FText::FromString(GameData.nameFormated));
	TextRating->SetText(FText::FromString(GameData.rating));
	TextReleaseDate->SetText(FText::FromString(UClassicFunctionLibrary::FormatDateToView(GameData.releasedate)) );
	TextDeveloper->SetText(FText::FromString(GameData.developer));
	TextPublisher->SetText(FText::FromString(GameData.publisher));
	TextGenre->SetText(FText::FromString(GameData.genre));
	TextPathImage->SetText(FText::FromString(GameData.imageFormated));
	TextPathThumbnail->SetText(FText::FromString(GameData.thumbnailFormated));
	TextPathVideos->SetText(FText::FromString(GameData.videoFormated));
	TextPlayers->SetText(FText::FromString(GameData.players));
	TextFavorite->SetText(FText::FromString(GameData.favorite ? TEXT("Yes") : TEXT("No")) );
	TextPlayCount->SetText(FText::FromString(FString::FromInt(GameData.playcount)));
	TextLastPlayed->SetText(FText::FromString(IsDateNull(GameData.lastplayed)));
	StarRating->SetUpdateRatingStar(FCString::Atof(*GameData.rating));
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


