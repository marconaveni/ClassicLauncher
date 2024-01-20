// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/ButtonCommon.h"
#include "Components/Image.h"
#include "UI/Components/Arrow.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Kismet/GameplayStatics.h"

UButtonCommon::UButtonCommon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bMouseFocus(false)
{
	TextColorFocus = FSlateColor(FColor::White);
	TextColorNoFocus = FSlateColor(FColor::White);
}

void UButtonCommon::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text->SetText(ButtonText);

}

FReply UButtonCommon::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UButtonCommon::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMouseFocus = true;
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

FReply UButtonCommon::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UButtonCommon::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	bMouseFocus = false;
}

FReply UButtonCommon::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

bool UButtonCommon::Initialize()
{
	bool Success = Super::Initialize();
	return Success;
}

void UButtonCommon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UButtonCommon::SetText(FText NewText)
{
	Text->SetText(NewText);
	ButtonText = NewText;
}

void UButtonCommon::SetFocusButton(bool bEnable)
{
	if (bEnable)
	{
		BackgroundFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Arrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Text->SetColorText(TextColorFocus);
		
		if (!bMouseFocus)
		{
			UGameplayStatics::PlaySound2D(this, SoundSelect);
		}
	}
	else
	{
		Text->SetColorText(TextColorNoFocus);
		BackgroundFocus->SetVisibility(ESlateVisibility::Hidden);
		Arrow->SetVisibility(ESlateVisibility::Hidden);
	}
	Super::SetFocusButton(bEnable);
}

void UButtonCommon::ButtonClick()
{
	if (HasAnyUserFocus())
	{
		UGameplayStatics::PlaySound2D(this, SoundClick);
	}
	Super::ButtonClick();
}

void UButtonCommon::NativeSetBackgroundAppearance(float CornerRadius, FString Color, float BorderWidth, FString BorderColor, float MarginLeft, float MarginTop, float MarginRight, float MarginBottom)
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

void UButtonCommon::NativeSetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin, float Size)
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

void UButtonCommon::NativeSetSize(float Width, float Height)
{
	UCanvasPanelSlot* CanvasPanel = Cast<UCanvasPanelSlot>(OverlayMain->Slot);
	CanvasPanel->SetSize(FVector2D(Width, Height));
}

void UButtonCommon::NativeSetArrow(UTexture2D* Texture0, UTexture2D* Texture1, UTexture2D* Texture2, float Angle, float Margin)
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

void UButtonCommon::NativeSetIcon(UTexture2D* Texture, float Margin, bool bEnable)
{
	if (Texture != nullptr)
	{
		Image->SetBrushFromTexture(Texture);
	}
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image->Slot);
	OverlaySlot->SetPadding(FMargin(Margin, 0, 0, 0));
	Image->SetVisibility((bEnable) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UButtonCommon::NativeSetImage(UTexture2D* Texture, float Margin, bool bEnable)
{
	if (Texture != nullptr)
	{
		Image->SetBrushFromTexture(Texture);
	}
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image->Slot);
	OverlaySlot->SetPadding(FMargin(Margin, 0, 0, 0));
	Image->SetVisibility((bEnable) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UButtonCommon::NativeSetTextColor(FSlateColor TextColorWithFocus, FSlateColor TextColorWithoutFocus)
{
	TextColorFocus = TextColorWithFocus;
	TextColorNoFocus = TextColorWithoutFocus;
	Text->SetColorText( HasFocusButton() ? TextColorFocus : TextColorNoFocus);
	
}

void UButtonCommon::SetBackgroundAppearance(float CornerRadius, FString Color, float BorderWidth, FString BorderColor, float MarginLeft, float MarginTop, float MarginRight, float MarginBottom)
{
	NativeSetBackgroundAppearance(CornerRadius, Color, BorderWidth, BorderColor, MarginLeft, MarginTop, MarginRight, MarginBottom);
}

void UButtonCommon::SetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin, float Size)
{
	NativeSetTextAppearance(NewTextStyle, bEnableImageText, Margin, Size);
}

void UButtonCommon::SetSize(float Width, float Height)
{
	NativeSetSize(Width, Height);
}

void UButtonCommon::SetArrow(UTexture2D* Texture0, UTexture2D* Texture1, UTexture2D* Texture2, float Angle, float Margin)
{
	NativeSetArrow(Texture0, Texture1, Texture2, Angle, Margin);
}

void UButtonCommon::SetIcon(UTexture2D* Texture, float Margin, bool bEnable)
{
	NativeSetIcon(Texture, Margin, bEnable);
}

void UButtonCommon::SetImage(UTexture2D* Texture, float Margin, bool bEnable)
{
	NativeSetImage(Texture, Margin, bEnable);
}

void UButtonCommon::SetTextColor(FSlateColor TextColorWithFocus, FSlateColor TextColorWithoutFocus)
{
	NativeSetTextColor(TextColorWithFocus, TextColorWithoutFocus);
}

void UButtonCommon::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundClick = SelectSound;
	SoundSelect = NavigateSound;
}
