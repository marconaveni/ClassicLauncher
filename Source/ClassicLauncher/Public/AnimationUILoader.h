// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimationUI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/WorldSubsystem.h"
#include "AnimationUILoader.generated.h"


class UWidget;
class UAnimationUI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateStartAnimationUI, UAnimationUI*, Animation, FName, Name);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateFinishAnimationUI, UAnimationUI*, Animation, FName, Name);


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
	UAnimationUI* DefaultAnimation;

    UPROPERTY()
    TMap<FName, UAnimationUI*> WidgetMap;

public:

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FDelegateStartAnimationUI OnStartAnimationTrigger;
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FDelegateFinishAnimationUI OnFinishAnimationTrigger;


    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "AnimationUI|Functions", meta = (AdvancedDisplay = "Curves, bBind, bReset" , ToOpacity="1" , Time="0.1"))
    void PlayAnimation( 
		UWidget* Widget,
        float Time, 
        FWidgetTransform ToPosition, 
        float ToOpacity, 
        bool bReset, 
        TEnumAsByte<EEasingFunc::Type> FunctionCurveFName, 
        FAnimationUICurves Curves,
        FName AnimationName = TEXT("None"),
        bool ForceUpdateAnimation = true,
        bool bBind = true
    );

    UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
    UAnimationUI* GetAnimation(FName Name);

    UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
    void ClearAllAnimations();

    UFUNCTION(BlueprintCallable, Category = "AnimationUI|Functions")
    void Clear();

protected:
    UFUNCTION()
    void StartAnimation(UAnimationUI* CurrentAnimation, FName AnimationName);
    UFUNCTION()
    void FinishAnimation(UAnimationUI* CurrentAnimation, FName AnimationName);

};
