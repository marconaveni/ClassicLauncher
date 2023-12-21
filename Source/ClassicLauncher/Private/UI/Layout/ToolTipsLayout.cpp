// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "UI/Layout/ToolTipsLayout.h"
#include "ToolTip.h"
#include "Components/CanvasPanelSlot.h"

void UToolTipsLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotToolTipSystem = Cast<UCanvasPanelSlot>(ToolTipSystem->Slot);
	SlotToolTipConfiguration = Cast<UCanvasPanelSlot>(ToolTipConfiguration->Slot);
	SlotToolTipFilter = Cast<UCanvasPanelSlot>(ToolTipFilter->Slot);
	SlotToolTipMoreInformation = Cast<UCanvasPanelSlot>(ToolTipMoreInformation->Slot);
}

void UToolTipsLayout::SetToolTipThemes(UTexture2D* TextureBackground, UTexture2D* TextureDetail, bool TextImage, float Size, FTextStyle NewTextStyle)
{
	ToolTipSystem->SetToolTipThemes(TextureBackground, TextureDetail, TextImage, Size, NewTextStyle);
	ToolTipConfiguration->SetToolTipThemes(TextureBackground, TextureDetail, TextImage, Size, NewTextStyle);
	ToolTipFilter->SetToolTipThemes(TextureBackground, TextureDetail, TextImage, Size, NewTextStyle);
	ToolTipMoreInformation->SetToolTipThemes(TextureBackground, TextureDetail, TextImage, Size, NewTextStyle);
}

void UToolTipsLayout::OnFocus(const int32 Index) const
{
	SlotToolTipSystem->SetZOrder(50);
	SlotToolTipConfiguration->SetZOrder(50);
	SlotToolTipFilter->SetZOrder(50);
	SlotToolTipMoreInformation->SetZOrder(50);
	ToolTipSystem->SetFocusToolTip(false);
	ToolTipConfiguration->SetFocusToolTip(false);
	ToolTipFilter->SetFocusToolTip(false);
	ToolTipMoreInformation->SetFocusToolTip(false);

	switch (Index)
	{
	case 1:
		SlotToolTipSystem->SetZOrder(51);
		ToolTipSystem->SetFocusToolTip(true);
		break;
	case 2:
		SlotToolTipConfiguration->SetZOrder(51);
		ToolTipConfiguration->SetFocusToolTip(true);
		break;
	case 3:
		SlotToolTipFilter->SetZOrder(51);
		ToolTipFilter->SetFocusToolTip(true);
		break;
	case 4:
		SlotToolTipMoreInformation->SetZOrder(51);
		ToolTipMoreInformation->SetFocusToolTip(true);
		break;
	default:
		break;
	}
}

void UToolTipsLayout::OnLostFocus(const int32 Index) const
{
	switch (Index)
	{
	case 1:
		SlotToolTipSystem->SetZOrder(50);
		ToolTipSystem->SetFocusToolTip(false);
		break;
	case 2:
		SlotToolTipConfiguration->SetZOrder(50);
		ToolTipConfiguration->SetFocusToolTip(false);
		break;
	case 3:
		SlotToolTipFilter->SetZOrder(50);
		ToolTipFilter->SetFocusToolTip(false);
		break;
	case 4:
		SlotToolTipMoreInformation->SetZOrder(50);
		ToolTipMoreInformation->SetFocusToolTip(false);
		break;
	default:;
	}
}
