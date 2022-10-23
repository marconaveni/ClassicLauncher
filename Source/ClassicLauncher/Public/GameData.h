// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameData.generated.h"

/**
 *
 */
 //Use USTRUCT(BlueprintType) if you would like to include your Struct in Blueprints "i really like the blueprints integration with c++"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ImageX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ImageY;

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
		ImageX = 0;
		ImageY = 0;

		//formated
		PathFormated = TEXT("");
		nameFormated = TEXT("");
		descFormated = TEXT("");
		imageFormated = TEXT("");
		thumbnailFormated = TEXT("");
		videoFormated = TEXT("");
	}
};

/**
 *
 */
 //Use USTRUCT(BlueprintType) if you would like to include your Struct in Blueprints
USTRUCT(BlueprintType)
struct FConfigSystem
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
	int32 ImageX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ImageY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameData> GameDatas;


	//construtor
	FConfigSystem()
	{
		Executable = TEXT("");
		Arguments = TEXT("");
		RomPath = TEXT("");
		SystemName = TEXT("");
		SystemLabel = TEXT("");
		ImageX = 200;
		ImageY = 200;
	}

};


/**
 *
 */
 //Use USTRUCT(BlueprintType) if you would like to include your Struct in Blueprints
USTRUCT(BlueprintType)
struct FConfig
{
	GENERATED_BODY()

		// Use UPROPERTY() to decorate member variables as they allow for easier integration with network replication as well as potential garbage collection processing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString pathmedia;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString defaultstartsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool rendering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 volume;

	//construtor
	FConfig()
	{
		pathmedia = TEXT("");
		defaultstartsystem = TEXT("");
		rendering = true;
		volume = 80;
	}

};