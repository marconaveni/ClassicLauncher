// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "LoopScrollBox.h"

#include "Arrow.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "Card.h"
#include "Cover.h"
#include "MainInterface.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Scrollbox.h"


void ULoopScrollBox::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void ULoopScrollBox::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply ULoopScrollBox::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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

FReply ULoopScrollBox::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	InputDirection = EButtonsGame::NONE;
	InputNavigation = EButtonsGame::NONE;
	if (UClassicFunctionLibrary::GetInputButton(InKeyEvent) == EButtonsGame::A && HasAnyUserFocus())
	{
		OnCardClick.Broadcast();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

FReply ULoopScrollBox::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply ULoopScrollBox::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void ULoopScrollBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ScrollConfiguration.ClampValues();
	SetIsFocusable(true);
	bIgnoreOffset = false;
	ConstructCards();
}

void ULoopScrollBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void ULoopScrollBox::Clear()
{
	Speed = 30.0f;
	ChildrenCount = 0;
	InputDirection = EButtonsGame::NONE;
	PositionOffsetFocus = 1;
	IndexFocusCard = 0;
	SetUnlockInput(false);
}

void ULoopScrollBox::PrepareScrollBox()
{
	if (MainInterfaceReference != nullptr)
	{
		MainInterfaceReference->IndexCard = IndexFocusCard;
	}

	const ESlateVisibility VisibilityCard = (ChildrenCount > ScrollConfiguration.MinimumInfinityCard) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	for (UCard* Card : CardReference)
	{
		Card->SetVisibility(VisibilityCard);
	}
	CardsDefault();
	OnPrepareScrollBox();
}

void ULoopScrollBox::OpenCard()
{
	OnOpenCard();
}

void ULoopScrollBox::SetFocusCover()
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

UCard* ULoopScrollBox::GetCardReference(const int32 Index)
{
	if (CardReference.IsValidIndex(Index + ScrollConfiguration.StartIndex))
	{
		return CardReference[Index + ScrollConfiguration.StartIndex];
	}
	return nullptr;
}

void ULoopScrollBox::SetCardFavorite(const bool ToggleFavorite)
{
	const int32 Position = PositionOffsetFocus + 5;
	if (CardReference.IsValidIndex(Position))
	{
		CardReference[Position]->SetFavorite(ToggleFavorite, true);
	}
}

void ULoopScrollBox::SetZOrderCard()
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

void ULoopScrollBox::AddCardsHorizontalBox(TArray<FGameData> GameData, int32 IndexFocus)
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

void ULoopScrollBox::SetCardValues(UCard* Card, FGameData& GameData, const int32 Index)
{
	if (Card != nullptr)
	{
		Card->IndexGameData = Index;
		Card->MapIndexGameData = GameData.MapIndex;
		Card->PathImage = GameData.imageFormated;
		Card->SetPlayers(GameData.players);
		Card->SetFavorite(GameData.favorite, false);
	}
}

void ULoopScrollBox::CardsDefault()
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

void ULoopScrollBox::ConstructCover()
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

void ULoopScrollBox::ConstructCards()
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
		Card->OnReleaseTrigger.AddUniqueDynamic(this, &ULoopScrollBox::OnReleaseCard);
		Card->OnHoveredTrigger.AddUniqueDynamic(this, &ULoopScrollBox::OnHoveredCard);
		Card->OnUnhoveredTrigger.AddUniqueDynamic(this, &ULoopScrollBox::OnUnhoveredCard);
		CardReference.Add(Card);
		CanvasCards->AddChild(Card);
		UCanvasPanelSlot* CanvasCard = Cast<UCanvasPanelSlot>(Card->Slot);
		const int32 Position = i * ScrollConfiguration.CardSize;
		CanvasCard->SetAutoSize(true);
		CanvasCard->SetPosition(FVector2D(Position, 0));
	}
}

FIndexPositions ULoopScrollBox::GetScrollOffSet() const
{
	FIndexPositions Position;
	Position.LastIndexFocus = IndexFocusCard;
	Position.LastIndexOffSet = PositionOffsetFocus;
	return Position;
}

void ULoopScrollBox::NewDirectionInput(const int32 NewIndex)
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

