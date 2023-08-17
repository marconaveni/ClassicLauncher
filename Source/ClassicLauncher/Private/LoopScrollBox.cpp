// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "LoopScrollBox.h"
#include "ClassicFunctionLibrary.h"
#include "GameData.h"
#include "Card.h"
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
}

void ULoopScrollBox::OnClickButton()
{
}

void ULoopScrollBox::GetCardReferences(int32 Index, UCard*& Left, UCard*& Right)
{
}

void ULoopScrollBox::AddCardsHorizontalBox(TArray<FGameData> GameData, int32 IndexFocus)
{
}

void ULoopScrollBox::AddImagesCards(UTexture2D* NewTexture, int32 Width, int32 Height, int32 Index)
{
}
