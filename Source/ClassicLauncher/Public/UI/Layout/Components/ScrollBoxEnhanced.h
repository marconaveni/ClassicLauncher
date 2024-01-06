// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBoxEnhanced.generated.h"

class UImage;
class UScrollBox;
class UBaseButton;

UENUM(BlueprintType, Category = "Navigation")
enum class EScrollTo : uint8
{
	NONE  UMETA(DisplayName = "None"),
	DOWN  UMETA(DisplayName = "Down"),
	UP    UMETA(DisplayName = "Up")
};

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UScrollBoxEnhanced : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ArrowUP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ArrowDown;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UNamedSlot* NamedSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VerticalBoxContent;
	
private:

	UScrollBoxEnhanced(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;
	
	UFUNCTION()
	void OnUserScrolled(float CurrentOffset);

	UFUNCTION()
	void SetIconArrow();

	UPROPERTY()
	bool bUpdateIndex;
	
	UPROPERTY()
	int32 IndexFocus;

	UPROPERTY()
	int32 IndexWheelFocus;
	
	UPROPERTY()
	float ScrollCurrentOffSet;

	UPROPERTY()
	FTimerHandle IndexWheelHandle;

	void BindButton(UWidget* Content);
	
	void BindButton(UBaseButton* Content);

	UWidget* AddIndex(EScrollTo Scroll);


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bAutoContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ESlateVisibility ScrollBarVisibility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTexture2D* ArrowIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTexture2D* ArrowIconOutline;
	
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void SetContent(UWidget* Content);
	
	void BindButtonsScroll();
	
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void ClearAllChildrenContent();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	int32 SetFocusScroll(EScrollTo Scroll);
	
	UFUNCTION(BlueprintPure, Category = "Functions")
	TArray<UWidget*> GetAllChildrenContent() const;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void SetScrollBarVisibility(ESlateVisibility EnableVisibility);

	UFUNCTION(BlueprintPure, Category = "Functions")
	bool GetScrollBarVisibility();

	UFUNCTION()
	void OnFocusButton(int32 Index);

	UFUNCTION()
	void OnLostFocusButton(int32 Index);
};
