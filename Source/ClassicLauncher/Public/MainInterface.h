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
	class UHorizontalBox* HBListGame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollListGame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtTitleGame;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TxtDebug;
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
	class UImage* BgBackground;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BgTitle;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImgFrame;
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
	class UClassicButton* BtnSelectSystem;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButton* BtnConfigurations;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButton* BtnFavorites;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UClassicButton* BtnInfo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UMessageBalloon* MessageDisplay;

	//animations WidgetBind

protected:

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationX1;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationX2;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationX3;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToSystems;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToConfig;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToFavorite;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY1ToInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY2ToInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY3ToInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationY4ToInfo;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationXTop1;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationXTop2;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FrameAnimationXTop3;
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
	TSubclassOf<class UCard> cardClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class UCover> coverClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	TSubclassOf<class UClassicButtonSystem> buttonSystemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TArray<class UCard*> cardReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TArray<class UCover*> coverReference;
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
	EButtonsGame ENavigationScroll;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationBack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	EButtonsGame ENavigationA;
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
	bool bHover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float TimerDelayAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float TriggerDelayPressed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float SpeedScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	float DescriptionScrollScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountLocationY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 MaxFrameMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Variables")
	int32 CountGarbageCollection;
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
	FTimerHandle TickTimerHandle;
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
	FTimerHandle TriggerTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle LauncherTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainInterface|Timers")
	FTimerHandle BackButtonTimerHandle;

	//Functions

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void TimerTick();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void TriggerTick();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void RestartWidget();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void OnErrorMessage(const FString& ErrorMessage);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetDebugMessage(const FString Message);
	UFUNCTION(BlueprintImplementableEvent)
	void OnError(const FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadConfigurationNative();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Teste")
	void LoadConfiguration();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadConfigSystemsNative();
	UFUNCTION(BlueprintImplementableEvent)
	void LoadConfigSystems();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadListNative();
	UFUNCTION(BlueprintImplementableEvent)
	void LoadList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ViewList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetPaddingCovers();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateGameListNative();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateGameList();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void GameSettingsInit();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void GameSettingsRunning();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void GameSettingsRunningInternal();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateCardsCoversWidget(int32 Min, int32 Max);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateCardCoverWidget(FGameData Data);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateGameSystems();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNativeNavigationGame(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNativeNavigationMain(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNativeNavigationSystem(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNativeNavigationInfo(EButtonsGame Navigate);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNativeNavigationConfiguration(EButtonsGame Navigate);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnNavigationGame(EButtonsGame Navigate);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusTop();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusMain();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusUpBottom();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void SetNavigationFocusDownBottom();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetFocusCardToLeft();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetFocusCardToRight();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNativeClick(FString RomPath);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ClassicLaunch();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void OpenLibretro(FString CorePath ,FString RomPath, bool CanUnzip);
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void OpenExternalProcess( FString ExecutablePath, TArray<FString> CommandArgs);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNativeClickSystem(int32 Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnClickPathEvent(const FString& ValuePath);

	UFUNCTION(BlueprintImplementableEvent)
	void OnClickSystem(const int32 Value);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Events")
	void OnNavigationFocus(UCard* Card);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetButtonsIconInterfaces(EPositionY GetPosition);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions", meta = (count = "150.0"))
	void ForceGarbageCollectionBP(float Count = 150.0f);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ImageOut(UTexture2D* TextureOut, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ASyncLoadCard(FString PathImage, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadFirstImages();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void LoadImages();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetImagesCard(UTexture2D* Texture, UCard* Card, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetImagesCover(UTexture2D* Texture,UCover* Cover, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void AddImagesCardCover(UTexture2D* Texture, int32 Index);

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

	UFUNCTION(BlueprintImplementableEvent)
	void LoadImageAsync(const int32& Index);
	UFUNCTION(BlueprintImplementableEvent)
	void LoadImageSync(const int32& Index);

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
	void OnLoopStartAsyncImage(const int32& Index);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoopPauseAsyncImage();
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoopResumeAsyncImage();

	//Animation

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Animations")
	void AnimationFrameMoveRight();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Animations")
	void AnimationFrameMoveLeft();
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Animations")
	void AnimationFrameToTop(UWidgetAnimation* Animation1, UWidgetAnimation* Animation2, UWidgetAnimation* Animation3, UWidgetAnimation* Animation4, bool Reverse);
	UFUNCTION(BlueprintImplementableEvent)
	void FrameMoveRight();
	UFUNCTION(BlueprintImplementableEvent)
	void FrameMoveLeft();

private:

	UPROPERTY()
	TArray<FString> TextTop;
	UPROPERTY()
	TArray<FString> TextCenter;
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
	void OnClickFavorite();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CreateFolders();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetScrollDescription(EButtonsGame Scroll);

	//macros ticks
	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ScrollCards();

public:

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void SetImageBottom();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CloseMenus();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void CloseBackMenu();

	UFUNCTION(BlueprintCallable, Category = "MainInterface|Functions")
	void ShowMessage(FString Message, float InRate);

};
