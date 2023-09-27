// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/WorldSubsystem.h"
#include "AnimationUILoader.generated.h"

class UWidget;
class UAnimationUI;
/**
 * 
 */
UCLASS(BlueprintType)
class CLASSICLAUNCHER_API UAnimationUILoader : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

    UPROPERTY()
	UAnimationUI* Animation;

    UPROPERTY()
    TMap<FName, UAnimationUI*> WidgetMap;

public:
    UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
	void PlayAnimation( UWidget* Widget, float Time, int32 FPS, FWidgetTransform ToPosition, float ToOpacity, bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurveFName, FAnimationIUCurves Curves ,FName AnimationName = TEXT("None"));
};
