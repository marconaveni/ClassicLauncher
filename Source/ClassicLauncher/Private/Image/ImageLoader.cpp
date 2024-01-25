// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Image/ImageLoader.h"
#include "Data/GameData.h"

UImageLoader::UImageLoader()
	: FirstIndex(-1)
	  , LastIndex(-1)
	  , Counter(0)
	  , MaxLoop(0)
	  , DefaultTexture(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureDefaultAsset(TEXT("/Game/themes/default-cover.default-cover"));
	if (TextureDefaultAsset.Succeeded())
	{
		DefaultTexture = Cast<UTexture2D>(TextureDefaultAsset.Object);
	}
}

void UImageLoader::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UImageLoader::Deinitialize()
{
	Super::Deinitialize();
}

bool UImageLoader::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return Super::DoesSupportWorldType(WorldType);
}

void UImageLoader::AddImageRange(const EButtonsGame Input, const int32 IndexCard, TArray<FGameData>& GameData, const int32 DistanceIndex)
{
	if ((Input == EButtonsGame::RIGHT || Input == EButtonsGame::RB ||
		Input == EButtonsGame::LEFT || Input == EButtonsGame::LB) && DistanceIndex > 0 && GameData.Num() >= 64)
	{
		if (Input == EButtonsGame::RIGHT || Input == EButtonsGame::RB)
		{
			FirstIndex = IndexCard - DistanceIndex;
			LastIndex = IndexCard + (DistanceIndex - 1);
		}
		else if (Input == EButtonsGame::LEFT || Input == EButtonsGame::LB)
		{
			FirstIndex = IndexCard - (DistanceIndex - 1);
			LastIndex = IndexCard + DistanceIndex;
		}

		if (FirstIndex < 0)
		{
			FirstIndex = GameData.Num() - FMath::Abs(FirstIndex);
		}
		if (LastIndex >= GameData.Num())
		{
			LastIndex = LastIndex - GameData.Num();
		}

		FirstIndex = FMath::Clamp(FirstIndex, 0, GameData.Num() - 1);
		LastIndex = FMath::Clamp(LastIndex, 0, GameData.Num() - 1);

		int32 IndexLoad = -1, IndexUnload = -1;
		if (Input == EButtonsGame::RIGHT || Input == EButtonsGame::RB)
		{
			IndexUnload = FirstIndex;
			IndexLoad = LastIndex;
		}
		else if (Input == EButtonsGame::LEFT || Input == EButtonsGame::LB)
		{
			IndexUnload = LastIndex;
			IndexLoad = FirstIndex;
		}
		if (IndexLoad != -1 && IndexUnload != -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("FirstIndex %d  IndexCard %d LastIndex %d"), FirstIndex, IndexCard, LastIndex);
			const FString PathImage = GameData[IndexLoad].imageFormated;
			const EClassicImageFormat Format = UClassicFunctionLibrary::GetFormatImage(PathImage);
			FLoadImageDelegate Out;
			Out.BindDynamic(this, &UImageLoader::OutLoadImage);
			UClassicFunctionLibrary::AsyncLoadTexture2DFromFile(Out, PathImage, IndexLoad, Format, EClassicTextureFilter::DEFAULT);
			if (ImageDelegate.IsBound())
			{
				TexturesMap.Add(IndexUnload, DefaultTexture);
				ImageDelegate.Broadcast(DefaultTexture, IndexUnload, true);
			}
		}
	}
}

void UImageLoader::AddImageLoop(int32 IndexCard, TArray<FGameData>& GameData, const int32 Count)
{
	Counter = 0;
	FirstIndex = IndexCard - 1;
	LastIndex = IndexCard;
	bool FlipFlop = true;

	const int32 MaxLength = GameData.Num() - 1;
	MaxLoop = FMath::Clamp(Count, 0, MaxLength);

	for (int i = 0; i <= MaxLoop; ++i)
	{
		int Index = -1;
		if (FlipFlop)
		{
			Index = (LastIndex <= MaxLength) ? LastIndex : 0;
			LastIndex = Index + 1;
		}
		else
		{
			Index = (FirstIndex >= 0) ? FirstIndex : GameData.Num() - 1;
			FirstIndex = Index - 1;
		}
		const FString PathImage = GameData[Index].imageFormated;
		const EClassicImageFormat Format = UClassicFunctionLibrary::GetFormatImage(PathImage);
		FLoadImageDelegate Out;
		Out.BindDynamic(this, &UImageLoader::OutLoopImage);
		UClassicFunctionLibrary::AsyncLoadTexture2DFromFile(Out, PathImage, Index, Format, EClassicTextureFilter::DEFAULT);
		FlipFlop = !FlipFlop;
	}
}

void UImageLoader::OutLoadImage(UTexture2D* TextureOut, int32 Index, bool Sucess)
{
	if (ImageDelegate.IsBound())
	{
		UTexture2D* TextureFinal = DefaultTexture;
		if (TextureOut != nullptr)
		{
			TextureFinal = TextureOut;
		}
		TexturesMap.Add(Index, TextureFinal);
		ImageDelegate.Broadcast(TextureFinal, Index, Sucess);
	}
}

void UImageLoader::OutLoopImage(UTexture2D* TextureOut, int32 Index, bool Sucess)
{
	OutLoadImage(TextureOut, Index, Sucess);
	Counter++;
	UE_LOG(LogTemp, Warning, TEXT("Counter value is %d Value Maxloop is %d"), Counter, MaxLoop);
	if (Counter > MaxLoop && FinalizeDelegate.IsBound())
	{
		FinalizeDelegate.Broadcast();
	}
}
