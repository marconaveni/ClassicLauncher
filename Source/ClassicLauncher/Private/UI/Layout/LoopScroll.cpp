// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/LoopScroll.h"

#include "UI/Components/Arrow.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Data/GameData.h"
#include "UI/Components/Card.h"
#include "UI/Components/Cover.h"
#include "UI/Screens/MainScreen.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Scrollbox.h"
#include "Data/DataManager.h"


void ULoopScroll::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void ULoopScroll::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULoopScroll::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DataManager = GetWorld()->GetSubsystem<UDataManager>();
	ScrollConfiguration.ClampValues();
	SetIsFocusable(true);
	bIgnoreOffset = false;
	ConstructCards();
}

FReply ULoopScroll::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(HasAnyUserFocus())
	{
		const EButtonsGame Input = UClassicFunctionLibrary::GetInputButton(InKeyEvent);
		if(Input == EButtonsGame::UP || Input == EButtonsGame::DOWN ||
			Input == EButtonsGame::LEFT || Input == EButtonsGame::RIGHT ||
			Input == EButtonsGame::LB || Input == EButtonsGame::RB)
		{
			InputNavigation = Input;
		}
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply ULoopScroll::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	InputDirection = EButtonsGame::NONE;
	InputNavigation = EButtonsGame::NONE;
	if (UClassicFunctionLibrary::GetInputButton(InKeyEvent) == EButtonsGame::A && HasAnyUserFocus())
	{
		OnCardClick.Broadcast();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

FReply ULoopScroll::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply ULoopScroll::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float ScrollScale = InMouseEvent.GetWheelDelta();

	if(HasAnyUserFocus() && GetUnlockInput() && CanvasCards->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		if (ScrollScale > 0)
		{
			DirectionLeft(true);
		}
		else if (ScrollScale < 0)
		{
			DirectionRight(true);
		}
		/*if(ScrollScale != 0)
		{
			CanvasCards->SetVisibility(ESlateVisibility::HitTestInvisible);
			GetWorld()->GetTimerManager().ClearTimer(MouseScrollTimerHandle);
			MouseScrollTimerHandle.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(
				MouseScrollTimerHandle, [&]()
			   {
				   CanvasCards->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			   }
			   , 0.015f, false, 0.001f);
		}*/
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void ULoopScroll::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (InputNavigation == EButtonsGame::LEFT || InputNavigation == EButtonsGame::RIGHT)
	{
		Speed = UKismetMathLibrary::Ease(InitialSpeedScroll, TargetSpeedScroll, Alpha, EEasingFunc::EaseIn);
		Alpha = FMath::Clamp(Alpha + 0.24f * GetWorld()->GetDeltaSeconds(), 0, 1);
	}
	else if (InputNavigation == EButtonsGame::LB || InputNavigation == EButtonsGame::RB)
	{
		Speed = FastSpeedScroll;
	}
	else if (Speed != InitialSpeedScroll)
	{
		Alpha = 0;
		Speed = InitialSpeedScroll;
	}
}

void ULoopScroll::Clear()
{
	Speed = 30.0f;
	ChildrenCount = 0;
	InputDirection = EButtonsGame::NONE;
	PositionOffsetFocus = 1;
	IndexFocusCard = 0;
	SetUnlockInput(false);
}

void ULoopScroll::PrepareScrollBox()
{
	if (DataManager->GetMainScreenReference() != nullptr)
	{
		DataManager->IndexGameData = IndexFocusCard;
	}

	const ESlateVisibility VisibilityCard = (ChildrenCount > ScrollConfiguration.MinimumInfinityCard) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	for (UCard* Card : CardReference)
	{
		Card->SetVisibility(VisibilityCard);
	}
	CardsDefault();
	OnPrepareScrollBox();
}

void ULoopScroll::OpenCard()
{
	OnOpenCard();
}

void ULoopScroll::SetFocusCover()
{
	if (CoverReference.IsValidIndex(IndexFocusCard) && CoverReference.IsValidIndex(0) && CoverReference.Num() - 1)
	{
		CoverReference[0]->FocusCover(false);
		CoverReference[CoverReference.Num() - 1]->FocusCover(false);
		ScrollBoxBottom->ScrollWidgetIntoView(CoverReference[IndexFocusCard], false, EDescendantScrollDestination::Center, 0);
		CoverReference[IndexFocusCard]->FocusCover(true);
	}
	if (CoverReference.IsValidIndex(IndexFocusCard + 1))
	{
		CoverReference[IndexFocusCard + 1]->FocusCover(false);
	}
	if (CoverReference.IsValidIndex(IndexFocusCard - 1))
	{
		CoverReference[IndexFocusCard - 1]->FocusCover(false);
	}
}

UCard* ULoopScroll::GetCardReference(const int32 Index)
{
	if (CardReference.IsValidIndex(Index + ScrollConfiguration.StartIndex))
	{
		return CardReference[Index + ScrollConfiguration.StartIndex];
	}
	return nullptr;
}

void ULoopScroll::SetCardFavorite(const bool ToggleFavorite)
{
	const int32 Position = PositionOffsetFocus + 5;
	if (CardReference.IsValidIndex(Position))
	{
		CardReference[Position]->SetFavorite(ToggleFavorite, true);
	}
}

void ULoopScroll::SetZOrderCard()
{
	for (int32 i = 6; i < 10; i++)
	{
		if (!CardReference.IsValidIndex(i)) continue;

		UCanvasPanelSlot* SlotCard = Cast<UCanvasPanelSlot>(CardReference[i]->Slot);
		if (CardReference[i]->HasFocusCard())
		{
			SlotCard->SetZOrder(1);
			continue;
		}
		SlotCard->SetZOrder(0);
	}
}

void ULoopScroll::AddCardsHorizontalBox(TArray<FGameData> GameData, int32 IndexFocus)
{
	if (GameData.IsEmpty()) return;

	CoverReference.Empty();
	ScrollBoxBottom->ClearChildren();
	Clear();

	ChildrenCount = GameData.Num();
	IndexFocusCard = (ChildrenCount > 4) ? FMath::Clamp(IndexFocus, 0, ChildrenCount) : 0;

	for (int32 i = 0; i < ChildrenCount; i++)
	{
		ConstructCover();
	}

	PrepareScrollBox();
}

void ULoopScroll::SetCardValues(UCard* Card, FGameData& GameData, const int32 Index)
{
	if (Card != nullptr)
	{
		Card->IndexGameData = Index;
		Card->MapIndexGameData = GameData.MapIndex;
		Card->PathImage = GameData.ImageFormatted;
		Card->SetPlayers(GameData.Players);
		Card->SetFavorite(GameData.bFavorite, false);
	}
}

void ULoopScroll::CardsDefault()
{
	const int32 Start = ScrollConfiguration.StartIndex + 1;
	const int32 End = Start + ScrollConfiguration.MinimumInfinityCard;
	for (int32 i = 0; i < CardReference.Num(); i++)
	{
		if (!CardReference.IsValidIndex(i)) continue;

		UCard* Card = CardReference[i];
		Card->SetFocusCard(false, false);
		Card->SetRenderOpacity(1);
		Card->SetRenderTransform(FWidgetTransform());

		if(i >= Start && i < End)
		{
			if (i - Start >= ChildrenCount)
			{
				Card->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				Card->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void ULoopScroll::ConstructCover()
{
	if (CoverClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CoverClass is Empty"));
		return;
	}

	UCover* Cover = WidgetTree->ConstructWidget<UCover>(CoverClass);
	ScrollBoxBottom->AddChild(Cover);
	Cover->SetCoverImage(ImageCardDefault);
	CoverReference.Add(Cover);
}

void ULoopScroll::ConstructCards()
{
	if (CardClassReference == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CardClassReference is Empty"));
		return;
	}

	CardReference.Empty();
	CanvasCards->ClearChildren();

	for (int32 i = 0; i < ScrollConfiguration.NumberCards; i++)
	{
		UCard* Card = WidgetTree->ConstructWidget<UCard>(CardClassReference);
		Card->IndexCard = i;
		Card->OnReleaseTrigger.AddUniqueDynamic(this, &ULoopScroll::OnReleaseCard);
		Card->OnHoveredTrigger.AddUniqueDynamic(this, &ULoopScroll::OnHoveredCard);
		Card->OnUnhoveredTrigger.AddUniqueDynamic(this, &ULoopScroll::OnUnhoveredCard);
		CardReference.Add(Card);
		CanvasCards->AddChild(Card);
		UCanvasPanelSlot* CanvasCard = Cast<UCanvasPanelSlot>(Card->Slot);
		const int32 Position = i * ScrollConfiguration.CardSize;
		CanvasCard->SetAutoSize(true);
		CanvasCard->SetPosition(FVector2D(Position, 0));
	}
}

FIndexPositions ULoopScroll::GetScrollOffSet() const
{
	FIndexPositions Position;
	Position.LastIndexFocus = IndexFocusCard;
	Position.LastIndexOffSet = PositionOffsetFocus;
	return Position;
}

void ULoopScroll::NewDirectionInput(const int32 NewIndex)
{
	if (NewIndex < PositionOffsetFocus)
	{
		DirectionLeft();
	}
	else if (NewIndex > PositionOffsetFocus)
	{
		DirectionRight();
	}
}

bool ULoopScroll::CheckFocus() const
{
	if (DataManager->GetMainScreenReference() == nullptr) return true;
	return !DataManager->GetMainScreenReference()->GetInputEnable() ||
		DataManager->GetMainScreenReference()->PositionY == EPositionY::BOTTOM ||
		DataManager->GetMainScreenReference()->Focus != EFocus::MAIN;
}

bool ULoopScroll::IndexFocusRange(int32 Index, int32& NewIndex) const
{
	if (CheckFocus()) return false;

	NewIndex = Index - ScrollConfiguration.StartIndex;
	const int32 StartIndex = ScrollConfiguration.StartIndex;
	const int32 MaxPositionOffset = StartIndex + ScrollConfiguration.MaxPositionOffset;
	return (Index > StartIndex && Index <= MaxPositionOffset && GetUnlockInput() && CanvasCards->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
}

void ULoopScroll::OnReleaseCard(int32 Index)
{
	int32 NewIndex = 0;
	if (IndexFocusRange(Index, NewIndex))
	{
		DataManager->GetMainScreenReference()->OnClickLaunch();
	}
	else
	{
		NewDirectionInput(NewIndex);
	}
}

void ULoopScroll::OnHoveredCard(int32 Index)
{
	int32 NewIndex = 0;
	if (IndexFocusRange(Index, NewIndex))
	{
		Speed = 0.001f;
		if (DataManager->GetMainScreenReference()->PositionY == EPositionY::TOP)
		{
			DataManager->GetMainScreenReference()->SetNavigationFocusDownBottom();
		}
		else if (DataManager->GetMainScreenReference()->PositionY == EPositionY::BOTTOM)
		{
			DataManager->GetMainScreenReference()->SetNavigationFocusUpBottom();
		}
		else if(DataManager->GetMainScreenReference()->PositionY == EPositionY::CENTER && DataManager->GetMainScreenReference()->GetInputEnable())
		{
			SetFocus();
			NewDirectionInput(NewIndex);
		}
		if (CardReference.IsValidIndex(NewIndex))
		{
			CurrentCard = CardReference[NewIndex];
		}
	}

	OnHoveredOnCard(Index);
}

void ULoopScroll::OnUnhoveredCard(int32 Index)
{
	int32 NewIndex = 0;
	if (IndexFocusRange(Index, NewIndex))
	{
		if (CardReference.IsValidIndex(NewIndex))
		{
			LastCard = CardReference[NewIndex];
		}
	}
	OnUnhoveredOnCard(Index);
}

void ULoopScroll::DirectionRight(const bool bIgnoreOffsetScroll)
{
	if(!GetUnlockInput()) return;
	SetUnlockInput(false);
	InputDirection = EButtonsGame::RIGHT;
	bIgnoreOffset = bIgnoreOffsetScroll;
	OnDirectionRight();
}

void ULoopScroll::DirectionLeft(const bool bIgnoreOffsetScroll)
{
	if(!GetUnlockInput()) return;
	SetUnlockInput(false);
	InputDirection = EButtonsGame::LEFT;
	bIgnoreOffset = bIgnoreOffsetScroll;
	OnDirectionLeft();
}

FScrollConfiguration ULoopScroll::GetScrollConfiguration() const
{
	return ScrollConfiguration;
}

int32 ULoopScroll::GetInitialScrollPosition() const
{
	const int32 Value = FMath::Clamp(PositionOffsetFocus - 1, 0, GetScrollConfiguration().MaxPositionOffset - 1);
	return Value * GetScrollConfiguration().CardSize;
}

int32 ULoopScroll::GetIndexToCount() const
{
	const int32 PositionOffsetDirection = (Direction == -1) ? PositionOffsetFocus + 1 : PositionOffsetFocus - 1;
	const int32 MinValue = (bIgnoreOffset) ? PositionOffsetDirection : 1;
	const int32 MaxValue = (bIgnoreOffset) ? PositionOffsetDirection : ScrollConfiguration.MaxPositionOffset;
	const int32 Result = ScrollConfiguration.StartIndex + FMath::Clamp(PositionOffsetFocus, MinValue, MaxValue);
	return  (IndexFocusCard - Result < 0) ? CoverReference.Num() - FMath::Abs(IndexFocusCard - Result) : IndexFocusCard - Result;
}

void ULoopScroll::SetUnlockInput(const bool bEnable)
{
	bUnlockInput = bEnable;
	const ESlateVisibility SlateVisibility = (bEnable) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::HitTestInvisible;
	SetVisibility(SlateVisibility);
}

bool ULoopScroll::GetUnlockInput() const
{
	return bUnlockInput;
}

void ULoopScroll::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundNavigate = NavigateSound;
	SoundSelect = SelectSound;
}
