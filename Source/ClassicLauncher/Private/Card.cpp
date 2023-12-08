// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Card.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

UCard::UCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PathImage = TEXT("");
	MapIndex = 0;
}

void UCard::NativePreConstruct()
{
	Super::NativePreConstruct();
}

bool UCard::Initialize()
{
	AnimationFrame = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());
	AnimationCard = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());
	AnimationFadeCard = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());
	AnimationFavorite = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());

	
	bool Success = Super::Initialize();
	return false;
}

FReply UCard::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UCard::SetPath(FString value)
{
	PathImage = value;
}

void UCard::SetPlayers(FString value)
{
	if (playersImage.Max() > 3) {
		if (value == TEXT("1") || value == TEXT("")) {
			PlayerIcon->SetBrush(playersImage[0]);
		}
		else if (value == TEXT("2") || value == TEXT("1-2")) {
			PlayerIcon->SetBrush(playersImage[1]);
		}
		else if (value == TEXT("3") || value == TEXT("1-3") || value == TEXT("2-3")) {
			PlayerIcon->SetBrush(playersImage[2]);
		}
		else if (value == TEXT("4") || value == TEXT("1-4") || value == TEXT("2-4") || value == TEXT("3-4")) {
			PlayerIcon->SetBrush(playersImage[3]);
		}
		else {
			PlayerIcon->SetBrush(playersImage[4]);
		}
	}
}

void UCard::SetFocusCard(bool bEnable)
{
	const FWidgetTransform Transform;
	if (bEnable)
	{
		AnimationFrame->PlayAnimation(this->FrameSelected, 0.12f, Transform, 1, false, EEasingFunc::EaseOut);
		AnimationCard->PlayAnimation(this->BackgroundSelected, 0.12f, Transform, 1, false, EEasingFunc::EaseOut);
	}
	else
	{
		AnimationFrame->PlayAnimation(this->FrameSelected, 0.12f, Transform, 0, false, EEasingFunc::EaseOut);
		AnimationCard->PlayAnimation(this->BackgroundSelected, 0.12f, Transform, 0, false, EEasingFunc::EaseOut);
	}
}

void UCard::SelectedFrameToBackground()
{
}

void UCard::SetThemeCard(UTexture2D* texture)
{
}

void UCard::SetFavorite(bool favorite, bool AnimateIcon)
{
	if (favorite == false && Favorite->GetVisibility() == ESlateVisibility::Hidden) return;

	Favorite->SetVisibility(ESlateVisibility::Visible);

	if (favorite)
	{
		FrameMain->SetRenderOpacity(0.0f);
		BackgroundMain->SetRenderOpacity(0.0f);
		FrameFavorite->SetRenderOpacity(1.0f);
		BackgroundFavorite->SetRenderOpacity(1.0f);
	}
	else
	{
		FrameMain->SetRenderOpacity(1.0f);
		BackgroundMain->SetRenderOpacity(1.0f);
		FrameFavorite->SetRenderOpacity(0.0f);
		BackgroundFavorite->SetRenderOpacity(0.0f);
	}

	if (AnimateIcon == false)
	{
		Favorite->SetRenderOpacity((favorite) ? 1.0f : 0.0f);
	}
	else
	{

		FWidgetTransform Transform;
		if (favorite)
		{
			AnimationFavorite->SetCurves(CurveFavoritesFoward);
			AnimationFavorite->PlayAnimation(this->Favorite, 0.45f, Transform, 1, false, EEasingFunc::EaseOut);
		}
		else
		{
			AnimationFavorite->SetCurves(CurveFavoritesReverse);
			AnimationFavorite->PlayAnimation(this->Favorite, 0.45f, Transform, 0, false, EEasingFunc::EaseOut);
			UE_LOG(LogTemp, Warning, TEXT("aqui"));
		}
	}
}

void UCard::SetCardImage(UTexture2D* texture, int32 width, int32 height)
{
	if (texture != nullptr)
	{
		FSlateBrush NewBrush;
		NewBrush.SetImageSize(FVector2D(width * 2, height * 2));
		NewBrush.SetResourceObject(texture);
		Cover->SetBrush(NewBrush);
	}
}

void UCard::ButtonClick()
{
	ClickButton();
	OnClickTrigger.Broadcast(PathImage);
}

void UCard::AnimationFade()
{
	if (AnimationFavorite != nullptr)
	{
		FWidgetTransform Transform;
		Transform.Scale = FVector2D(1.8f, 1.8f);
		AnimationFadeCard->PlayAnimation(this, 0.25f, Transform, 0, true, EEasingFunc::EaseOut);
	}
}

bool UCard::GetPositionCover(const int32 Left, const int32 Right)
{
	return false;
}
