// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XMLDateTime.h"
#include "GameData.generated.h"

UENUM(BlueprintType)
enum class EGamesFilter : uint8
{
	DEFAULT UMETA(DisplayName = "Default"),
	FAVORITES_FIRST UMETA(DisplayName = "Favorites First"),
	FAVORITES_ONLY UMETA(DisplayName = "Favorites Only")
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()

	// map index
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MapIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Rating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Developer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Publisher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Genre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Hash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Screenshot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Video;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GenreId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFavorite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Executable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Arguments;
	
	//
	//formatted 
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PathFormatted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NameFormatted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DescFormatted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ImageFormatted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ScreenshotFormatted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VideoFormatted;

	//
	//datetime
	//
	UPROPERTY()
	FXMLDateTime ReleaseDate;

	UPROPERTY()
	FXMLDateTime LastPlayed;
	
	//constructor
	FGameData()
	: MapIndex(-1) //map index
	, Players(TEXT("1"))
	, bFavorite(false)
	, PlayCount(0)
	{}

	bool operator==(const FGameData& A) const
	{
		return (MapIndex == A.MapIndex);
	}
	bool operator>(const FGameData& A) const
	{
		return (MapIndex > A.MapIndex);
	}
	bool operator<(const FGameData& A) const
	{
		return (MapIndex < A.MapIndex);
	}
};

USTRUCT(BlueprintType)
struct FIndexPositions
{
	GENERATED_BODY()

	/** Last focus card */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LastIndexFocus;
	/** Last index offset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LastIndexOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGamesFilter OrderBy;

	FIndexPositions()
		: LastIndexFocus(0)
		  , LastIndexOffSet(1)
		  , OrderBy(EGamesFilter::DEFAULT)
	{
	}

	void DefaultValues()
	{
		LastIndexFocus = 0;
		LastIndexOffSet = 1;
		OrderBy = EGamesFilter::DEFAULT;
	}

	void ChangeFilter()
	{
		switch (OrderBy)
		{
		case EGamesFilter::DEFAULT: OrderBy = EGamesFilter::FAVORITES_FIRST;
			break;
		case EGamesFilter::FAVORITES_FIRST: OrderBy = EGamesFilter::FAVORITES_ONLY;
			break;
		case EGamesFilter::FAVORITES_ONLY: OrderBy = EGamesFilter::DEFAULT;
			break;
		}
	}
};

/**
 *
 */
//Use USTRUCT(BlueprintType) if you would like to include your Struct in Blueprints
USTRUCT(BlueprintType)
struct FGameSystem
{
	GENERATED_BODY()

	// Use UPROPERTY() to decorate member variables as they allow for easier integration with network replication as well as potential garbage collection processing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Executable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Arguments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RomPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SystemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SystemLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Screenshot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameData> GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIndexPositions Positions;

	void DefaultValues()
	{
		Positions.DefaultValues();
	}

	void ChangeFilter()
	{
		Positions.ChangeFilter();
	}

	FGameSystem()
	{
		Executable = TEXT("");
		Arguments = TEXT("");
		RomPath = TEXT("");
		SystemName = TEXT("");
		SystemLabel = TEXT("");
		Image = TEXT("");
		Description = TEXT("");
		Screenshot = TEXT("");
	}
};


/**
 *
 */
USTRUCT(BlueprintType)
struct FConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PathMedia;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DefaultStartSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Rendering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VolumeMaster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VolumeMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VolumeVideo;

	FConfig()
		: PathMedia(TEXT(""))
		  , DefaultStartSystem(TEXT(""))
		  , Rendering(true)
		  , VolumeMaster(80)
		  , VolumeMusic(80)
		  , VolumeVideo(80)
	{
	}
};



