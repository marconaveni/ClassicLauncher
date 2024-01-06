// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassicFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "BaseButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateFocusBaseButton, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateFocusLostBaseButton, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateClickBaseButton, int32, Index);


//enum class EButtonsGame;
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusBaseButton OnFocusTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusLostBaseButton OnFocusLostTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateClickBaseButton OnClickTrigger;

protected:

	UBaseButton(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 Index;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	EButtonsGame Input;
	
	UFUNCTION()
	virtual void ButtonClick();
	
	virtual void SetFocusButton(bool bEnable);

public:

	UFUNCTION(BlueprintCallable, Category = "Button|Functions")
	void SetIndex(int32 NewIndex);
	
	UFUNCTION(BlueprintPure, Category = "Button|Functions")
	virtual bool HasFocusButton() final;
	
private:
	
	UPROPERTY()
	bool bFocus;
	
};
