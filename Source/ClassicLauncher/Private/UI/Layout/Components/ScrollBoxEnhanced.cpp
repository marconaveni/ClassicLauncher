// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/Components/ScrollBoxEnhanced.h"
#include "Components/Image.h"
#include "Components/NamedSlot.h"
#include "Components/Scrollbox.h"
#include "Components/VerticalBox.h"
#include "UI/BaseButton.h"


UScrollBoxEnhanced::UScrollBoxEnhanced(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , bUpdateIndex(false)
	  , IndexFocus(0)
	  , IndexWheelFocus(0)
	  , bAutoContent(false)
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

	SetScrollBarVisibility(ScrollBarVisibility);
	bUpdateIndex = true;
	BindButtonsScroll();
}

void UScrollBoxEnhanced::NativeOnInitialized()
{
	Super::NativeOnInitialized();
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
		IndexWheelFocus = FMath::Clamp(IndexWheelFocus - 1, 2, GetAllChildrenContent().Num() - 3);
	}
	else if (WheelDelta < 0)
	{
		IndexWheelFocus = FMath::Clamp(IndexWheelFocus + 1, 2, GetAllChildrenContent().Num() - 3);
	}
	if (GetAllChildrenContent().IsValidIndex(IndexWheelFocus) && WheelDelta != 0)
	{
		ScrollBox->ScrollWidgetIntoView(GetAllChildrenContent()[IndexWheelFocus], true, EDescendantScrollDestination::Center);
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
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
	for (UWidget* Widget : Widgets)
	{
		UBaseButton* BaseButton = Cast<UBaseButton>(Widget);
		if (BaseButton != nullptr && UClassicFunctionLibrary::GetVisibilityWidget(BaseButton))
		{
			BindButton(BaseButton);
			BaseButton->SetIndex(NewIndex);
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

int32 UScrollBoxEnhanced::SetFocusScroll(EScrollTo Scroll)
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
	ScrollBox->ScrollWidgetIntoView(BaseButton, false, EDescendantScrollDestination::Center);
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
