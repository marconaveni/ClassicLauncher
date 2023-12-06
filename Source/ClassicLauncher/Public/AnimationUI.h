// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slate/WidgetTransform.h"
#include "UObject/NoExportTypes.h"
#include "AnimationUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateStartAnimation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFinishAnimation);


USTRUCT(BlueprintType)
struct FAnimationUICurves
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

	FAnimationUICurves(): TranslationX(nullptr), TranslationY(nullptr), ScaleX(nullptr), ScaleY(nullptr),
	                      ShearX(nullptr), ShearY(nullptr),
	                      Angle(nullptr),
	                      Opacity(nullptr)
	{}

	bool CheckTranslationX() const { return TranslationX != nullptr; }
	bool CheckTranslationY() const { return TranslationY != nullptr; }
	bool CheckScaleX() const { return ScaleX != nullptr; }
	bool CheckScaleY() const { return ScaleY != nullptr; }
	bool CheckShearX() const { return ShearX != nullptr; }
	bool CheckShearY() const { return ShearY != nullptr; }
	bool CheckAngle() const { return Angle != nullptr; }
	bool CheckOpacity() const { return Opacity != nullptr; }
};


class UWidget;
/**
 * 
 */
UCLASS(BlueprintType)
class CLASSICLAUNCHER_API UAnimationUI : public UObject
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateStartAnimation OnStartAnimationTrigger;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFinishAnimation OnFinishAnimationTrigger;

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
	FAnimationUICurves AnimationCurves;
	UPROPERTY()
	float CurrentTime;
	UPROPERTY()
	float TimeAnimation;

	UFUNCTION()
	void Animation();
	UFUNCTION()
	float SetPositionsCurves(const UCurveFloat* Value) const;
	UFUNCTION()
	float SetPositions(float ValueInitial, float ValueTarget, float ValueAlpha) const;
	

public:

	UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
	void SetCurves(const FAnimationUICurves& Curves);

	UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
	void PlayAnimation(UWidget* Target, float Time , FWidgetTransform ToPosition, float ToOpacity , bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve, bool ForceUpdateAnimation = true);

	UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
	void ClearAnimation();

	UFUNCTION(BlueprintPure, Category = "AnimationUI|Functions")
	float GetCurrentTimeAnimation() const;
};
