// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ImageLoader.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLoadedImage, UTexture2D*, TextureOut, bool, ValidateIs);

UCLASS()
class CLASSICLAUNCHER_API UImageLoader : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FLoadedImage OnLoadedImage;

	UPROPERTY(BlueprintAssignable)
	FLoadedImage OnFailLoadedImage;

	UPROPERTY()
	FString PathTemp;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UImageLoader* AsyncImageLoader(FString PathImage);

	virtual void Activate() override;

	void LoadTexture();

};