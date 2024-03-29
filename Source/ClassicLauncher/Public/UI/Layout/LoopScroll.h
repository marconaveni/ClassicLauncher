// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Data/GameData.h"
#include "Interfaces/MusicInterface.h"
#include "UI/BaseUserWidget.h"
#include "LoopScroll.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta= ( UIMin = "4", UIMax = "13") )
	int32 MinimumInfinityCard;
	
	FScrollConfiguration()
		: MaxPositionOffset(4)
		  , StartIndex(5)
		  , NumberCards(16)
		  , CardSize(385)
	      , MinimumInfinityCard(4)
	{
	}

	void ClampValues()
	{
		StartIndex = FMath::Clamp(StartIndex, 0 , NumberCards - 3); 
		MaxPositionOffset = FMath::Clamp(MaxPositionOffset, 1 , NumberCards - StartIndex - 2); 
	}
};


/*DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateCard, int32, Index, EButtonsGame , Input);*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateOnCardClick);

class UMainScreen;

/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API ULoopScroll : public UUserWidget ,public IMusicInterface
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	float Alpha;

	UPROPERTY()
	FTimerHandle MouseScrollTimerHandle;

protected:

	UPROPERTY()
	class UDataManager* DataManager;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Configurations")
	float InitialSpeedScroll = 0.18f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Configurations")
	float TargetSpeedScroll = 0.125f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Configurations")
	float FastSpeedScroll = 0.085f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Configurations")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Configurations" )
	FScrollConfiguration ScrollConfiguration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UCard* CurrentCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UCard* LastCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	int32 Direction;
	
	virtual FReply NativeOnPreviewKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	
public:

	/*UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateCard OnCardIndex;*/

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDelegateOnCardClick OnCardClick;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	TArray<UCard*> CardReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	TArray<UCover*> CoverReference;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	UMainScreen* MainInterfaceReference;*/
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	EButtonsGame InputNavigation;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bIgnoreOffset;

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
	UCard* GetCardReference(const int32 Index);

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
	
	bool IndexFocusRange(int32 Index, int32& NewIndex) const;

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

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	FScrollConfiguration GetScrollConfiguration() const;
	
	UFUNCTION(BlueprintPure, Category = "LoopScrollBox|Functions")
	int32 GetInitialScrollPosition() const;

	UFUNCTION(BlueprintPure, Category = "LoopScrollBox|Functions")
	int32 GetIndexToCount() const;

	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	void SetUnlockInput(const bool bEnable);
	
	UFUNCTION(BlueprintCallable, Category = "LoopScrollBox|Functions")
	bool GetUnlockInput() const;

	virtual void EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound) override;

};
