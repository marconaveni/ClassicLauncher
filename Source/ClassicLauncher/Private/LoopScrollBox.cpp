// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoopScrollBox.h"

#include "Arrow.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "Card.h"
#include "Cover.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
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
	Time = 0.0f;
	IndexScroll = 0;
	Move = 0;
	ChildrenCount = 0;
	InputDirection = EButtonsGame::NONE;
	PositionOffsetFocus = 1;
	IndexFocusCard = 0;
	UnlockInput = true;
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

void ULoopScrollBox::StartScrollTo(EButtonsGame LeftRight)
{
	OnStartScrollTo(LeftRight);
}

void ULoopScrollBox::CancelScroll()
{
	OnCancelScroll();
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
	if (CardReferenceLeft.IsValidIndex(IndexFocusCard) && CardReferenceCenter.IsValidIndex(IndexFocusCard))
	{
		CardReferenceLeft[IndexFocusCard]->SetFocusCard(Enable);
		CardReferenceCenter[IndexFocusCard]->SetFocusCard(Enable);
		if (ChildrenCount <= 10)
		{
			CardReferenceRight[IndexFocusCard]->SetFocusCard(Enable);
		}
	}
}

void ULoopScrollBox::SetFocusCover()
{
	if (CoverReference.IsValidIndex(IndexFocusCard))
	{
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

void ULoopScrollBox::GetCardReferences(int32 Index, UCard*& Left, UCard*& Center , UCard*& Right)
{
	if (CardReferenceLeft.IsValidIndex(Index) && CardReferenceCenter.IsValidIndex(Index))
	{
		Left = CardReferenceLeft[Index];
		Center = CardReferenceCenter[Index];
		Right = CardReferenceCenter[Index];
	}
	if (CardReferenceRight.IsValidIndex(Index)  )
	{
		if(ChildrenCount <= 10)
		{
			Right = CardReferenceRight[Index];
		}
	}
}

void ULoopScrollBox::AddCardsHorizontalBox(TArray<FGameData> GameData, int32 IndexFocus)
{
	if (GameData.IsEmpty()) return;

	const int32 NumElements = GameData.Num() - 1;
	FString Players;

	HorizontalBoxLeft->ClearChildren();
	HorizontalBoxCenter->ClearChildren();
	HorizontalBoxRight->ClearChildren();

	CardReferenceLeft.Empty();
	CardReferenceCenter.Empty();
	CardReferenceRight.Empty();

	ScrollBoxBottom->ClearChildren();
	CoverReference.Empty();

	Clear();


	IndexFocusCard = (NumElements > 4) ? FMath::Clamp(IndexFocus, 0, NumElements) : 0;

	if(NumElements > 10)
	{
		UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		FSlateBrush NewBrush;
		NewBrush.SetImageSize(FVector2D(385 * GameData.Num(), 88));
		Image->SetBrush(NewBrush);
		HorizontalBoxRight->AddChild(Image);
	}

	for (int32 i = 0; i <= NumElements; i++)
	{
		CardReferenceLeft.Add(ConstructCard(HorizontalBoxLeft, GameData[i]));
		CardReferenceCenter.Add(ConstructCard(HorizontalBoxCenter, GameData[i]));
		ConstructCover(GameData[i].Texture);

		if (NumElements <= 10)
		{
			CardReferenceRight.Add(ConstructCard(HorizontalBoxRight, GameData[i]));
		}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (Debug)
		{
			CardReferenceLeft[i]->SetColorAndOpacity(FLinearColor(1.0f, 0.384006f, 0.423825f, 1.0f));
			CardReferenceCenter[i]->SetColorAndOpacity(FLinearColor(0.446118f, 1.0f, 0.431361f, 1.0f));
			if (NumElements <= 10)
			{
				CardReferenceRight[i]->SetColorAndOpacity(FLinearColor(0.446118f, 0.431361f, 1.0f, 1.0f));
			}
		}
		else
		{
			CardReferenceLeft[i]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			CardReferenceCenter[i]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			if (NumElements <= 10)
			{
				CardReferenceRight[i]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
#endif
	}

	PrepareScrollBox();
}

UCard* ULoopScrollBox::ConstructCard(UHorizontalBox* HorizontalBox, FGameData GameData)
{
	UCard* Card = CreateWidget<UCard>(GetOwningPlayer(), CardClassReference);
	Card->MapIndex = GameData.MapIndex;
	Card->PathImage = GameData.imageFormated;
	Card->SetPlayers(GameData.players);
	Card->SetFavorite(GameData.favorite, false);
	Card->SelectedFrameToBackground();
	if(GameData.Texture != nullptr)
	{
		Card->SetCardImage(GameData.Texture, GameData.Texture->GetSizeX(), GameData.Texture->GetSizeY());
	}
	else
	{
		Card->SetCardImage(ImageCardDefault, 204, 204);
	}
	HorizontalBox->AddChild(Card);
	return Card;
}

void ULoopScrollBox::ConstructCover(UTexture2D* Texture)
{
	UCover* Cover = CreateWidget<UCover>(GetOwningPlayer(), CoverClass);
	ScrollBoxBottom->AddChild(Cover);
	if (Texture != nullptr)
	{
		Cover->SetCoverImage(Texture, Texture->GetSizeX(), Texture->GetSizeY());
	}
	else
	{
		Cover->SetCoverImage(ImageCardDefault, 204, 204);
	}
	CoverReference.Add(Cover);
}

FIndexPositions ULoopScrollBox::GetScrollOffSet()
{
	FIndexPositions Position;
	Position.LastIndexFocus = IndexFocusCard;
	Position.Index = IndexScroll;
	Position.OffSet = ScrollBox->GetScrollOffset();
	Position.LastIndexOffSet = PositionOffsetFocus;
	return Position;
}


void ULoopScrollBox::AddImagesCards(UTexture2D* NewTexture, int32 Width, int32 Height, int32 Index)
{
	UCard* Left;
	UCard* Center;
	UCard* Right;
	GetCardReferences(Index, Left, Center,Right);
	Left->SetCardImage(NewTexture, Width, Height);
	Center->SetCardImage(NewTexture, Width, Height);

	if (ChildrenCount <= 10)
	{
		Right->SetCardImage(NewTexture, Width, Height);
	}
}

void ULoopScrollBox::DirectionRight()
{
	OnDirectionRight();
}

void ULoopScrollBox::DirectionLeft()
{
	OnDirectionLeft();
}