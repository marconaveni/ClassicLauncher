// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Label.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API ULabel : public UUserWidget
{
	GENERATED_BODY()

protected:

	ULabel(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextImageBlock* TextImageBlock;

	virtual void NativePreConstruct() override;

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Size;

	UFUNCTION(BlueprintCallable)
	void SetText(const FText NewText);

	UFUNCTION(BlueprintCallable)
	UTextImageBlock* GetTextImageBlock();
};
