// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ButtonCommon.h"
#include "ButtonCommonAlternative.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UButtonCommonAlternative : public UButtonCommon
{
	GENERATED_BODY()

protected:
	
	virtual void SetFocusButton(bool bEnable) override;
	virtual bool Initialize() override;
	virtual void NativeSetBackgroundAppearance(float CornerRadius, FString Color, float BorderWidth, FString BorderColor, float MarginLeft, float MarginTop, float MarginRight, float MarginBottom) override;
	virtual void NativeSetTextAppearance(FTextStyle NewTextStyle, bool bEnableImageText, float Margin, float Size) override;
	virtual void NativeSetSize(float Width, float Height) override;
	virtual void NativeSetArrow(UTexture2D* Texture0, UTexture2D* Texture1, UTexture2D* Texture2, float Angle, float Margin) override;
	virtual void NativeSetIcon(UTexture2D* Texture, float Margin, bool bEnable) override;
	virtual void NativeSetImage(UTexture2D* Texture, float Margin, bool bEnable) override;

public:
	
	UFUNCTION(BlueprintCallable)
	void SetButtonTextures(UTexture2D* TextureToFocus, UTexture2D* TextureNoFocus);
};
