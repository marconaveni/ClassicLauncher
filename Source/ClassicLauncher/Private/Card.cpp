// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Card.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

UCard::UCard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PathImage(TEXT(""))
	, MapIndexGameData(0)
	, IndexCard(0)
	, Favorite(nullptr)
	, bFocus(false)
{
}

void UCard::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UCard::NativeConstruct()
{
	Super::NativeConstruct();
	BindButton();
}

bool UCard::Initialize()
{
	const bool Success = Super::Initialize();
	//if (!Success)  return false;
	
	AnimationFrame = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());
	AnimationCard = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());
	AnimationFadeCard = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());
	AnimationFavorite = NewObject<UAnimationUI>(this, UAnimationUI::StaticClass());
	
	return Success;
}

FReply UCard::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UCard::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float CursorDelta = FMath::Abs(InMouseEvent.GetCursorDelta().X + InMouseEvent.GetCursorDelta().Y);

	if (BtnClick->IsHovered() && CursorDelta > 0 && !bMouseMove)
	{
		bMouseMove = true;
		Hovered();
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UCard::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UCard::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bMouseMove = false;
	Super::NativeOnMouseLeave(InMouseEvent);
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

bool UCard::BindButton()
{
	if (!ensure(BtnClick != nullptr)) return false;
	BtnClick->OnReleased.AddUniqueDynamic(this, &UCard::Release);
	BtnClick->OnUnhovered.AddUniqueDynamic(this, &UCard::Unhovered);
	return true;
}

void UCard::SetThemeCard(UTexture2D* texture)
{
}

void UCard::SetFavorite(bool bEnable, bool bAnimateIcon)
{

	FrameMain->SetRenderOpacity((bEnable) ? 0.0f : 1.0f);
	BackgroundMain->SetRenderOpacity((bEnable) ? 0.0f : 1.0f);
	FrameFavorite->SetRenderOpacity((bEnable) ? 1.0f : 0.0f);
	BackgroundFavorite->SetRenderOpacity((bEnable) ? 1.0f : 0.0f);
	const float ToOpacity = ((bEnable) ? 1.0f : 0.0f);
	if (bAnimateIcon == false)
	{
		Favorite->SetRenderOpacity(ToOpacity);
		Favorite->SetRenderScale(FVector2D(1.5f , 1.5f));
		return;
	}

	const FAnimationUICurves Curves = ((bEnable) ? CurveFavoritesFoward : CurveFavoritesReverse);
	const FWidgetTransform Transform;
	AnimationFavorite->SetCurves(Curves);
	AnimationFavorite->PlayAnimation(Favorite, 0.45f, Transform, ToOpacity, false, EEasingFunc::EaseOut);
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

void UCard::Release()
{
	OnReleaseTrigger.Broadcast(IndexCard);
}

void UCard::Hovered()
{
	OnHoveredTrigger.Broadcast(IndexCard);
}

void UCard::Unhovered()
{
	OnUnhoveredTrigger.Broadcast(IndexCard);
}

void UCard::AnimationFade()
{
	if (AnimationFavorite != nullptr)
	{
		FWidgetTransform Transform;
		Transform.Scale = FVector2D(1.8f, 1.8f);
		AnimationFadeCard->PlayAnimation(this, 0.25f, Transform, 0, false, EEasingFunc::EaseOut);
	}
	FTimerHandle FadeHandle;
	GetWorld()->GetTimerManager().SetTimer(FadeHandle, [&]()
	{
		this->SetRenderTransform(FWidgetTransform());
		this->SetRenderOpacity(1);
	}
	, 0.5f, false);
}

bool UCard::HasFocusCard() const
{
	return bFocus;
}