bool ULoopScrollBox::CheckFocus() const
{
	if (MainInterfaceReference == nullptr) return true;
	return !MainInterfaceReference->GetInputEnable() ||
		MainInterfaceReference->PositionY == EPositionY::BOTTOM ||
		MainInterfaceReference->Focus != EFocus::MAIN;
}

bool ULoopScrollBox::IndexFocusRange(int32 Index, int32& NewIndex) const
{
	if (CheckFocus()) return false;

	NewIndex = Index - ScrollConfiguration.StartIndex;
	const int32 StartIndex = ScrollConfiguration.StartIndex;
	const int32 MaxPositionOffset = StartIndex + ScrollConfiguration.MaxPositionOffset;
	return (Index > StartIndex && Index <= MaxPositionOffset && GetUnlockInput() && CanvasCards->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
}

void ULoopScrollBox::OnReleaseCard(int32 Index)
{
	int32 NewIndex = 0;
	if (IndexFocusRange(Index, NewIndex))
	{
		MainInterfaceReference->OnClickLaunch();
	}
	else
	{
		NewDirectionInput(NewIndex);
	}
}

void ULoopScrollBox::OnHoveredCard(int32 Index)
{
	int32 NewIndex = 0;
	if (IndexFocusRange(Index, NewIndex))
	{
		Speed = 0.001f;
		if (MainInterfaceReference->PositionY == EPositionY::TOP)
		{
			MainInterfaceReference->SetNavigationFocusDownBottom();
		}
		else if (MainInterfaceReference->PositionY == EPositionY::BOTTOM)
		{
			MainInterfaceReference->SetNavigationFocusUpBottom();
		}
		else if(MainInterfaceReference->PositionY == EPositionY::CENTER && MainInterfaceReference->GetInputEnable())
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

void ULoopScrollBox::OnUnhoveredCard(int32 Index)
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

void ULoopScrollBox::DirectionRight(const bool bIgnoreOffsetScroll)
{
	if(!GetUnlockInput()) return;
	SetUnlockInput(false);
	InputDirection = EButtonsGame::RIGHT;
	bIgnoreOffset = bIgnoreOffsetScroll;
	OnDirectionRight();
}

void ULoopScrollBox::DirectionLeft(const bool bIgnoreOffsetScroll)
{
	if(!GetUnlockInput()) return;
	SetUnlockInput(false);
	InputDirection = EButtonsGame::LEFT;
	bIgnoreOffset = bIgnoreOffsetScroll;
	OnDirectionLeft();
}

FScrollConfiguration ULoopScrollBox::GetScrollConfiguration() const
{
	return ScrollConfiguration;
}

int32 ULoopScrollBox::GetInitialScrollPosition() const
{
	const int32 Value = FMath::Clamp(PositionOffsetFocus - 1, 0, GetScrollConfiguration().MaxPositionOffset - 1);
	return Value * GetScrollConfiguration().CardSize;
}

int32 ULoopScrollBox::GetIndexToCount() const
{
	const int32 PositionOffsetDirection = (Direction == -1) ? PositionOffsetFocus + 1 : PositionOffsetFocus - 1;
	const int32 MinValue = (bIgnoreOffset) ? PositionOffsetDirection : 1;
	const int32 MaxValue = (bIgnoreOffset) ? PositionOffsetDirection : ScrollConfiguration.MaxPositionOffset;
	const int32 Result = ScrollConfiguration.StartIndex + FMath::Clamp(PositionOffsetFocus, MinValue, MaxValue);
	return  (IndexFocusCard - Result < 0) ? CoverReference.Num() - FMath::Abs(IndexFocusCard - Result) : IndexFocusCard - Result;
}

void ULoopScrollBox::SetUnlockInput(const bool bEnable)
{
	bUnlockInput = bEnable;
	const ESlateVisibility SlateVisibility = (bEnable) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::HitTestInvisible;
	SetVisibility(SlateVisibility);
}

bool ULoopScrollBox::GetUnlockInput() const
{
	return bUnlockInput;
}

void ULoopScrollBox::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundNavigate = NavigateSound;
	SoundSelect = SelectSound;
}
