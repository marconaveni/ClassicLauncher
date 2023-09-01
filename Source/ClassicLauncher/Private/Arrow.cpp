// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "Arrow.h"
#include "Components/Image.h"

UArrow::UArrow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Angle = 0.0f;
	DelayAnimation = 0.16f;
	ArrowIndex = 0;
	ArrowSize = FVector2D(45.0f,28.0f);
	ArrowTextures.Add(nullptr);
	ArrowTextures.Add(nullptr);
	ArrowTextures.Add(nullptr);
	ArrowTextures.Shrink();
}

void UArrow::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetTheme();
}

void UArrow::NativeConstruct()
{
	Super::NativeConstruct();
	Animate();
}

void UArrow::Animate()
{
	if (ArrowIndex == ArrowTextures.Num() - 1)
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

void UArrow::SetTheme()
{
	BgImage->SetRenderTransformAngle(Angle);
	BgImage->SetBrushTintColor(ArrowColor);
	BgImage->SetDesiredSizeOverride(ArrowSize);
}
