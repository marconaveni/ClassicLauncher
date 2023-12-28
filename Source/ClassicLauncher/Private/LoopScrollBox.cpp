// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoopScrollBox.h"

#include "Arrow.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "Card.h"
#include "Cover.h"
#include "MainInterface.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Scrollbox.h"


void ULoopScrollBox::NativeOnInitialized()
{
	//PrepareScrollBox();
	Super::NativeOnInitialized();
}

void ULoopScrollBox::Clear()
{
	Speed = 30.0f;
	ChildrenCount = 0;
	InputDirection = EButtonsGame::NONE;
	PositionOffsetFocus = 1;
	IndexFocusCard = 0;
	bUnlockInput = true;
}

void ULoopScrollBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULoopScrollBox::NativePreConstruct()
{
	Super::NativePreConstruct();
	ConstructCards();
}

void ULoopScrollBox::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULoopScrollBox::PrepareScrollBox()
{
	if (MainInterfaceReference != nullptr)
	{
		MainInterfaceReference->IndexCard = IndexFocusCard;
	}

	const ESlateVisibility VisibilityCard = (ChildrenCount > 4) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
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

void ULoopScrollBox::SetCenterFocus() const
{
	BtnClick->SetKeyboardFocus();
}

void ULoopScrollBox::GetCardReference(UCard*& CardRef, const int32 Index, const int32 StartIndex)
{
	if (CardReference.IsValidIndex(Index + StartIndex))
	{
		CardRef = CardReference[Index + StartIndex];
	}
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

void ULoopScrollBox::SetCardValues(UCard* Card, FGameData& GameData)
{
	if (Card != nullptr)
	{
		Card->MapIndex = GameData.MapIndex;
		Card->PathImage = GameData.imageFormated;
		Card->SetPlayers(GameData.players);
		Card->SetFavorite(GameData.favorite, false);
	}
}

void ULoopScrollBox::CardsDefault()
{
	for (int32 Index = 6; Index < 10; Index++)
	{
		if (!CardReference.IsValidIndex(Index)) continue;

		UCard* CurrentCard = CardReference[Index];
		CurrentCard->SetFocusCard(false, false);
		CurrentCard->SetRenderOpacity(1);
		CurrentCard->SetRenderTransform(FWidgetTransform());

		if(Index - 6 >= ChildrenCount)
		{
			CurrentCard->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			CurrentCard->SetVisibility(ESlateVisibility::Visible);
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
	Cover->SetCoverImage(ImageCardDefault, ImageCardDefault->GetSizeX(), ImageCardDefault->GetSizeY());
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

	for (int32 i = 0; i < 16; i++)
	{
		UCard* Card = WidgetTree->ConstructWidget<UCard>(CardClassReference);
		CardReference.Add(Card);
		CanvasCards->AddChild(Card);
		UCanvasPanelSlot* CanvasCard = Cast<UCanvasPanelSlot>(Card->Slot);
		const int32 Position = i * 385;
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

void ULoopScrollBox::DirectionRight()
{
	OnDirectionRight();
}

void ULoopScrollBox::DirectionLeft()
{
	OnDirectionLeft();
}

void ULoopScrollBox::EffectSound(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	SoundNavigate = NavigateSound;
	SoundSelect = SelectSound;
}