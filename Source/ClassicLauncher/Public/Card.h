// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnimationUI.h"
#include "Card.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate, FString , value);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateTrigger, EUINavigation, value, UCard*, selfCard);
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

	//UPROPERTY(BlueprintAssignable, BlueprintCallable)
	//FDelegateTrigger OnNavigate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegate OnClickTrigger;


	UCard(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual bool Initialize() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	FAnimationUICurves CurveFavoritesFoward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	FAnimationUICurves CurveFavoritesReverse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	FString PathImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	int32 MapIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Variables")
	TArray<FSlateBrush> PlayersImage;

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetPath(FString value);
	
	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetPlayers(FString NumberPlayers);

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetFocusCard(bool bEnable, bool bAnimate = true, bool bReset = false, float TimeAnimation = 0.2f);


	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetThemeCard(UTexture2D* texture);

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void SetFavorite(bool bEnable, bool bAnimateIcon);

	UFUNCTION(BlueprintCallable, Category = "Card|Functions")
	void SetCardImage(UTexture2D* texture, int32 width, int32 height);

	UFUNCTION(BlueprintCallable,  Category = "Card|Functions")
	void ButtonClick();

	UFUNCTION(BlueprintCallable, Category = "Card|Functions")
	void AnimationFade();

	UFUNCTION(BlueprintPure, Category = "Card|Functions")
	bool HasFocusCard() const; 

	UFUNCTION(BlueprintImplementableEvent)
	void ClickButton();

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

};

