// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Card.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

UCard::UCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Path = TEXT("");
	IndexCard = 0;
}

void UCard::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCard::NativePreConstruct()
{
	Super::NativePreConstruct();
}

bool UCard::Initialize()
{
	bool Success = Super::Initialize();
	if (BtnClick)
	{
		BtnClick->OnClicked.AddDynamic(this, &UCard::ButtonClick);
	}
	return false;
}

FReply UCard::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UCard::SetPath(FString value)
{
	Path = value;		
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
	if (enable) {
		UUserWidget::PlayAnimationForward(ChangeColor);
		BtnClick->SetKeyboardFocus();
	}
	else {
		UUserWidget::PlayAnimationReverse(ChangeColor);
	}

}

void UCard::SetThemeCard(UTexture2D* texture)
{
}

void UCard::SetFavorite(bool favorite, bool AnimateIcon)
{
	if (AnimateIcon) {	
		Favorite->SetVisibility(ESlateVisibility::Visible);
		if (favorite) {
			UUserWidget::PlayAnimationForward(FadeFavorite);
		}
		else {
			UUserWidget::PlayAnimationReverse(FadeFavorite);
		}
	}
	else {
		if (favorite) {
			Favorite->SetVisibility(ESlateVisibility::Visible);
			UUserWidget::PlayAnimationForward(FadeFavorite);
		}
		else {
			Favorite->SetVisibility(ESlateVisibility::Hidden);
			UUserWidget::PlayAnimationReverse(FadeFavorite);
		}
	}
}

void UCard::LoadImageCard(UTexture2D* texture, int32 width, int32 height)
{
	Cover->SetDesiredSizeOverride(FVector2D(FMath::Clamp(width, 32, 342), FMath::Clamp(height,32, 306)));
	Cover->SetBrushFromTexture(texture);
}

void UCard::ButtonClick()
{
	UUserWidget::PlayAnimationForward(StartSystem,1.0f,true);
	ClickButton();
	OnClickTrigger.Broadcast(Path);
}
