// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameData.h"
#include "EasyXMLElement.h"
#include "ClassicFunctionLibrary.generated.h"



//~~~~~~~~~~~~~~~~~~~~~~
//			Key Modifiers
//~~~~~~~~~~~~~~~~~~~~~~
UENUM(BlueprintType)
enum class EClassicImageFormat : uint8
{
	JPG		UMETA(DisplayName = "JPG        "),
	PNG		UMETA(DisplayName = "PNG        "),
	BMP		UMETA(DisplayName = "BMP        "),
	ICO		UMETA(DisplayName = "ICO        "),
	EXR		UMETA(DisplayName = "EXR        "),
	ICNS	UMETA(DisplayName = "ICNS       ")
};


UENUM(BlueprintType)
enum class EButtonsGame : uint8
{
	LEFT	UMETA(DisplayName = "Left"),
	RIGHT	UMETA(DisplayName = "Right"),
	UP		UMETA(DisplayName = "Up"),
	DOWN	UMETA(DisplayName = "Down"),
	A		UMETA(DisplayName = "A / Enter"),
	B		UMETA(DisplayName = "B / Backspace"),
	Y		UMETA(DisplayName = "Y / F"), 
	X		UMETA(DisplayName = "X / S"),
	LB		UMETA(DisplayName = "LB / Q"),
	RB		UMETA(DisplayName = "RB / E"),
	LT		UMETA(DisplayName = "LT / A"),
	RT		UMETA(DisplayName = "RT / D"),
	START	UMETA(DisplayName = "Start / Ctrl"),
	SELECT	UMETA(DisplayName = "Select(Back) / Alt"),
	NONE    UMETA(DisplayName = "No Input")
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FLoadImageDelegate, UTexture2D*, TextureOut, int32 , Index);
/**
 * 
 */
UCLASS()
class CLASSICLAUNCHER_API UClassicFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static EUINavigation GetInputnavigation(const FKeyEvent& InKeyEvent);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static EButtonsGame GetInputButton(const FKeyEvent& InKeyEvent);

	///** Sort a GameData.name array alphabetically!  */
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static TArray<FGameData> SortGameDate(TArray<FGameData> gameData);

	///** Sort a GameData.name array alphabetically!  */
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static TArray<FConfigSystem> SortConfigSystem(TArray<FConfigSystem> configData);

	/** InstallDir/GameName */
	UFUNCTION(BlueprintPure, Category = "Functions")
	static FString GetGameRootDirectory();

	//pause a main thread
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static void PauseProcess(float timer);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static bool SaveStringToFile(FString SaveDirectory, FString JoyfulFileName, FString SaveText, bool AllowOverWriting, bool AllowAppend);

	//verify if not exists folder and create
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static FORCEINLINE bool VerifyOrCreateDirectory(const FString& TestDir)
	{
		// Every function call, unless the function is inline, adds a small
		// overhead which we can avoid by creating a local variable like so.
		// But beware of making every function inline!
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Directory Exists?
		if (!PlatformFile.DirectoryExists(*TestDir))
		{
			PlatformFile.CreateDirectory(*TestDir);

			if (!PlatformFile.DirectoryExists(*TestDir))
			{
				return false;
				//~~~~~~~~~~~~~~
			}
		}
		return true;
	}
	//delete file if exists in folder 
	UFUNCTION(BlueprintCallable, Category = "Functions")
		static FORCEINLINE bool DeleteFile(const FString& File)
	{
		// Every function call, unless the function is inline, adds a small
		// overhead which we can avoid by creating a local variable like so.
		// But beware of making every function inline!
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Directory Exists?
		if (PlatformFile.FileExists(*File))
		{
			PlatformFile.DeleteFile(*File);
			return true;
		}
		return false;
	}

	//Generate a random number without repeating the last one
	UFUNCTION(BlueprintPure, Category = "Functions")
	static int32 GenerateNumberWithoutRepeat(int32 value, int32 min, int32 max);

	UFUNCTION(BlueprintPure, Category = "Functions")
	static bool InvertBool(bool value);

	UFUNCTION(BlueprintPure, Category = "Functions")
	static FString ReplacePath(FString value , FString path);

	UFUNCTION(BlueprintPure, Category = "Functions")
	static FString ReplaceCover(FString image, FString media, FString path, FString format, FConfig config, FConfigSystem configSystem);

	//create a file gamelist.xml for save 
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static FString CreateXMLGameFile(TArray<FGameData> gameData, FVector2D IgnoreImageSize );
	
	//generate especify tag ex:  <name>value</name>
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static FString GenerateXmlTag(FString tagName, FString data);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static TArray<UEasyXMLElement*> LoadXML(FString XMLString , FString AccessString);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static UEasyXMLElement* LoadXMLSingle(FString XMLString, FString AccessString);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static FConfig SetConfig(UEasyXMLElement* element);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static TArray<FConfigSystem> SetConfigSystem(TArray<UEasyXMLElement*>  elements);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	static TArray<FGameData> SetGameData(TArray<UEasyXMLElement*>  elements);

	//formata o array game data
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static TArray<FGameData> FormatGameData(TArray<FGameData> datas, FConfig config, FConfigSystem configSystem);

	//formata o array game data
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static TArray<FGameData> FilterFavoriteGameData(TArray<FGameData> datas, bool filterFavorites);

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///these functions are not mine, all credits go to Rama

	//function credits rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintCallable, Category = "Functions")
	static bool LoadStringFile(FString& Result, FString FullFilePath);

	//function credits rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintPure, Category = "Functions|LoadTexture", meta = (DisplayName = "LoadTexture", Keywords = "loadtexture"))
	static UTexture2D* LoadTexture2DFromFile(const FString& FullFilePath, bool& IsValid, EClassicImageFormat ImageFormat, int32& Width, int32& Height);

	//function credits rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintCallable, Category = "Functions|System")
	static void CreateProcess(int32& ProcessId, FString FullPath, TArray<FString> CommandlineArgs, bool Detach, bool Hidden, int32 Priority = 0, FString OptionalWorkingDirectory = "");

	//function credits rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintPure, Category = "Functions|System")
	static bool ClassicIsApplicationRunning(int32 ProcessId)
	{
		//Please note it should really be uint32 but that is not supported by BP yet
		return FPlatformProcess::IsApplicationRunning(ProcessId);
	}

	/////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Functions|LoadTexture", meta = (DisplayName = "AsyncLoadTexture", Keywords = "asyncloadtexture"))
	static void AsyncLoadTexture2DFromFile(FLoadImageDelegate Out, const FString FullFilePath, int32 Index);

	//formated date ex 19940619T000000 to 1994-06-19 
	UFUNCTION(BlueprintPure, Category = "Functions")
	static FString FormatDateToView(FString DateXml);

	//formated date ex 19940619T224020 to 1994-06-19 22:40
	UFUNCTION(BlueprintPure, Category = "Functions")
	static FString FormatDateTimeToView(FString DateXml);

	UFUNCTION(BlueprintPure, Category = "Functions")
	static FString FormatDateToXml();


};

