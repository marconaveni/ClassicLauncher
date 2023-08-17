// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "LoopScrollBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateCard, int32, Index);
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API ULoopScrollBox : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateCard GetCardIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Subclass")
	TSubclassOf<class UCard> CardClassReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<class UCard*> CardReferenceLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<class UCard*> CardReferenceCenter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<class UCard*> CardReferenceRight;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	float Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	float Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 IndexScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 ChildrenCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	EButtonsGame InputDirection;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 PositionOffsetFocus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 IndexFocusCard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	bool UnlockInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables|Debug")
	bool Debug = false;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HorizontalBoxLeft;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HorizontalBoxCenter;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HorizontalBoxRight;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* SizeStrech;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BtnClick;

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void ScrollMovement();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetAnimatedScrollOffset();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void DirectionRight();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void DirectionLeft();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void PrepareScrollBox();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void StartScrollTo(EButtonsGame LeftRight);
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void CancelScroll();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SelectDirectionScroll();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void OpenCard();

	UFUNCTION(BlueprintImplementableEvent)
	void OnScrollMovement();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetAnimatedScrollOffset();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDirectionLeft();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDirectionRight();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPrepareScrollBox();
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartScrollTo(const EButtonsGame& LeftRight);
	UFUNCTION(BlueprintImplementableEvent)
	void OnCancelScroll();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectDirectionScroll();
	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenCard();

	UFUNCTION(BlueprintPure, Category = "LoopScrollBox|Functions")
	void GetCardReferences(int32 Index, class UCard*& Left, class UCard*& Center);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void AddCardsHorizontalBox( TArray<FGameData> GameData, int32 IndexFocus);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void AddImagesCards(UTexture2D* NewTexture, int32 Width, int32 Height,int32 Index);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetValuesCard(class UCard* Card, FString Players ,bool Favorite);
	
	void OnClickButton();
};
