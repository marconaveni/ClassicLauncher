// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "MusicInterface.h"
#include "UI/BaseUserWidget.h"
#include "LoopScrollBox.generated.h"

USTRUCT(BlueprintType)
struct FScrollConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta= (UIMin = "1", UIMax = "13"))
	int32 MaxPositionOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , meta= ( UIMin = "0", UIMax = "13"))
	int32 StartIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta= ( UIMin = "8", UIMax = "16"))
	int32 NumberCards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta= ( UIMin = "1", UIMax = "1000") )
	int32 CardSize;
	
	FScrollConfiguration()
		: MaxPositionOffset(4)
		  , StartIndex(5)
	      , NumberCards(16)
	      , CardSize(385)
	{
	}

	void ClampValues()
	{
		if(StartIndex < 0 )
		{
			StartIndex = 0;
		}
		if(MaxPositionOffset < 1)
		{
			MaxPositionOffset = 1;
		}
		if(MaxPositionOffset > NumberCards - 2)
		{
			MaxPositionOffset = NumberCards - 2;
		}
		if(StartIndex + MaxPositionOffset > NumberCards - 3 )
		{
			MaxPositionOffset = 1;
			StartIndex = NumberCards - 3;
		} 
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateCard, int32, Index);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateOnCardClick);

class UMainInterface;

/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API ULoopScrollBox : public UUserWidget, public  IMusicInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Configurations" )
	FScrollConfiguration ScrollConfiguration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UCard* CurrentCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UCard* LastCard;
	
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateCard OnCardIndex;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateOnCardClick OnCardClick;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	TArray<UCard*> CardReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	TArray<UCover*> CoverReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	UMainInterface* MainInterfaceReference;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<UCard> CardClassReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<UCover> CoverClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	int32 IndexScroll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	int32 ChildrenCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	EButtonsGame InputDirection;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bIgnoreOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	int32 PositionOffsetFocus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	int32 IndexFocusCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bUnlockInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTexture2D* ImageCardDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	USoundBase* SoundNavigate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
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
	void SetCardValues(UCard* Card, UPARAM(ref) FGameData& GameData, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void CardsDefault();
	
	UFUNCTION()
	void ConstructCover();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void ConstructCards();

	UFUNCTION()
	void Clear();

	void NewDirectionInput(int32 NewIndex);
	
	bool CheckFocus() const;

public:

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetCardFavorite(const bool ToggleFavorite);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void OpenCard();

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void DirectionRight(bool bIgnoreOffsetScroll = false);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void DirectionLeft(bool bIgnoreOffsetScroll = false);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetFocusCover();

	UFUNCTION(BlueprintPure, Category = "LoopScrollBox|Functions")
	FIndexPositions GetScrollOffSet() const;

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void OnReleaseCard(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void OnHoveredCard(int32 Index);

	UFUNCTION(BlueprintImplementableEvent, Category = "LoopScrollBox|Functions")
	void OnHoveredOnCard(const int32& Index);
	
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void OnUnhoveredCard(int32 Index);

	UFUNCTION(BlueprintImplementableEvent, Category = "LoopScrollBox|Functions")
	void OnUnhoveredOnCard(const int32& Index);
	
	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;

};
