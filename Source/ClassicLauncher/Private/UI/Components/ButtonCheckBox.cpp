// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Components/ButtonCheckBox.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"


void UButtonCheckBox::ButtonClick()
{
	Super::ButtonClick();
	SetCheckBox(!bChecked);
}

void UButtonCheckBox::SetCheckBoxPosition(FMargin Margin)
{
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image->Slot);
	OverlaySlot->SetPadding(Margin);
}

void UButtonCheckBox::SetTexturesCheckBox(UTexture2D* TextureEnable, UTexture2D* TextureDisable)
{
	CheckBoxEnable = TextureEnable;
	CheckBoxDisable = TextureDisable;
	NativeSetImage(HasChecked() ? TextureEnable : TextureDisable, 0, true);
}

void UButtonCheckBox::SetCheckBox(bool bEnable)
{
	if (CheckBoxEnable != nullptr && CheckBoxDisable != nullptr)
	{
		if (bEnable)
		{
			bChecked = true;
			Image->SetBrushFromTexture(CheckBoxEnable);
		}
		else
		{
			bChecked = false;
			Image->SetBrushFromTexture(CheckBoxDisable);
		}
		CheckDelegate.Broadcast(bChecked);
	}
}

bool UButtonCheckBox::HasChecked() const
{
	return bChecked;
}
