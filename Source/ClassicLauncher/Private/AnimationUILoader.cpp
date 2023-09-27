// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "AnimationUILoader.h"

#include "AnimationUI.h"

void UAnimationUILoader::Initialize(FSubsystemCollectionBase& Collection)
{
	if (!IsValid(Animation))
	{
		Animation = NewObject<UAnimationUI>(this);
	}
}

void UAnimationUILoader::Deinitialize()
{
	Super::Deinitialize();
}

bool UAnimationUILoader::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return Super::DoesSupportWorldType(WorldType);
}

void UAnimationUILoader::PlayAnimation( UWidget* Widget, float Time, int32 FPS, FWidgetTransform ToPosition, float ToOpacity, bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve, FAnimationIUCurves Curves, FName AnimationName)
{
	if (AnimationName.IsEqual("None"))
	{
		Animation->SetCurves(Curves);
		Animation->PlayAnimation(Widget, Time, FPS, ToPosition, ToOpacity, bReset, FunctionCurve);
	}
	else
	{
		UAnimationUI** FoundValue = WidgetMap.Find(AnimationName);
		if(FoundValue == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("null"));
			UAnimationUI* NewAnimation = NewObject<UAnimationUI>(this);
			WidgetMap.Add(AnimationName, NewAnimation);
			NewAnimation->SetCurves(Curves);
			NewAnimation->PlayAnimation(Widget, Time, FPS, ToPosition, ToOpacity, bReset, FunctionCurve);
		}
		else
		{
			const auto& ValueAssociatedWithKey = *FoundValue;
			ValueAssociatedWithKey->SetCurves(Curves);
			ValueAssociatedWithKey->PlayAnimation(Widget, Time, FPS, ToPosition, ToOpacity, bReset, FunctionCurve);
		}
	}
}
