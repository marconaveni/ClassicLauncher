// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Card.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Components/CanvasPanelSlot.h"

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

void UCard::SetFocusCard(bool enable)
{
	if (enable) 
	{
		PlayAnimationForward(ChangeColor);
	}
	else 
	{
		PlayAnimationReverse(ChangeColor);
	}
	UCanvasPanelSlot* CanvasFrameSelected = Cast<UCanvasPanelSlot>(FrameSelected->Slot);
	UCanvasPanelSlot* CanvasBackgroundSelected = Cast<UCanvasPanelSlot>(BackgroundSelected->Slot);
	CanvasFrameSelected->SetZOrder(2);
	CanvasBackgroundSelected->SetZOrder(-1);
}

void UCard::SelectedFrameToBackground()
{
	UCanvasPanelSlot* CanvasFrameSelected = Cast<UCanvasPanelSlot>(FrameSelected->Slot);
	UCanvasPanelSlot* CanvasBackgroundSelected = Cast<UCanvasPanelSlot>(BackgroundSelected->Slot);
	CanvasFrameSelected->SetZOrder(-99);
	CanvasBackgroundSelected->SetZOrder(-99);
}

void UCard::SetThemeCard(UTexture2D* texture)
{
}

void UCard::SetFavorite(bool favorite, bool AnimateIcon)
{
	if (favorite == false && Favorite->GetVisibility() == ESlateVisibility::Hidden) return;

	Favorite->SetVisibility(ESlateVisibility::Visible);

	if (AnimateIcon == false)
	{
		if (favorite) {
			FrameMain->RenderOpacity = 0.0f;
			BackgroundMain->RenderOpacity = 0.0f;
			FrameFavorite->RenderOpacity = 1.0f;
			BackgroundFavorite->RenderOpacity = 1.0f;
			Favorite->RenderOpacity = 1.0f;
		}
		else {
			FrameMain->RenderOpacity = 1.0f;
			BackgroundMain->RenderOpacity = 1.0f;
			FrameFavorite->RenderOpacity = 0.0f;
			BackgroundFavorite->RenderOpacity = 0.0f;
			Favorite->RenderOpacity = 0.0f;
		}
	}
	else {

		if (favorite) {
			PlayAnimationForward(FadeFavorite);
		}
		else {
			PlayAnimationReverse(FadeFavorite);
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
	if (GetPositionCover())
	{
		PlayAnimationForward(StartSystem, 1.0f, true);
	}
}

bool UCard::GetPositionCover()
{
	const FGeometry& Geometry = GetCachedGeometry();
	FVector2D PixelPosition, ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(this, Geometry, FVector2D(0, 0), PixelPosition, ViewportPosition);
	if (PixelPosition.X > 90 && PixelPosition.X < 1800)
	{
		return true;
	}
	return false;
}
