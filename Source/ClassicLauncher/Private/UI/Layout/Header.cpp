// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "UI/Layout/Header.h"
#include "ClassicButton.h"
#include "ToolTip.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UHeader::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SelectSystem->OnClickTrigger.AddDynamic(this, &UHeader::OnClick);
	Configurations->OnClickTrigger.AddDynamic(this, &UHeader::OnClick);
	Filter->OnClickTrigger.AddDynamic(this, &UHeader::OnClick);
	MoreInformation->OnClickTrigger.AddDynamic(this, &UHeader::OnClick);

	SelectSystem->OnFocusTrigger.AddDynamic(this, &UHeader::OnFocusClick);
	Configurations->OnFocusTrigger.AddDynamic(this, &UHeader::OnFocusClick);
	Filter->OnFocusTrigger.AddDynamic(this, &UHeader::OnFocusClick);
	MoreInformation->OnFocusTrigger.AddDynamic(this, &UHeader::OnFocusClick);

	SelectSystem->OnFocusLostTrigger.AddDynamic(this, &UHeader::OnLostFocusClick);
	Configurations->OnFocusLostTrigger.AddDynamic(this, &UHeader::OnLostFocusClick);
	Filter->OnFocusLostTrigger.AddDynamic(this, &UHeader::OnLostFocusClick);
	MoreInformation->OnFocusLostTrigger.AddDynamic(this, &UHeader::OnLostFocusClick);

}

void UHeader::OnClick(int32 Index)
{
	OnClickDelegate.Broadcast(Index);
}

void UHeader::OnFocusClick(int32 Index)
{
	OnFocusDelegate.Broadcast(Index);
}

void UHeader::OnLostFocusClick(int32 Index)
{
	OnLostFocusDelegate.Broadcast(Index);
}


void UHeader::SetFocusButton(const int32 Index, const bool bIsSound)
{

	bool bIsFocus = false;

	if (bIsSound)
	{
		UGameplayStatics::PlaySound2D(this, SoundNavigation);
	}

	if (Index > 0)
	{
		bIsFocus = true;
	}

	switch (Index)
	{
	case 1:
		SelectSystem->SetFocus();
		break;
	case 2:
		Configurations->SetFocus();
		break;
	case 3:
		Filter->SetFocus();
		break;
	case 4:
		MoreInformation->SetFocus();
		break;
	default:;
	}

	AnimationFocus(bIsFocus);
}

void UHeader::HasButtonFocus()
{
	TArray<UWidget*> Widgets = HorizontalBoxButtons->GetAllChildren();
	bool bIsFocus = false;
	for (UWidget*& Widget : Widgets)
	{
		UClassicButton* Button = Cast<UClassicButton>(Widget);
		if (Button == nullptr) continue;

		if (Button->HasFocusButton())
		{
			bIsFocus = true;
		}

	}
	AnimationFocus(bIsFocus);
}

void UHeader::EnableButtonsHeader(const int32 Index) const
{
	if (Index == 0)
	{
		Filter->EnableButton(false);
		MoreInformation->EnableButton(false);
		return;
	}
	Filter->EnableButton(true);
	MoreInformation->EnableButton(true);
}

void UHeader::SetThemeButton(UTexture2D* TextureMain, UTexture2D* TextureConfig, UTexture2D* TextureFilter, UTexture2D* TextureInformation, FSlateBrush BackgroundColor)
{
	SelectSystem->SetTheme(TextureMain, BackgroundColor);
	Configurations->SetTheme(TextureConfig, BackgroundColor);
	Filter->SetTheme(TextureFilter, BackgroundColor);
	MoreInformation->SetTheme(TextureInformation, BackgroundColor);
}

void UHeader::SetThemeBackground(UTexture2D* TextureBackground, UTexture2D* TextureMain, UTexture2D* TextureMainOpacity)
{
	Background->SetBrushFromTexture(TextureBackground);
	Main->SetBrushFromTexture(TextureMain);
	MainOpacity->SetBrushFromTexture(TextureMainOpacity);
}

void UHeader::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundNavigation = NavigateSound;
}
