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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
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
	TArray<UBaseButton*> BaseButtons;
	
	UPROPERTY()
	FTimerHandle IndexWheelHandle;

	void BindButton(UWidget* Content);
	
	void BindButton(UBaseButton* Content);

	UWidget* AddIndex(EScrollTo Scroll);

	UFUNCTION()
	void OnFocusButton(int32 Index);

	UFUNCTION()
	void OnLostFocusButton(int32 Index);
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default" , meta=(UIMin = 0 , UIMax = 45))
	float ScrollbarThickness = 28;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default" , meta=(UIMin = 0 , UIMax = 45))
	float ScrollbarThicknessBackground = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bAutoContent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 StartIndexFocus;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 OffsetTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 OffsetBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	ESlateVisibility ScrollBarVisibility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UTexture2D* ArrowIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UTexture2D* ArrowIconOutline;
	
	UFUNCTION(BlueprintCallable, Category = "ScrollBoxEnhanced|Functions")
	void SetContent(UWidget* Content);
	
	void BindButtonsScroll();
	
	UFUNCTION(BlueprintCallable, Category = "ScrollBoxEnhanced|Functions")
	void ClearAllChildrenContent();

	UFUNCTION(BlueprintCallable, Category = "ScrollBoxEnhanced|Functions")
	int32 SetFocusScroll(EScrollTo Scroll, bool AnimatedScroll = false);
	
	UFUNCTION(BlueprintPure, Category = "ScrollBoxEnhanced|Functions")
	TArray<UWidget*> GetAllChildrenContent() const;

	UFUNCTION(BlueprintPure, Category = "ScrollBoxEnhanced|Functions")
	TArray<UBaseButton*> GetAllBaseButtons() const;

	UFUNCTION(BlueprintCallable, Category = "ScrollBoxEnhanced|Functions")
	void SetScrollBarVisibility(ESlateVisibility EnableVisibility);

	UFUNCTION(BlueprintPure, Category = "ScrollBoxEnhanced|Functions")
	bool GetScrollBarVisibility();

	UFUNCTION(BlueprintCallable, Category = "ScrollBoxEnhanced|Functions")
	void SetArrowIcons(UTexture2D* Texture, UTexture2D* TextureOutLine);

	UFUNCTION(BlueprintCallable, Category = "ScrollBoxEnhanced|Functions")
	void ScrollBarSettings(UTexture2D* TextureThumb, UTexture2D* TextureBackground, const float SizeThumb, const float SizeBackground);
};
