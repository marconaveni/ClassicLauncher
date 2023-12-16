// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassicFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "GameData.h"
#include "UI/BaseUserWidget.h"
#include "MainInterface.generated.h"



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

	FMultiInput() { SetAllNoneInput(); }

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
class CLASSICLAUNCHER_API UMainInterface : public UBaseUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextImageBlock* TextTitleGame;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgBottomMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImgImageBottom;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImgVideo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgTopBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgTopBarIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundTopBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundBottomBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundVideo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* StartSystemBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BgBottom;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelSystemSelect;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelConfiguration;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanelBottom;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* WBPToolTipConfiguration;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* WBPToolTipFavorites;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* WBPToolTipInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolTip* WBPToolTipSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicInfoInterface* WBPInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicSystemListInterface* WBPSystemsList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicConfigurations* WBPClassicConfigurationsInterface;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButtonsIcons* WBPButtonsIconsInterfaces;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UFrame* WBPFrame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* BtnSelectSystem;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* BtnConfigurations;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* BtnFavorites;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UClassicButton* BtnInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMessageBalloon* MessageDisplay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	ULoopScrollBox* LoopScroll;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBoxScroll* WBPTextBoxScroll;

	//animations WidgetBind

public:

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowDescBottomInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowSystem;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationShowConfiguration;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BarTop;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeStartSystem;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* LoadListGame;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* VideoAnimation;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeChangeImageToVideo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ChangeVideoToImage;

	//subclass and references
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<UClassicButtonSystem> ButtonSystemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TArray<UClassicButtonSystem*> ButtonSystemReferences;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AClassicMediaPlayer* ClassicMediaPlayerReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	USoundBase* SoundSelect;
	
public:

	UMainInterface(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

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
	int32 ProcessID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationLastButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	FMultiInput MultiInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EPositionY PositionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EFocus Focus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	FKeyEvent KeyEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bDelayPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bKeyPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bUpDownPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bInputEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bDelayFavoriteClick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bDelayQuit;

	UPROPERTY()
	float Alpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float SpeedScroll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float InitialSpeedScroll = 0.18f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float TargetSpeedScroll = 0.125f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float FastSpeedScroll = 0.085f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountLocationY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	UTexture2D* ImageBottomDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	UTexture2D* ImageNull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Components")
	UClassicGameInstance* ClassicGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	AClassicGameMode* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables|Debug")
	bool bDebug = false;

	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipSystem;
	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipConfiguration;
	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipFavorites;
	UPROPERTY()
	UCanvasPanelSlot* SlotToolTipInfo;

	//timers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayRunAppTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayReloadTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayLoadListTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayFavoriteTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayQuitTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle TriggerTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle LauncherTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle BackButtonTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle InitializeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle StartVideoTimerHandle;

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

	UFUNCTION()
	void EnableButtonsTop() const;

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadImages(const int32 DistanceIndex = 15);

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
	void NavigationGame(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationMain(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationSystem(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationInfo(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void NavigationConfiguration(EButtonsGame Navigate);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnNavigationGame(EButtonsGame Navigate);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void StartVideo();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetDirection(EButtonsGame Navigate);

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
	void SetRenderOpacityList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ResetCards(const bool bAnimationBarTop, const bool bAnimationShowSystem);

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


	/**
	* Function to avoid losing Focus on Widget .
	*/
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnPreventLoseFocus();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetTopButtonFocus();

	//bindbuttons
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


	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickFavorite();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetVisibiltyDebugButton(UButton* Button);

public:

	UFUNCTION()
	void SetZOrderToolTips(UCanvasPanelSlot* ToolTipSlot) const;

	UFUNCTION()
	void SetVisibilityToolTips(UToolTip* ToolTip = nullptr) const;

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetImageBottom();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ChangeVisibilityImageVideo();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CloseMenus();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CloseBackMenu();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ShowMessage(const FText Message,const float InRate);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetLastPositions(bool bResetPositions) const;

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetFrame();
};
