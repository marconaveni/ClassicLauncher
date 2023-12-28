// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "MusicInterface.h"
#include "UI/BaseUserWidget.h"
#include "LoopScrollBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateCard, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateIndexStart, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateIndexFinal, int32, Index);

class UMainInterface;

/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API ULoopScrollBox : public UUserWidget, public  IMusicInterface
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateCard GetCardIndex;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateIndexStart StartIndexToFinal;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateIndexFinal FinalIndexToStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<UCard*> CardReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	TArray<UCover*> CoverReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	UMainInterface* MainInterfaceReference;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Subclass")
	TSubclassOf<UCard> CardClassReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Subclass")
	TSubclassOf<UCover> CoverClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	int32 IndexScroll;

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
	bool bUnlockInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	UTexture2D* ImageCardDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	USoundBase* SoundNavigate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoopScrollBox|Variables")
	USoundBase* SoundSelect;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBoxBottom;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasCards;

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BtnClick;

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void PrepareScrollBox();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDirectionLeft();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDirectionRight();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPrepareScrollBox();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenCard();

	UFUNCTION(BlueprintPure, Category = "LoopScrollBox|Functions")
	void GetCardReference(UPARAM(DisplayName = "Card Reference") UCard*& CardRef, const int32 Index, const int32 StartIndex = 5);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions", meta = (DisplayName = "Set Z-Order Card"))
	void SetZOrderCard();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void AddCardsHorizontalBox(TArray<FGameData> GameData, int32 IndexFocus);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetCardValues(UCard* Card, UPARAM(ref) FGameData& GameData);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void CardsDefault();
	
	UFUNCTION()
	void ConstructCover();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void ConstructCards();

	UFUNCTION()
	void Clear();

public:

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetCardFavorite(const bool ToggleFavorite);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void OpenCard();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void DirectionRight();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void DirectionLeft();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetFocusCover();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetCenterFocus() const;

	UFUNCTION(BlueprintPure, Category = "LoopScrollBox|Functions")
	FIndexPositions GetScrollOffSet() const;

	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;

};
