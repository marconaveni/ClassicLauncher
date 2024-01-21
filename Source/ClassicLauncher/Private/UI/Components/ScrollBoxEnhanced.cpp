// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/ScrollBoxEnhanced.h"
#include "Components/Image.h"
#include "Components/NamedSlot.h"
#include "Components/Scrollbox.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/BaseButton.h"


UScrollBoxEnhanced::UScrollBoxEnhanced(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , bUpdateIndex(false)
	  , IndexFocus(0)
	  , IndexWheelFocus(0)
	  , bAutoContent(false)
	  , StartIndexFocus(0)
	  , OffsetTop(4)
	  , OffsetBottom(4)
	  , ScrollBarVisibility(ESlateVisibility::Hidden)
{
}

void UScrollBoxEnhanced::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (bAutoContent)
	{
		NamedSlot->AddChild(VerticalBoxContent);
	}
	ScrollBarSettings(nullptr, nullptr, ScrollbarThickness, ScrollbarThicknessBackground);
	SetScrollBarVisibility(ScrollBarVisibility);
	bUpdateIndex = true;
	BindButtonsScroll();
}

void UScrollBoxEnhanced::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IndexFocus = StartIndexFocus;
	ScrollBox->OnUserScrolled.AddDynamic(this, &UScrollBoxEnhanced::OnUserScrolled);
}

void UScrollBoxEnhanced::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetIconArrow();
}

FReply UScrollBoxEnhanced::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float WheelDelta = InMouseEvent.GetWheelDelta();

	if (bUpdateIndex)
	{
		bUpdateIndex = false;
		IndexWheelFocus = IndexFocus;
	}

	if (WheelDelta > 0)
	{
		IndexWheelFocus = FMath::Clamp(IndexWheelFocus - 1, 0, GetAllChildrenContent().Num() - 1 - OffsetBottom);
	}
	else if (WheelDelta < 0)
	{
		IndexWheelFocus = FMath::Clamp(IndexWheelFocus + 1, OffsetTop, GetAllChildrenContent().Num());
	}
	if (GetAllChildrenContent().IsValidIndex(IndexWheelFocus) && WheelDelta != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("scroll Index Wheel %d"), IndexWheelFocus);
		ScrollBox->ScrollWidgetIntoView(GetAllChildrenContent()[IndexWheelFocus], true, EDescendantScrollDestination::Center);
	}

	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UScrollBoxEnhanced::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UScrollBoxEnhanced::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bUpdateIndex = true;
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UScrollBoxEnhanced::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bUpdateIndex = true;
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UScrollBoxEnhanced::OnUserScrolled(float CurrentOffset)
{
	SetIconArrow();
}

void UScrollBoxEnhanced::SetIconArrow()
{
	ScrollCurrentOffSet = ScrollBox->GetScrollOffset();

	if (GetAllChildrenContent().Num() <= 5)
	{
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		return;
	}

	if (ScrollCurrentOffSet < 1)
	{
		//set ArrowUP
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIcon);
		return;
	}

	if (ScrollCurrentOffSet >= ScrollBox->GetScrollOffsetOfEnd() - 1)
	{
		//set ArrowDown
		ArrowUP->SetBrushFromTexture(ArrowIcon);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		return;
	}

	ArrowUP->SetBrushFromTexture(ArrowIcon);
	ArrowDown->SetBrushFromTexture(ArrowIcon);
}

void UScrollBoxEnhanced::SetContent(UWidget* Content)
{
	VerticalBoxContent->AddChild(Content);
	BindButton(Content);
}

void UScrollBoxEnhanced::BindButton(UWidget* Content)
{
	UBaseButton* BaseButton = Cast<UBaseButton>(Content);
	if (BaseButton != nullptr)
	{
		BindButton(BaseButton);
	}
}

void UScrollBoxEnhanced::BindButton(UBaseButton* Content)
{
	Content->OnFocusTrigger.AddUniqueDynamic(this, &UScrollBoxEnhanced::OnFocusButton);
	Content->OnFocusLostTrigger.AddUniqueDynamic(this, &UScrollBoxEnhanced::OnLostFocusButton);
}

void UScrollBoxEnhanced::BindButtonsScroll()
{
	TArray<UWidget*> Widgets = GetAllChildrenContent();
	int32 NewIndex = 0;
	BaseButtons.Empty();
	for (UWidget* Widget : Widgets)
	{
		UBaseButton* BaseButton = Cast<UBaseButton>(Widget);
		if (BaseButton != nullptr && UClassicFunctionLibrary::GetVisibilityWidget(BaseButton))
		{
			BindButton(BaseButton);
			BaseButton->SetIndex(NewIndex);
			BaseButtons.Add(BaseButton);
		}
		NewIndex++;
	}
	SetIconArrow();
}

void UScrollBoxEnhanced::ClearAllChildrenContent()
{
	VerticalBoxContent->ClearChildren();
}

