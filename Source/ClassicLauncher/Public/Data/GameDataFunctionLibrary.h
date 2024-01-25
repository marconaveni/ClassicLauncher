// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EasyXMLElement.h"
#include "GameData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameDataFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UGameDataFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	/* Sort  GameSystem.SystemLabel array */
	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void SortConfigSystem(UPARAM(ref) TArray<FGameSystem>& GameSystems, const bool bAscending = true);

	/* Sort  GameData.Name array  */
	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void SortGameData(UPARAM(ref) TArray<FGameData>& GameData, const bool bAscending = true);

	/* filters GameData.bFavorites if is equals true first in the array */
	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static TArray<FGameData> FilterGameDataFavoritesFirst(UPARAM(ref) TArray<FGameData>& GameData, const bool bOnlyFavorites);

	/* filters larger GameData.PlayCount value first in the array */
	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void FilterGameDataMostPlayed(UPARAM(ref) TArray<FGameData>& GameData);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void FilterGameDataLastPlayed(UPARAM(ref) TArray<FGameData>& GameData);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static bool SaveGameListXML(FString& GameListPath, TArray<FGameData>& NewGames);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|Configuration")
	static void SaveConfigurationData(const FConfig ConfigurationData);

	UFUNCTION(BlueprintPure, Category = "GameDataFunctionLibrary|Strings")
	static FString ReplacePath(FString Value, FString Path);

	//create a file gamelist.xml for save 
	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData|XML")
	static FString CreateXMLGameFile(TArray<FGameData> GameData);
	
	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData|XML")
	static FString CreateXMLConfigFile(FConfig ConfigData);

	//generate specific tag ex:  <name>value</name>
	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData|XML")
	static FString GenerateXmlTag(FString TagName, FString Data);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData|XML")
	static TArray<UEasyXMLElement*> LoadXML(FString XMLString, FString AccessString);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData|XML")
	static UEasyXMLElement* LoadXMLSingle(FString XMLString, FString AccessString);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void SetConfig(UEasyXMLElement* Element, FConfig& Config);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void SetGameSystem(TArray<UEasyXMLElement*>  Elements, TArray<FGameSystem>& ConfigSystems);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void SetGameData(TArray<UEasyXMLElement*>  Elements, TArray<FGameData>& GameData, UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static FGameSystem SetSystemToGameData(TArray<FGameSystem> Systems);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static void FormatGameData(UPARAM(ref) FGameSystem& GameSystems, FConfig Configuration);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|GameData")
	static bool FindGameData(TArray<FGameData> GameData, FGameData  GameDataElement, int32& Index);

	UFUNCTION(BlueprintCallable, Category = "GameDataFunctionLibrary|File IO")
	static void CreateFolders(FString PathMedia, TArray<FGameSystem> GameSystems);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static TArray<FGameData> FilterGameData(TArray<FGameData> GameData, EGamesFilter& Filter);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static bool IsHasFavorites(UPARAM(ref) TArray<FGameData>& GameData);
};
