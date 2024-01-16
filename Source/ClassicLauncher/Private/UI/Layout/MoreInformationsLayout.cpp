// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/MoreInformationsLayout.h"
#include "Components/Image.h"
#include "UI/Components/StarRating.h"
#include "Components/ScrollBox.h"

UMoreInformationsLayout::UMoreInformationsLayout(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	CurrentOffSet = 0;
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

/*void UClassicInfoInterface::ScrollTopEnd(EButtonsGame Navigate)
{
	if (Navigate == EButtonsGame::UP)
	{
		if (CurrentOffSet == 0)
		{
			ArrowUP->SetBrushFromTexture(ArrowIconOutline);
			ArrowDown->SetBrushFromTexture(ArrowIcon);
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("top"));
		}
	}
	else if(Navigate == EButtonsGame::DOWN)
	{
		if (CurrentOffSet >= ScrollBox->GetScrollOffsetOfEnd())
		{
			ArrowUP->SetBrushFromTexture(ArrowIcon);
			ArrowDown->SetBrushFromTexture(ArrowIconOutline);
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("down"));
		}
	}
}*/

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
	WBPStar->SetUpdateRatingStar(FCString::Atof(*GameData.rating));
}

void UMoreInformationsLayout::Scrolled(float NewValue)
{
	ScrollBox->SetScrollOffset(FMath::Clamp(NewValue, 0, ScrollBox->GetScrollOffsetOfEnd()) );
	CurrentOffSet = ScrollBox->GetScrollOffset();
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


