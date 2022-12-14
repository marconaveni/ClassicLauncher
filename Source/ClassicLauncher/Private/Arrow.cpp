// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Arrow.h"
#include "Components/Image.h"

UArrow::UArrow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Angle = 0.0f;
	DelayAnimation = 0.16f;
	ArrowSize = FVector2D(45.0f,28.0f);
	ArrowTextures.Add(nullptr);
	ArrowTextures.Add(nullptr);
	ArrowTextures.Add(nullptr);
	ArrowTextures.Shrink();
}

void UArrow::NativePreConstruct()
{
	Super::NativePreConstruct();
	BgImage->SetRenderTransformAngle(Angle);
	BgImage->SetBrushTintColor(ArrowColor);
	BgImage->SetDesiredSizeOverride(ArrowSize);
}

void UArrow::NativeConstruct()
{
	Super::NativeConstruct();
	Animate();
}

void UArrow::Animate()
{
	if (ArrowIndex == 2)
	{
		ArrowIndex = 0;
	}
	else
	{
		ArrowIndex++;
	}
	SetFrame();
}

void UArrow::SetFrame()
{
	if (ArrowTextures.IsValidIndex(ArrowIndex))
	{
		BgImage->SetBrushFromTexture(ArrowTextures[ArrowIndex]);
	}
	GetWorld()->GetTimerManager().SetTimer(FrameTimerHandle, this, &UArrow::Animate , DelayAnimation , false, -1);
}
