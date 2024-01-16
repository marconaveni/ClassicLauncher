// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "ClassicButtonSystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Arrow.h"
#include "ClassicFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Kismet/GameplayStatics.h"

UClassicButtonSystem::UClassicButtonSystem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bMouseFocus(false)
{
}

void UClassicButtonSystem::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text->SetText(ButtonText);
}

FReply UClassicButtonSystem::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UClassicButtonSystem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMouseFocus = true;
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

FReply UClassicButtonSystem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UClassicButtonSystem::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	bMouseFocus = false;
}

FReply UClassicButtonSystem::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

bool UClassicButtonSystem::Initialize()
{
	bool Success = Super::Initialize();
	return Success;
}

void UClassicButtonSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UClassicButtonSystem::SetText(FText NewText)
{
	Text->SetText(NewText);
	ButtonText = NewText;
}

void UClassicButtonSystem::SetFocusButton(bool bEnable)
{
	if (bEnable)
	{
		BackgroundFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Arrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (!bMouseFocus)
		{
			UGameplayStatics::PlaySound2D(this, SoundSelect);
		}
	}
	else
	{
		BackgroundFocus->SetVisibility(ESlateVisibility::Hidden);
		Arrow->SetVisibility(ESlateVisibility::Hidden);
	}
	Super::SetFocusButton(bEnable);
}

void UClassicButtonSystem::ButtonClick()
{
	if (HasAnyUserFocus())
	{
		UGameplayStatics::PlaySound2D(this, SoundClick);
	}
	Super::ButtonClick();
}

void UClassicButtonSystem::SetBackgroundAppearance(float CornerRadius, FString Color, float BorderWidth, FString BorderColor, float MarginLeft, float MarginTop, float MarginRight, float MarginBottom)
{
	
	FSlateBrushOutlineSettings BorderOutlineSettings;
	BorderOutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	BorderOutlineSettings.CornerRadii = FVector4(CornerRadius, CornerRadius, CornerRadius, CornerRadius);
	BorderOutlineSettings.Width = BorderWidth;
	BorderOutlineSettings.Color = FLinearColor::FromSRGBColor(UClassicFunctionLibrary::HexToColor(BorderColor));
	FSlateBrush Brush;
	Brush.DrawAs = BackgroundDrawAs;
	Brush.Margin = FMargin(0.5f);
	Brush.SetImageSize(FVector2f(1920, 32));
	Brush.TintColor = FLinearColor::FromSRGBColor(UClassicFunctionLibrary::HexToColor(Color));
	Brush.OutlineSettings = FSlateBrushOutlineSettings(BorderOutlineSettings);
	BackgroundFocus->SetBrush(Brush);
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(BackgroundFocus->Slot);
	OverlaySlot->SetPadding(FMargin(MarginLeft, MarginTop, MarginRight, MarginBottom));
}

void UClassicButtonSystem::SetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin, float Size)
{
	Text->SetTextStyle(NewTextStyle);
	Text->SetTextImageSize(Size);
	Text->DefaultToImageText(bEnableImageText, true);
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Text->Slot);
	if (OverlaySlot != nullptr)
	{
		OverlaySlot->SetPadding(FMargin(Margin, 0, 0, 0));
	}
}

void UClassicButtonSystem::SetSize(float Width, float Height)
{
	UCanvasPanelSlot* CanvasPanel = Cast<UCanvasPanelSlot>(OverlayMain->Slot);
	CanvasPanel->SetSize(FVector2D(Width, Height));
}

void UClassicButtonSystem::SetArrow(UTexture2D* Texture0, UTexture2D* Texture1, UTexture2D* Texture2, float Angle, float Margin)
{
	Arrow->ArrowTextures.Empty();
	if (Texture0 != nullptr)
	{
		Arrow->ArrowTextures.Add(Texture0);
	}
	if (Texture1 != nullptr)
	{
		Arrow->ArrowTextures.Add(Texture1);
	}
	if (Texture2 != nullptr)
	{
		Arrow->ArrowTextures.Add(Texture2);
	}
	Arrow->Angle = Angle;
	Arrow->SetTheme();
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Arrow->Slot);
	OverlaySlot->SetPadding(FMargin(Margin, 0, 0, 0));
}

void UClassicButtonSystem::SetIcon(UTexture2D* Texture, float Margin, bool bEnable)
{
	if (Texture != nullptr)
	{
		Image->SetBrushFromTexture(Texture);
	}
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image->Slot);
	OverlaySlot->SetPadding(FMargin(Margin, 0, 0, 0));
	Image->SetVisibility((bEnable) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UClassicButtonSystem::SetImage(UTexture2D* Texture, float Margin, bool bEnable)
{
	if (Texture != nullptr)
	{
		Image->SetBrushFromTexture(Texture);
	}
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image->Slot);
	OverlaySlot->SetPadding(FMargin(Margin, 0, 0, 0));
	Image->SetVisibility((bEnable) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UClassicButtonSystem::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundClick = SelectSound;
	SoundSelect = NavigateSound;
}
