// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicButtonSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateFocusLost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateClick, int32, value);
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicButtonSystem : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocus OnFocusTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateFocusLost OnFocusLostTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateClick OnClickTrigger;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Click;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UArrow* WBPArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text;
	
	UClassicButtonSystem(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetText(FString NewText);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void SetCount(int32 NewValue);

	UFUNCTION(BlueprintCallable, Category = "ClassicButtonSystem|Functions")
	void ButtonClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	int32 CountSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	class USoundBase* SoundSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassicButtonSystem|Variables")
	class USoundBase* SoundClick;



private:

	UPROPERTY()
	bool Hover;
};
