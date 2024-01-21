// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Data/GameData.h"
#include "UI/BaseUserWidget.h"
#include "MainScreen.generated.h"



UENUM(BlueprintType, Category = "Navigation")
enum class EPositionY : uint8
{
	TOP        UMETA(DisplayName = "Top"),
	CENTER    UMETA(DisplayName = "Center"),
	BOTTOM     UMETA(DisplayName = "Bottom")
};

UENUM(BlueprintType, Category = "Navigation")
enum class EFocus : uint8
{
	MAIN     UMETA(DisplayName = "Main"),
	SYSTEM   UMETA(DisplayName = "System"),
	INFO     UMETA(DisplayName = "Info"),
	CONFIG   UMETA(DisplayName = "Configuration")
};

USTRUCT(BlueprintType)
struct FMultiInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EButtonsGame ENavigation1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EButtonsGame ENavigation2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EButtonsGame ENavigation3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EButtonsGame ENavigation4;

	FMultiInput() {
		SetAllNoneInput();
	}

	FORCEINLINE void SetAllNoneInput()
	{
		ENavigation1 = EButtonsGame::NONE;
		ENavigation2 = EButtonsGame::NONE;
		ENavigation3 = EButtonsGame::NONE;
		ENavigation4 = EButtonsGame::NONE;
	}

	FORCEINLINE bool CheckInputPressed() const
	{
		return (ENavigation1 != EButtonsGame::NONE && ENavigation2 != EButtonsGame::NONE &&
			ENavigation3 != EButtonsGame::NONE && ENavigation4 != EButtonsGame::NONE);
	}

	FORCEINLINE void SetInput(const EButtonsGame Input)
	{
		switch (Input)
		{
		case EButtonsGame::SELECT: ENavigation1 = EButtonsGame::SELECT; break;
		case EButtonsGame::START: ENavigation2 = EButtonsGame::START; break;
		case EButtonsGame::LT: ENavigation3 = EButtonsGame::LT; break;
		case EButtonsGame::RT: ENavigation4 = EButtonsGame::RT; break;
		default: break;
		}
	}
};


/**
 *
 */
UCLASS()
class CLASSICLAUNCHER_API UMainScreen : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextTitleGame;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHeader* Header;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UFooter* Footer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UFooterDetails* FooterDetails;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* StartSystemBackground;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelRoot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelScreen;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelSystemSelect;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelConfiguration;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMoreInformationsLayout* WBPInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UGameListLayout* WBPSystemsList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOptionsLayout* Options;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonsPrompt* WBPButtonsIconsInterfaces;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UFrame* Frame;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTipsLayout* ToolTips;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMessageBalloon* MessageDisplay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	ULoopScroll* LoopScroll;
	
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeStartSystem;
	
	//subclass and references

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<UButtonCommon> ButtonSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TArray<UButtonCommon*> ButtonSystemReferences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AClassicMediaPlayer* ClassicMediaPlayerReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	USoundBase* SoundSelect;

protected:

	UMainScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativePressInput(const FKey& InKey) override;
	virtual void NativeReleaseInput(const FKey& InKey) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetReferences();
	
	//variables






	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	TArray<FGameData> GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	TArray<FGameData> GameDataIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	TArray<FGameSystem> GameSystems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	FConfig ConfigurationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 IndexCard;





	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EPositionY PositionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EFocus Focus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 ProcessID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	FMultiInput MultiInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bUpDownPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bDelayQuit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountLocationY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	UTexture2D* ImageNull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Components")
	UClassicGameInstance* ClassicGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	AClassicGameMode* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bDelayTooltip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables|Debug")
	bool bDebug = false;

	//timers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayRunAppTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayReloadTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayLoadListTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayQuitTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle TriggerTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle LauncherTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle BackButtonTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle LoadImagesTimerHandle;
	
	
	//TempVariables
	UPROPERTY(BlueprintReadOnly)
	int32 FirstIndex;

	UPROPERTY(BlueprintReadOnly)
	int32 LastIndex;

	//Functions
	UFUNCTION()
	void SteamRunApp();

	UFUNCTION()
	void ExternRunApp();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadGamesList();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadGamesList();
	

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadImages(const int32& Index, const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ShowGames();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShowGames();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void PrepareThemes();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPrepareThemes();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationGame(EButtonsGame Input);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationMain(EButtonsGame Input);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationSystem(EButtonsGame Input);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationInfo(EButtonsGame Input);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationConfiguration(EButtonsGame Input);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNavigationGame(EButtonsGame Input);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusUpBottom();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusDownBottom();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickLaunch();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void AppLaunch();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void OpenExternalProcess(FString ExecutablePath, TArray<FString> CommandArgs);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickSystem(int32 Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnClickOnSystem();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetTitle(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetButtonsIconInterfaces(EPositionY GetPosition);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Save")
	void SetCountPlayerToSave();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Save")
	void SetFavoriteToSave();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void RunningGame(bool bIsRun);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ResetCards();

	UFUNCTION()
	void Clear();

private:

	UPROPERTY()
	TArray<FText> TextTop;

	UPROPERTY()
	TArray<FText> TextCenter;

	UPROPERTY()
	TArray<FText> TextCenterSystem;

	UPROPERTY()
	TArray<ESlateVisibility> IconTop;

	UPROPERTY()
	TArray<ESlateVisibility> IconCenter;

	UPROPERTY()
	TArray<ESlateVisibility> IconCenterSystem;

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetHeaderButtonFocus();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickHeader(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnFocusHeader(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnLostFocusHeader(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickSelectSystem();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickConfigurations();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickFavorites();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickInfo();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickBackAction();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetVisibiltyDebugButton(UButton* Button);

public:

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CloseMenus();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ShowMessage(const FText Message, const float InRate);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetLastPositions(bool bResetPositions) const;

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetFrame();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetPlayAnimation(FName AnimationName);
	
	/*UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CardIndex(int32 CardIndex, EButtonsGame Input);*/
};
