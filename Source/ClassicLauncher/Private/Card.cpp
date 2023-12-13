// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Card.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

UCard::UCard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PathImage(TEXT(""))
	, MapIndex(0)
	, bFocus(false)
{
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

void UCard::SetPlayers(FString NumberPlayers)
{
	if (PlayersImage.Max() > 3) {
		if (NumberPlayers == TEXT("1") || NumberPlayers == TEXT("")) {
			PlayerIcon->SetBrush(PlayersImage[0]);
		}
		else if (NumberPlayers == TEXT("2") || NumberPlayers == TEXT("1-2")) {
			PlayerIcon->SetBrush(PlayersImage[1]);
		}
		else if (NumberPlayers == TEXT("3") || NumberPlayers == TEXT("1-3") || NumberPlayers == TEXT("2-3")) {
			PlayerIcon->SetBrush(PlayersImage[2]);
		}
		else if (NumberPlayers == TEXT("4") || NumberPlayers == TEXT("1-4") || NumberPlayers == TEXT("2-4") || NumberPlayers == TEXT("3-4")) {
			PlayerIcon->SetBrush(PlayersImage[3]);
		}
		else {
			PlayerIcon->SetBrush(PlayersImage[4]);
		}
	}
}

void UCard::SetFocusCard(bool bEnable, bool bAnimate, bool bReset, float TimeAnimation)
{
	bFocus = bEnable;
	const float Time = TimeAnimation;
	const float Opacity = (bEnable) ? 1 : 0;

	AnimationFrame->ClearAnimation();
	AnimationCard->ClearAnimation();

	if (bAnimate)
	{
		const FWidgetTransform Transform;
		AnimationFrame->PlayAnimation(FrameSelected, Time, Transform, Opacity, bReset, EEasingFunc::EaseOut, true);
		AnimationCard->PlayAnimation(BackgroundSelected, Time, Transform, Opacity, bReset, EEasingFunc::EaseOut, true);
	}
	else
	{
		FrameSelected->SetRenderOpacity(Opacity);
		BackgroundSelected->SetRenderOpacity(Opacity);
	}
}

void UCard::SetThemeCard(UTexture2D* texture)
{
}

void UCard::SetFavorite(bool bFavorite, bool bAnimateIcon)
{
	if (bFavorite == false && Favorite->GetVisibility() == ESlateVisibility::Hidden) return;

	Favorite->SetVisibility(ESlateVisibility::Visible);

	if (bFavorite)
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

	if (bAnimateIcon == false)
	{
		Favorite->SetRenderOpacity((bFavorite) ? 1.0f : 0.0f);
	}
	else
	{

		const FWidgetTransform Transform;
		if (bFavorite)
		{
			AnimationFavorite->SetCurves(CurveFavoritesFoward);
			AnimationFavorite->PlayAnimation(Favorite, 0.45f, Transform, 1, false, EEasingFunc::EaseOut);
		}
		else
		{
			AnimationFavorite->SetCurves(CurveFavoritesReverse);
			AnimationFavorite->PlayAnimation(Favorite, 0.45f, Transform, 0, false, EEasingFunc::EaseOut);
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

bool UCard::HasFocusCard() const
{
	return bFocus;
}

