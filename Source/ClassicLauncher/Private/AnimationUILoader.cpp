// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "AnimationUILoader.h"
#include "AnimationUI.h"

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

void UAnimationUILoader::PlayAnimation(UWidget* Widget, float Time, FWidgetTransform ToPosition, float ToOpacity, bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve, FAnimationUICurves Curves, FName AnimationName, bool ForceUpdateAnimation)
{
	CurrentAnimation = DefaultAnimation;
	if (!AnimationName.IsEqual("None"))
	{
		UAnimationUI** FoundValue = WidgetMap.Find(AnimationName);
		if (FoundValue == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("null"));
			CurrentAnimation = NewObject<UAnimationUI>(this);
			CurrentAnimation->OnStartAnimationTrigger.AddDynamic(this, &UAnimationUILoader::StartAnimation);
			CurrentAnimation->OnFinishAnimationTrigger.AddDynamic(this, &UAnimationUILoader::FinishAnimation);
			WidgetMap.Add(AnimationName, CurrentAnimation);
		}
		else
		{
			CurrentAnimation = *FoundValue;
		}
	}

	CurrentNameAnimation = AnimationName;
	CurrentAnimation->SetCurves(Curves);
	CurrentAnimation->PlayAnimation(Widget, Time, ToPosition, ToOpacity, bReset, FunctionCurve, ForceUpdateAnimation);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentNameAnimation.ToString());

}

UAnimationUI* UAnimationUILoader::GetAnimation(FName Name)
{
	for (const auto& Element : WidgetMap)
	{
		if(Name == Element.Key)
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

void UAnimationUILoader::StartAnimation() 
{
	OnStartAnimationTrigger.Broadcast(CurrentAnimation, CurrentNameAnimation);
}

void UAnimationUILoader::FinishAnimation()
{
	OnFinishAnimationTrigger.Broadcast(CurrentAnimation, CurrentNameAnimation);
}
