// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassicFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "GameData.h"
#include "MainInterface.generated.h"



UENUM(BlueprintType, Category = "Navigation")
enum class EPositionY : uint8
{
	TOP        UMETA(DisplayName = "Top"),
	CENTRAL    UMETA(DisplayName = "Center"),
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



/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UMainInterface : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollListGame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtTitleGame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UMultiLineEditableTextBox* TxtDescription;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollDescription;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBottomMenu;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImgImageBottom;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImgVideo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgTopBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgTopBarIcon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBackground3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgTitle;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImgPreventClickMouse;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgBottom;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasPanelSystemSelect;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasPanelInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasPanelConfiguration;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasPanelBottom;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UArrow* WBPArrow;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UToolTip* WBPToolTipConfiguration;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UToolTip* WBPToolTipFavorites;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UToolTip* WBPToolTipInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UToolTip* WBPToolTipSystem;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicInfoInterface* WBPInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicSystemListInterface* WBPSystemsList;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicConfigurations* WBPClassicConfigurationsInterface;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButtonsIcons* WBPButtonsIconsInterfaces;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UFrame* WBPFrame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButton* BtnSelectSystem;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButton* BtnConfigurations;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButton* BtnFavorites;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButton* BtnInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UMessageBalloon* MessageDisplay;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MessageCenter;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class ULoopScrollBox* LoopScroll;

	//animations WidgetBind

protected:

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ShowDescBottomInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ShowSystem;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimationShowConfiguration;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* BarTop;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FadeStartSystem;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ShowInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* LoadListGame;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* VideoAnimation;


	//subclass and references

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class UCover> CoverClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class UClassicButtonSystem> ButtonSystemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TArray<class UCover*> CoverReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TArray<class UClassicButtonSystem*> ButtonSystemReferences;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class AClassicMediaPlayer* ClassicMediaPlayerReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class AClassicLibretroTV* ClassicLibretroTVReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class USoundBase* SoundSelect;

public:

	UMainInterface(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnAnimationStartedPlaying(UUMGSequencePlayer& Player) override;
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	TArray<FGameData> GameData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	TArray<FGameData> NewGameData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	TArray<FConfigSystem> GameSystems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	FConfig ConfigurationData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 IndexCard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 FirstIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 LastIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 IndexAsyncImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 ProcessID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 PositionCenterX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 PositionTopX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationLastButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationScroll;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationBack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationLB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationRB;
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
	bool bKeyTriggerLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bKeyTriggerRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bInputEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bFilterFavorites;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bDelayFavoriteClick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bDelayQuit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	bool bHover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float TimerDelayAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float TriggerDelayPressed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float DescriptionScrollScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables", meta = (ClampMin = "1", UIMin = "1", UIMax = "100", SliderExponent = 1) )
	float MultiplySpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainInterface|Variables");
	float Multiply;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float SpeedScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float FrameSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float DefaultFrameSpeed = 1.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float DefaultSpeedScroll = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountLocationY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	TArray<UTexture2D*> ImageCards;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	UTexture2D* ImageCard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	UTexture2D* ImageNull;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	UTexture2D* ImageFrameTop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	UTexture2D* ImageFrameCenter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Components")
	class UClassicGameInstance* ClassicGameInstance;

	//timers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayCreateGameListTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayReloadTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayLoadListTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle DelayPressedTimerHandle;
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
	FTimerHandle SetArrowsTimerHandle;

	//Functions

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void RestartWidget();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetCenterText(const FText Message);
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetCenterText(const FText& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadConfiguration();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "On Load Configuration")
	void OnLoadConfiguration();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadConfigSystems();
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadConfigSystems();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadGamesList();
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadGamesList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ShowGames();
	UFUNCTION(BlueprintImplementableEvent)
	void OnShowGames();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void PrepareThemes();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPrepareThemes();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetPaddingCovers();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateNewGameList();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCreateNewGameList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void GameSettingsInit();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void GameSettingsRunning();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void GameSettingsRunningInternal();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateCoversWidget(int32 Min, int32 Max);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void AddCoverWidget(FGameData Data);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void AddSystems();

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
	void SetDirection(EButtonsGame Navigate,float Speed);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusTop();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusUpBottom();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusDownBottom();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickLaunch();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void AppLaunch();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void OpenLibretro(FString CorePath ,FString RomPath, bool CanUnzip);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void OpenExternalProcess( FString ExecutablePath, TArray<FString> CommandArgs);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickSystem(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetTitle(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetButtonsIconInterfaces(EPositionY GetPosition);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ChangeCoversVisibilitys(int32 Size);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ChangeCoverVisibility();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetImagesCover(UTexture2D* Texture,UCover* Cover, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Save")
	void SetCountPlayerToSave();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Save")
	void SetFavoriteToSave();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Save")
	bool SaveGameListXML(FString& GameListPath, TArray<FGameData>& NewGameDatas);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Save")
	bool SaveGameList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void RunningGame(bool IsRun);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void PressedDelayNavigation(float Delay);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void PressedTimerNavigation();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetRenderOpacityList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ResetCards(bool bAnimationBarTop, bool bAnimationShowSystem);

	UFUNCTION()
	void Clear();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoopPauseAsyncImage();
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoopResumeAsyncImage();


private:

	UPROPERTY()
	TArray<FText> TextTop;
	UPROPERTY()
	TArray<FText> TextCenter;
	UPROPERTY()
	TArray<ESlateVisibility> IconTop;
	UPROPERTY()
	TArray<ESlateVisibility> IconCenter;



	//bindbuttons
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnFocusSelectSystem();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnFocusConfigurations();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnFocusFavorites();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnFocusInfo();

	void SetToolTip(UToolTip* ToolTip);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnLostFocusSelectSystem();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnLostFocusConfigurations();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnLostFocusFavorites();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnLostFocusInfo();

	/**
	* Function to avoid losing Focus on Widget .
	*/
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnPreventLoseFocus();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetTopButtonFocus();

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
	void QuitGame();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnClickFavorite();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateFolders();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetScrollDescription(EButtonsGame Scroll);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetVisibiltyDebugButton(UButton* button);

public:

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetImageBottom();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CloseMenus();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CloseBackMenu();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ShowMessage(FText Message, float InRate);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetArrows();
};