UWidget* UScrollBoxEnhanced::AddIndex(EScrollTo Scroll)
{
	const int32 Count = GetAllChildrenContent().Num() - 1;
	if (Scroll == EScrollTo::UP)
	{
		IndexFocus--;
		IndexFocus = (IndexFocus < 0) ? Count : IndexFocus;
	}
	else if (Scroll == EScrollTo::DOWN)
	{
		IndexFocus++;
		IndexFocus = (IndexFocus > Count) ? 0 : IndexFocus;
	}
	if (GetAllChildrenContent().IsValidIndex(IndexFocus))
	{
		return GetAllChildrenContent()[IndexFocus];
	}
	return nullptr;
}

int32 UScrollBoxEnhanced::SetFocusScroll(EScrollTo Scroll, bool AnimatedScroll)
{
	UBaseButton* BaseButton = nullptr;
	for (int i = 0; i < GetAllChildrenContent().Num() - 1; i++)
	{
		BaseButton = Cast<UBaseButton>(AddIndex(Scroll));
		if (BaseButton != nullptr)
		{
			if (BaseButton->IsFocusable() && UClassicFunctionLibrary::GetVisibilityWidget(BaseButton))
			{
				BaseButton->SetFocus();
				break;
			}
		}
	}
	ScrollBox->ScrollWidgetIntoView(BaseButton, AnimatedScroll, EDescendantScrollDestination::Center);
	return IndexFocus;
}

TArray<UWidget*> UScrollBoxEnhanced::GetAllChildrenContent() const
{
	UPanelWidget* Content = Cast<UPanelWidget>(NamedSlot->GetChildAt(0));
	if (Content != nullptr)
	{
		return Content->GetAllChildren();
	}
	return TArray<UWidget*>();
}

TArray<UBaseButton*> UScrollBoxEnhanced::GetAllBaseButtons() const
{
	return BaseButtons;
}

void UScrollBoxEnhanced::SetScrollBarVisibility(ESlateVisibility EnableVisibility)
{
	ScrollBarVisibility = EnableVisibility;
	const bool bAlwaysShowScrollbar = (EnableVisibility == ESlateVisibility::Visible);
	ScrollBox->SetAlwaysShowScrollbar(bAlwaysShowScrollbar);
	ScrollBox->SetScrollBarVisibility(EnableVisibility);
	ArrowUP->SetVisibility(EnableVisibility);
	ArrowDown->SetVisibility(EnableVisibility);
}

bool UScrollBoxEnhanced::GetScrollBarVisibility()
{
	return ScrollBox->GetScrollBarVisibility() == ESlateVisibility::Visible;
}

void UScrollBoxEnhanced::OnFocusButton(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("focus %d"), Index);
	IndexFocus = Index;
}

void UScrollBoxEnhanced::OnLostFocusButton(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("lost focus %d"), Index);
}

void UScrollBoxEnhanced::SetArrowIcons(UTexture2D* Texture, UTexture2D* TextureOutLine)
{
	if (Texture != nullptr)
	{
		ArrowIcon = Texture;
	}
	if (TextureOutLine != nullptr)
	{
		ArrowIconOutline = TextureOutLine;
	}
	SetIconArrow();
}

void UScrollBoxEnhanced::ScrollBarSettings(UTexture2D* TextureThumb, UTexture2D* TextureBackground, const float SizeThumb, const float SizeBackground)
{
	FMargin Margin;
	Margin.Bottom = 85;
	Margin.Top = 85;
	Margin.Left = 0;
	Margin.Right = (45.0f - SizeThumb) / 2;
	ScrollbarThickness = FMath::Clamp(SizeThumb, 0, 45);
	ScrollBox->SetScrollbarPadding(Margin);
	ScrollBox->SetScrollbarThickness(FVector2D(ScrollbarThickness, 0));
	FScrollBarStyle BarStyle = ScrollBox->GetWidgetBarStyle();
	if (TextureThumb != nullptr)
	{
		FSlateBrush SlateBrushThumb = ScrollBox->GetWidgetBarStyle().NormalThumbImage;
		SlateBrushThumb.SetResourceObject(TextureThumb);
		BarStyle.SetNormalThumbImage(SlateBrushThumb);
		BarStyle.SetHoveredThumbImage(SlateBrushThumb);
		BarStyle.SetDraggedThumbImage(SlateBrushThumb);
	}
	if (TextureBackground != nullptr)
	{
		FSlateBrush SlateBrushTopBottom = ScrollBox->GetWidgetBarStyle().HorizontalTopSlotImage;
		SlateBrushTopBottom.ImageSize = FVector2D(SizeBackground, 1);
		SlateBrushTopBottom.DrawAs = ESlateBrushDrawType::Image;
		SlateBrushTopBottom.SetResourceObject(TextureBackground);
		BarStyle.SetVerticalTopSlotImage(SlateBrushTopBottom);
		BarStyle.SetVerticalBottomSlotImage(SlateBrushTopBottom);
	}
	ScrollBox->SetWidgetBarStyle(BarStyle);
}
