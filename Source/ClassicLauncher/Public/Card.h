// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnimationUI.h"
#include "Card.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateRelease, int32 , Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateHovered, int32 , Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateUnhovered, int32 , Index);

/**
 * 
 */

class UImage;
class UButton;
class UAnimationUI;

UCLASS()
class CLASSICLAUNCHER_API UCard : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateRelease OnReleaseTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateHovered OnHoveredTrigger;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateUnhovered OnUnhoveredTrigger;

	UCard(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	FAnimationUICurves CurveFavoritesFoward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	FAnimationUICurves CurveFavoritesReverse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	FString PathImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	int32 MapIndexGameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	int32 IndexGameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	int32 IndexCard;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	TArray<FSlateBrush> PlayersImage;
	
	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetPlayers(FString NumberPlayers);

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetFocusCard(bool bEnable, bool bAnimate = true, bool bReset = false, float TimeAnimation = 0.2f);

	UFUNCTION(BlueprintCallable, Category = "Card|Functions")
	bool BindButton();

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetThemeCard(UTexture2D* texture);

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetFavorite(bool bEnable, bool bAnimateIcon);

	UFUNCTION(BlueprintCallable, Category = "Card|Functions")
	void SetCardImage(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void Release();

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void Hovered();

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void Unhovered();
	
	UFUNCTION(BlueprintCallable, Category = "Card|Functions")
	void AnimationFade();

	UFUNCTION(BlueprintPure, Category = "Card|Functions")
	bool HasFocusCard() const; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BtnClick;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Cover;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundFavorite;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* FrameFavorite;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundMain;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* FrameMain;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundSelected;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* FrameSelected;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* PlayerIcon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Favorite;

	UPROPERTY()
	UAnimationUI* AnimationFrame;
	UPROPERTY()
	UAnimationUI* AnimationCard;
	UPROPERTY()
	UAnimationUI* AnimationFavorite;
	UPROPERTY()
	UAnimationUI* AnimationFadeCard;

private:

	UPROPERTY()
	bool bFocus;

	UPROPERTY()
	bool bFavorite;

	UPROPERTY()
	bool bMouseMove;

};

