// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoopScrollBox.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "Card.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"

void ULoopScrollBox::NativeOnInitialized()
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

	BtnClick->OnPressed.AddDynamic(this, &ULoopScrollBox::OnClickButton);

	PrepareScrollBox();
	Super::NativeOnInitialized();
}

void ULoopScrollBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	SetAnimatedScrollOffset();
	ScrollMovement();

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULoopScrollBox::ScrollMovement()
{
	OnScrollMovement();
}

void ULoopScrollBox::SetAnimatedScrollOffset()
{
	OnSetAnimatedScrollOffset();
}

void ULoopScrollBox::DirectionRight()
{
	OnDirectionRight();
}

void ULoopScrollBox::DirectionLeft()
{
	OnDirectionLeft();
}

void ULoopScrollBox::PrepareScrollBox()
{
	OnPrepareScrollBox();
}

void ULoopScrollBox::StartScrollTo(EButtonsGame LeftRight)
{
	EButtonsGame Input = LeftRight;
	OnStartScrollTo(Input);
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
	Card->SetFavorite(Favorite,false);
}

void ULoopScrollBox::OnClickButton()
{
}

void ULoopScrollBox::GetCardReferences(int32 Index, UCard*& Left, UCard*& Center)
{
	if (CardReferenceLeft.IsValidIndex(Index) && CardReferenceCenter.IsValidIndex(Index))
	{
		Left = CardReferenceLeft[Index];
		Center = CardReferenceCenter[Index];
	}
}

void ULoopScrollBox::AddCardsHorizontalBox(TArray<FGameData> GameData, int32 IndexFocus)
{
	if (GameData.IsEmpty()) return;

	const int32 NumElements = GameData.Num() - 1;
	FString Players;
	bool Favorite;
	float SizeX;
	float SizeY;

	IndexFocusCard = FMath::Clamp(IndexFocus, 0, NumElements);

	if (ChildrenCount > 0)
	{
		HorizontalBoxLeft->ClearChildren();
		HorizontalBoxCenter->ClearChildren();
		ChildrenCount = 0;
	}

	for (int32 i = 0; i <= NumElements; i++)
	{
		 Players = GameData[i].players;
		 Favorite = GameData[i].favorite;
		 SizeX =  GameData[i].ImageX;
		 SizeY = GameData[i].ImageY;

		 CardReferenceLeft.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));
		 CardReferenceCenter.Add(CreateWidget<UCard>(GetOwningPlayer(), CardClassReference));

		 SetValuesCard(CardReferenceCenter[i], Players, Favorite);
		 HorizontalBoxLeft->AddChild(CardReferenceLeft[i]);
		 HorizontalBoxCenter->AddChild(CardReferenceCenter[i]);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		 if (Debug)
		 {
			 CardReferenceLeft[i]->SetColorAndOpacity(FLinearColor(1.0f, 0.384006f, 0.423825f, 1.0f));
			 CardReferenceCenter[i]->SetColorAndOpacity(FLinearColor(0.446118f, 1.0f, 0.431361f, 1.0f));
		 }
#endif
	}

	PrepareScrollBox();
}

void ULoopScrollBox::AddImagesCards(UTexture2D* NewTexture, int32 Width, int32 Height, int32 Index)
{
	UCard* Left;
	UCard* Center;
	GetCardReferences(Index, Left, Center);
	Left->LoadImageCard(NewTexture, Width, Height);
	Center->LoadImageCard(NewTexture, Width, Height);
}
