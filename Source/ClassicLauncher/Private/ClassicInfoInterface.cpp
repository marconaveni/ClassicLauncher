// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicInfoInterface.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Star.h"
#include "Components/ScrollBox.h"

UClassicInfoInterface::UClassicInfoInterface(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	CurrentOffSet = 0;
}

FReply UClassicInfoInterface::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UClassicInfoInterface::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UClassicInfoInterface::ScrollTopEnd(EButtonsGame Navigate)
{
	if (Navigate == EButtonsGame::UP)
	{
		if (CurrentOffSet == 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("top"));
		}
	}
	else if(Navigate == EButtonsGame::DOWN)
	{
		if (CurrentOffSet >= ScrollBox->GetScrollOffsetOfEnd())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("down"));
		}
	}
}

FString IsDateNull(FString value)
{
	return (value == TEXT("")) ? TEXT("Never") : UClassicFunctionLibrary::FormatDateTimeToView(value);
}

void UClassicInfoInterface::SetGameInfo(FGameData GameData)
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
	WBPStar->SetUpdateRatingStar(FCString::Atof(*GameData.rating));
}

void UClassicInfoInterface::Scrolled(float NewValue)
{
	ScrollBox->SetScrollOffset(FMath::Clamp(NewValue, 0, ScrollBox->GetScrollOffsetOfEnd()) );
	CurrentOffSet = ScrollBox->GetScrollOffset();
}


