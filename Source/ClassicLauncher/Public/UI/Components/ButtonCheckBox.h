// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ButtonCommon.h"
#include "ButtonCheckBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateOnCheck, bool, bCheck);

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UButtonCheckBox : public UButtonCommon
{
	
	GENERATED_BODY()
	
protected:
	
	virtual void ButtonClick() override;
	
public:
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateOnCheck CheckDelegate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* CheckBoxEnable;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* CheckBoxDisable;

	UFUNCTION(BlueprintCallable, Category = "Button|Functions")
	void SetCheckBoxPosition(FMargin Margin);

	UFUNCTION(BlueprintCallable, Category = "Button|Functions")
	void SetTexturesCheckBox(UTexture2D* TextureEnable, UTexture2D* TextureDisable);

	UFUNCTION(BlueprintCallable, Category = "Button|Functions")
	void SetCheckBox(bool bEnable);

	UFUNCTION(BlueprintPure, Category = "Button|Functions")
	bool HasChecked() const;

private:
	
	UPROPERTY()
	bool bChecked;
	
};
