// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/Components/ScrollBoxEnhanced.h"
#include "Components/Image.h"
#include "Components/NamedSlot.h"
#include "Components/Scrollbox.h"
#include "Components/VerticalBox.h"
#include "UI/BaseButton.h"

void UScrollBoxEnhanced::NativePreConstruct()
{
	Super::NativePreConstruct();
	if(bAutoContent)
	{
		NamedSlot->AddChild(VerticalBoxContent);
	}
	bUpdateIndex = true;
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

	if(bUpdateIndex)
	{
		bUpdateIndex = false;
		IndexWheelFocus = IndexFocus;
	}
	
	if(WheelDelta > 0)
	{
		IndexWheelFocus = FMath::Clamp(IndexWheelFocus - 1 , 0 ,GetAllChildrenContent().Num()); 
	}
	else if(WheelDelta < 0)
	{
		IndexWheelFocus = FMath::Clamp(IndexWheelFocus + 1 , 0 ,GetAllChildrenContent().Num());
	}
	if(GetAllChildrenContent().IsValidIndex(IndexWheelFocus) && WheelDelta != 0 )
	{
		ScrollBox->ScrollWidgetIntoView(GetAllChildrenContent()[IndexWheelFocus], true , EDescendantScrollDestination::Center);
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

	if (VerticalBoxContent->GetAllChildren().Num() <= 5)
	{
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		return;
	}
	
	if (ScrollCurrentOffSet >= ScrollBox->GetScrollOffsetOfEnd() - 2)
	{
		//set ArrowDown
		ArrowUP->SetBrushFromTexture(ArrowIcon);
		ArrowDown->SetBrushFromTexture(ArrowIconOutline);
		return;
	}
	if (ScrollCurrentOffSet <= 1)
	{
		//set ArrowUP
		ArrowUP->SetBrushFromTexture(ArrowIconOutline);
		ArrowDown->SetBrushFromTexture(ArrowIcon);
		return;
	}

	ArrowUP->SetBrushFromTexture(ArrowIcon);
	ArrowDown->SetBrushFromTexture(ArrowIcon);
}

void UScrollBoxEnhanced::SetContent(UWidget* Content)
{
	VerticalBoxContent->AddChild(Content);
	UBaseButton* BaseButton = Cast<UBaseButton>(Content);
	if(BaseButton != nullptr)
	{
		BaseButton->OnFocusTrigger.AddUniqueDynamic(this, &UScrollBoxEnhanced::OnFocusButton);
		BaseButton->OnFocusLostTrigger.AddUniqueDynamic(this, &UScrollBoxEnhanced::OnLostFocusButton);
	}
}

void UScrollBoxEnhanced::ClearAllChildrenContent()
{
	VerticalBoxContent->ClearChildren();
}

int32 UScrollBoxEnhanced::SetFocusScroll(EScrollTo Scroll)
{
	const int32 Count = GetAllChildrenContent().Num() - 1 ;
	if(Scroll == EScrollTo::UP)
	{
		IndexFocus--;
		IndexFocus = (IndexFocus < 0 ) ?  Count : IndexFocus  ; 
	}
	else if(Scroll == EScrollTo::DOWN)
	{
		IndexFocus++;
		IndexFocus = (IndexFocus > Count ) ?  0 : IndexFocus ; 
	}
	if(GetAllChildrenContent().IsValidIndex(IndexFocus))
	{
		GetAllChildrenContent()[IndexFocus]->SetFocus();
		ScrollBox->ScrollWidgetIntoView(GetAllChildrenContent()[IndexFocus], false , EDescendantScrollDestination::Center);
	}
	return IndexFocus;
}

TArray<UWidget*> UScrollBoxEnhanced::GetAllChildrenContent() const
{
	return VerticalBoxContent->GetAllChildren();
}

void UScrollBoxEnhanced::OnFocusButton(int32 Index)
{
	UE_LOG(LogTemp,Warning,TEXT("focus %d"), Index);
	IndexFocus = Index;
}

void UScrollBoxEnhanced::OnLostFocusButton(int32 Index)
{
	UE_LOG(LogTemp,Warning,TEXT("lost focus %d"), Index);
}
