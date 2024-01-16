// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "UObject/Object.h"
#include "ImageLoader.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLoadImagesDelegate, UTexture2D*, TextureOut, int32, IndexLoad, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoopFinalizeDelegate);

/**
 * 
 */

UCLASS()
class CLASSICLAUNCHER_API UImageLoader : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	UImageLoader();

	UPROPERTY()
	int32 FirstIndex;

	UPROPERTY()
	int32 LastIndex;

	UPROPERTY()
	int32 Counter;

	UPROPERTY()
	int32 MaxLoop;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageLoader")
	UTexture2D* DefaultTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageLoader")
	TMap<int32, UTexture2D*> TexturesMap;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ImageLoader")
	FOnLoadImagesDelegate ImageDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ImageLoader")
	FOnLoopFinalizeDelegate FinalizeDelegate;

	UFUNCTION(BlueprintCallable, Category = "ImageLoader")
	void AddImageRange(const EButtonsGame Input, int32 IndexCard, UPARAM(ref) TArray<FGameData>& GameData, const int32 DistanceIndex = 32);

	UFUNCTION(BlueprintCallable, Category = "ImageLoader")
	void AddImageLoop(int32 IndexCard, UPARAM(ref) TArray<FGameData>& GameData, const int32 Count = 64);

private:

	UFUNCTION()
	void OutLoadImage(UTexture2D* TextureOut, int32 Index, bool Sucess);

	UFUNCTION()
	void OutLoopImage(UTexture2D* TextureOut, int32 Index, bool Sucess);
};
