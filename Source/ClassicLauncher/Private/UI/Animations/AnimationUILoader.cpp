// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "UI/Animations/AnimationUILoader.h"
#include "UI/Animations/AnimationUI.h"

void UAnimationUILoader::Initialize(FSubsystemCollectionBase& Collection)
{
	if (!IsValid(DefaultAnimation))
	{
		DefaultAnimation = NewObject<UAnimationUI>(this);
		DefaultAnimation->OnStartAnimationTrigger.AddDynamic(this, &UAnimationUILoader::StartAnimation);
		DefaultAnimation->OnFinishAnimationTrigger.AddDynamic(this, &UAnimationUILoader::FinishAnimation);
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

void UAnimationUILoader::PlayAnimation(UWidget* Widget, float Time, FWidgetTransform ToPosition, float ToOpacity, bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve, FAnimationUICurves Curves, FName AnimationName, bool ForceUpdateAnimation, bool bBind)
{
	UAnimationUI* CurrentAnimate = DefaultAnimation;
	if (!AnimationName.IsEqual("None"))
	{
		UAnimationUI** FoundValue = WidgetMap.Find(AnimationName);
		if (FoundValue == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("null"));
			CurrentAnimate = NewObject<UAnimationUI>(this);
			if (bBind)
			{
				CurrentAnimate->OnStartAnimationTrigger.AddDynamic(this, &UAnimationUILoader::StartAnimation);
				CurrentAnimate->OnFinishAnimationTrigger.AddDynamic(this, &UAnimationUILoader::FinishAnimation);
			}
			WidgetMap.Add(AnimationName, CurrentAnimate);
		}
		else
		{
			CurrentAnimate = *FoundValue;
		}
	}

	CurrentAnimate->SetCurves(Curves);
	CurrentAnimate->PlayAnimation(Widget, Time, ToPosition, ToOpacity, bReset, FunctionCurve, ForceUpdateAnimation, AnimationName);

}

UAnimationUI* UAnimationUILoader::GetAnimation(FName Name)
{
	for (const auto& Element : WidgetMap)
	{
		if (Name == Element.Key)
		{
			return Element.Value;
		}
	}
	return DefaultAnimation;
}

void UAnimationUILoader::ClearAllAnimations()
{
	for (const auto& Element : WidgetMap)
	{
		Element.Value->ClearAnimation();
	}

}

void UAnimationUILoader::Clear()
{
	WidgetMap.Empty();
}

void UAnimationUILoader::StartAnimation(UAnimationUI* CurrentAnimation, const FName AnimationName)
{
	OnStartAnimationTrigger.Broadcast(CurrentAnimation, AnimationName);
}

void UAnimationUILoader::FinishAnimation(UAnimationUI* CurrentAnimation, const FName AnimationName)
{
	OnFinishAnimationTrigger.Broadcast(CurrentAnimation, AnimationName);
}
