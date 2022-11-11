// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UArrow : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow|Timers")
	FTimerHandle FrameTimerHandle;

	//constructor 
	UArrow(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow|Variables")
	float Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow|Variables")
	float DelayAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow|Variables")
	int32 ArrowIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow|Variables")
    FSlateColor ArrowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow|Variables")
	FVector2D ArrowSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow|Variables")
	TArray<UTexture2D*> ArrowTextures;

	UFUNCTION(BlueprintCallable, Category = "Arrow|Functions")
	void Animate();
	
	UFUNCTION(BlueprintCallable, Category = "Arrow|Functions")
	void SetFrame();
	
};
