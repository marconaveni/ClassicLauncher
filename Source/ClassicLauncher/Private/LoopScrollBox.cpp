// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoopScrollBox.h"

#include "Arrow.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "Card.h"
#include "Cover.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Scrollbox.h"


void ULoopScrollBox::NativeOnInitialized()
{
	BtnClick->OnPressed.AddDynamic(this, &ULoopScrollBox::OnClickButton);
	PrepareScrollBox();
	Super::NativeOnInitialized();
}

void ULoopScrollBox::Clear()
{
	Offset = 0.0f;
	Speed = 30.0f;
	//Time = 0.0f;
	//IndexScroll = 0;
	//Move = 0;
	ChildrenCount = 0;
	InputDirection = EButtonsGame::NONE;
	PositionOffsetFocus = 1;
	IndexFocusCard = 0;
	//UnlockInput = true;
}

void ULoopScrollBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SetAnimatedScrollOffset();
	ScrollMovement();
}

void ULoopScrollBox::ScrollMovement()
{
	OnScrollMovement();
}

void ULoopScrollBox::SetAnimatedScrollOffset()
{
	OnSetAnimatedScrollOffset();
}

void ULoopScrollBox::PrepareScrollBox()
{
	OnPrepareScrollBox();
}

void ULoopScrollBox::StartScrollTo(const EButtonsGame LeftRight)
{
	InputDirection = LeftRight;
}

void ULoopScrollBox::CancelScroll()
{
	InputDirection = EButtonsGame::NONE;
}

void ULoopScrollBox::SelectDirectionScroll()
{
	OnSelectDirectionScroll();
}

void ULoopScrollBox::OpenCard()
{
	OnOpenCard();
}


void ULoopScrollBox::SetFocusCard(bool Enable)
{
	//if (CardReferenceLeft.IsValidIndex(IndexFocusCard) && CardReferenceCenter.IsValidIndex(IndexFocusCard))
	//{
	//	CardReferenceLeft[IndexFocusCard]->SetFocusCard(Enable);
	//	CardReferenceCenter[IndexFocusCard]->SetFocusCard(Enable);
	//	if (ChildrenCount <= 10)
	//	{
	//		CardReferenceRight[IndexFocusCard]->SetFocusCard(Enable);
	//	}
	//}
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

void ULoopScrollBox::OnClickButton()
{
}

void ULoopScrollBox::GetCardReference(int32 Index, UCard*& CardRef)
{
	if (CardReferenceCenter.IsValidIndex(Index))
	{
		CardRef = CardReferenceCenter[Index];
	}
}

void ULoopScrollBox::SetCardFavorite(const bool ToggleFavorite)
{
	if (CardReferenceCenter.IsValidIndex(PositionOffsetFocus - 1))
	{
		CardReferenceCenter[PositionOffsetFocus - 1]->SetFavorite(ToggleFavorite, true);
	}
}

void ULoopScrollBox::AddCardsHorizontalBox(TArray<FGameData> GameData, int32 IndexFocus)
{
	if (GameData.IsEmpty()) return;

	CoverReference.Empty();
	CardReferenceLeft.Empty();
	CardReferenceCenter.Empty();
	CardReferenceRight.Empty();
	HorizontalBoxLeft->ClearChildren();
	HorizontalBoxCenter->ClearChildren();
	HorizontalBoxRight->ClearChildren();
	ScrollBoxBottom->ClearChildren();
	Clear();

	ChildrenCount = GameData.Num();
	IndexFocusCard = (ChildrenCount > 3) ? FMath::Clamp(IndexFocus, 0, ChildrenCount) : 0;

	for (int32 i = 0; i < ChildrenCount; i++)
	{
		ConstructCover();
	}

	for (int32 i = 0; i < 5; i++)
	{
		CardReferenceLeft.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));
		CardReferenceCenter.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));
		CardReferenceRight.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));
		HorizontalBoxLeft->AddChild(CardReferenceLeft[i]);
		HorizontalBoxCenter->AddChild(CardReferenceCenter[i]);
		HorizontalBoxRight->AddChild(CardReferenceRight[i]);
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
		Card->SelectedFrameToBackground();
	}
}

void ULoopScrollBox::ConstructCover()
{
	UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), CoverClass);
	ScrollBoxBottom->AddChild(Cover);
	Cover->SetCoverImage(ImageCardDefault, ImageCardDefault->GetSizeX(), ImageCardDefault->GetSizeY());
	CoverReference.Add(Cover);
}

FIndexPositions ULoopScrollBox::GetScrollOffSet()
{
	FIndexPositions Position;
	Position.LastIndexFocus = IndexFocusCard;
	Position.Index = IndexScroll;
	//Position.OffSet = ScrollBox->GetScrollOffset();
	Position.LastIndexOffSet = PositionOffsetFocus;
	return Position;
}


void ULoopScrollBox::AddImagesCards(UTexture2D* NewTexture, int32 Width, int32 Height, int32 Index)
{
	//UCard* Left;
	//UCard* Center;
	//UCard* Right;
	//GetCardReferences(Index, Left, Center, Right);
	//Left->SetCardImage(NewTexture, Width, Height);
	//Center->SetCardImage(NewTexture, Width, Height);

	//if (ChildrenCount <= 10)
	//{
	//	Right->SetCardImage(NewTexture, Width, Height);
	//}
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