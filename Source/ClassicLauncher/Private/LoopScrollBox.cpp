// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoopScrollBox.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "Card.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"

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

void ULoopScrollBox::SetValuesCard(UCard* Card, FString Players, bool Favorite)
{
	Card->SetPlayers(Players);
	Card->SetFavorite(Favorite, false);
	Card->SelectedFrameToBackground();
	Card->SetCardImage(ImageCardDefault, 204, 204);
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
		CardReferenceLeft.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));
		SetValuesCard(CardReferenceLeft[i], GameData[i].players, GameData[i].favorite);
		HorizontalBoxLeft->AddChild(CardReferenceLeft[i]);

		CardReferenceCenter.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));
		SetValuesCard(CardReferenceCenter[i], GameData[i].players, GameData[i].favorite);
		HorizontalBoxCenter->AddChild(CardReferenceCenter[i]);

		if (NumElements <= 10)
		{
			CardReferenceRight.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));
			SetValuesCard(CardReferenceRight[i], GameData[i].players, GameData[i].favorite);
			HorizontalBoxRight->AddChild(CardReferenceRight[i]);
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