// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "MusicInterface.h"
#include "LoopScrollBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateCard, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateIndexStart, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateIndexFinal, int32, Index);

class UCard;
class UCover;
class UScrollBox;
class UHorizontalBox;
class UButton;

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API ULoopScrollBox : public UUserWidget, public  IMusicInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateCard GetCardIndex;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateIndexStart StartIndexToFinal;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateIndexFinal FinalIndexToStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Subclass")
	TSubclassOf<UCard> CardClassReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Subclass")
	TSubclassOf<UCover> CoverClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<UCard*> CardReferenceLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<UCard*> CardReferenceCenter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<UCard*> CardReferenceRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<UCover*> CoverReference;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	float Offset;
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
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 PositionOffsetFocus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 IndexFocusCard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	bool UnlockInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	UTexture2D* ImageCardDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables|Debug")
	bool Debug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	USoundBase* SoundNavigate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	USoundBase* SoundSelect;


	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//class UScrollBox* ScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBoxBottom;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxMain;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxLeft;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxCenter;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxRight;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BtnClick;

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
	void OpenCard();
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SelectDirectionScroll();

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
	void GetCardReference(int32 Index, UPARAM(DisplayName = "Card Reference") UCard*& CardRef);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions") 
	void SetCardFavorite(const bool ToggleFavorite);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void AddCardsHorizontalBox( TArray<FGameData> GameData, int32 IndexFocus);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetCardValues(UCard* Card, UPARAM(ref) FGameData& GameData);

	UFUNCTION()
	void ConstructCover();

	UFUNCTION(BlueprintPure, Category = "LoopScrollBox|Functions")
	FIndexPositions GetScrollOffSet();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void AddImagesCards(UTexture2D* NewTexture, int32 Width, int32 Height,int32 Index);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetFocusCard(bool Enable);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetFocusCover();

	UFUNCTION()
	void OnClickButton();

	UFUNCTION()
	void Clear();

public:

	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;
};
