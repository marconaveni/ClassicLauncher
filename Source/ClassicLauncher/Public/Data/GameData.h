// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	// Use UPROPERTY() to decorate member variables as they allow for easier integration with network replication as well as potential garbage collection processing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString rating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString releasedate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString developer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString publisher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString genre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString hash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString video;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString genreid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool favorite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 playcount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString lastplayed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Executable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Arguments;


	//formated 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PathFormated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString nameFormated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString descFormated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString imageFormated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString thumbnailFormated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString videoFormated;


	//construtor
	FGameData()
	{
		//map index
		MapIndex = -1;

		Path = TEXT("");
		name = TEXT("");
		desc = TEXT("");
		rating = TEXT("");
		releasedate = TEXT("");
		developer = TEXT("");
		publisher = TEXT("");
		genre = TEXT("");
		players = TEXT("1");
		hash = TEXT("");
		image = TEXT("");
		thumbnail = TEXT("");
		video = TEXT("");
		genreid = TEXT("");
		favorite = false;
		playcount = 0;
		lastplayed = TEXT("");
		Executable = TEXT("");
		Arguments = TEXT("");

		//formated
		PathFormated = TEXT("");
		nameFormated = TEXT("");
		descFormated = TEXT("");
		imageFormated = TEXT("");
		thumbnailFormated = TEXT("");
		videoFormated = TEXT("");
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
		if (OrderBy == EGamesFilter::DEFAULT)
			OrderBy = EGamesFilter::FAVORITES_FIRST;
		else if (OrderBy == EGamesFilter::FAVORITES_FIRST)
			OrderBy = EGamesFilter::FAVORITES_ONLY;
		else
			OrderBy = EGamesFilter::DEFAULT;
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
	TArray<FGameData> GameDatas;

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
