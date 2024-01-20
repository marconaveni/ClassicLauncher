// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/Label.h"

#include "TextImageBlock.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"

ULabel::ULabel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , Overlay(nullptr)
	  , TextImageBlock(nullptr)
	  , Size(FVector2D(900, 108))
{
}

void ULabel::NativePreConstruct()
{
	Super::NativePreConstruct();
	UCanvasPanelSlot* SlotPanel = Cast<UCanvasPanelSlot>(Overlay->Slot);
	if (SlotPanel != nullptr)
	{
		SlotPanel->SetSize(Size);
	}
	SetText(Text);
}

void ULabel::SetText(const FText NewText)
{
	TextImageBlock->SetText(NewText);
}

UTextImageBlock* ULabel::GetTextImageBlock()
{
	return TextImageBlock;
}
