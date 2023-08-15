// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Card.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Styling/SlateBrush.h"

UCard::UCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Path = TEXT("");
	IndexCard = 0;
}


void UCard::NativePreConstruct()
{
	Super::NativePreConstruct();
}

bool UCard::Initialize()
{
	bool Success = Super::Initialize();
	//if (BtnClick)
	//{
	//	BtnClick->OnClicked.AddDynamic(this, &UCard::ButtonClick);
	//}
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
		//BtnClick->SetKeyboardFocus();
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
			UUserWidget::PlayAnimationForward(FadeFavorite);
		}
		else {
			UUserWidget::PlayAnimationReverse(FadeFavorite);
		}
	}
}

void UCard::LoadImageCard(UTexture2D* texture, int32 width, int32 height)
{
	//Cover->SetDesiredSizeOverride(FVector2D(FMath::Clamp(width, 32, 342), FMath::Clamp(height,32, 306)));
	FSlateBrush NewBrush;
	NewBrush.SetImageSize(FVector2D(width, height));
	NewBrush.SetResourceObject(texture);
	Cover->SetBrush(NewBrush);
}

void UCard::ButtonClick()
{	
	ClickButton();
	OnClickTrigger.Broadcast(Path);
}

void UCard::AnimationFade()
{
	UUserWidget::PlayAnimationForward(StartSystem, 1.0f, true);
}
