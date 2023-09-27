// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slate/WidgetTransform.h"
#include "UObject/NoExportTypes.h"
#include "AnimationUI.generated.h"


USTRUCT(BlueprintType)
struct FAnimationIUCurves
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* TranslationX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* TranslationY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ScaleX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ScaleY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ShearX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ShearY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* Opacity;

	FAnimationIUCurves()
	{
		
	}

	bool CheckTranslationX() { return TranslationX != nullptr; }
	bool CheckTranslationY() { return TranslationY != nullptr; }
	bool CheckScaleX() { return ScaleX != nullptr; }
	bool CheckScaleY() { return ScaleY != nullptr; }
	bool CheckShearX() { return ShearX != nullptr; }
	bool CheckShearY() { return ShearY != nullptr; }
	bool CheckAngle() { return Angle != nullptr; }
	bool CheckOpacity() { return Opacity != nullptr; }
};


class UWidget;
/**
 * 
 */
UCLASS(BlueprintType)
class CLASSICLAUNCHER_API UAnimationUI : public UObject
{
	GENERATED_BODY()


protected:

	UPROPERTY()
	float FramesPerSeconds;
	UPROPERTY()
	float Alpha;
	UPROPERTY()
	float RenderOpacity;
	UPROPERTY()
	float InitialRenderOpacity;
	UPROPERTY()
	int32 Frames;
	UPROPERTY()
	int32 FramesCount;
	UPROPERTY()
	bool bRelative;
	UPROPERTY()
	bool bResetPosition;
	UPROPERTY()
	UWidget* WidgetTarget;
	UPROPERTY()
	FWidgetTransform InitialPosition;
	UPROPERTY()
	FWidgetTransform TargetPosition;
	UPROPERTY()
	TEnumAsByte<EEasingFunc::Type> EasingFunc;
	UPROPERTY()
	FTimerHandle AnimationTimerHandle;
	UPROPERTY()
	FAnimationIUCurves AnimationCurves;

	UFUNCTION()
	void Animation();
	UFUNCTION()
	float SetPositionsCurves(const UCurveFloat* Value) const;
	UFUNCTION()
	float SetPositions(float ValueInitial, float ValueTarget, float ValueAlpha) const;
	

public:

	UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
	void SetCurves(const FAnimationIUCurves& Curves);

	UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
	void PlayAnimation(UWidget* Target, float Time , int32 FPS , FWidgetTransform ToPosition, float ToOpacity , bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve);

	UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
	void ClearAnimations();
};
