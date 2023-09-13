// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameData.h"
#include "EasyXMLElement.h"
#include "ClassicFunctionLibrary.generated.h"



//~~~~~~~~~~~~~~~~~~~~~~
//			Format
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

//~~~~~~~~~~~~~~~~~~~~~~
//			Key Modifiers
//~~~~~~~~~~~~~~~~~~~~~~
UENUM(BlueprintType)
enum class EButtonsGame : uint8
{
	NONE		UMETA(DisplayName = "No Input"),
	LEFT		UMETA(DisplayName = "Left"),
	RIGHT		UMETA(DisplayName = "Right"),
	UP			UMETA(DisplayName = "Up"),
	DOWN		UMETA(DisplayName = "Down"),
	A			UMETA(DisplayName = "A / Enter"),
	B			UMETA(DisplayName = "B / Backspace"),
	Y			UMETA(DisplayName = "Y / F"), 
	X			UMETA(DisplayName = "X / S"),
	LB			UMETA(DisplayName = "LB / Q"),
	RB			UMETA(DisplayName = "RB / E"),
	LT			UMETA(DisplayName = "LT / A"),
	RT			UMETA(DisplayName = "RT / D"),
	START		UMETA(DisplayName = "Start / Ctrl"),
	SELECT		UMETA(DisplayName = "Select(Back) / Alt"),
	SCROLLUP	UMETA(DisplayName = "Stick Right Up / Mouse Scroll Up"),
	SCROLLDOWN	UMETA(DisplayName = "Stick Right Down / Mouse Scroll Down"),
	M			UMETA(DisplayName = "Stick Right Button / M")
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

	///** Return EUINavigation*/
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|Input")
	static EUINavigation GetInputnavigation(const FKeyEvent& InKeyEvent);

	///** Return EEButtonsGame*/
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|Input")
	static EButtonsGame GetInputButton(const FKeyEvent& InKeyEvent);

	///** Sort a GameData.name array alphabetically!  */
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static void SortGameDate(TArray<FGameData>& GameDatas);

	///** Sort a GameData.name array alphabetically!  */
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static TArray<FConfigSystem> SortConfigSystem(TArray<FConfigSystem> configData);

	/** InstallDir/GameName */
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|File IO")
	static FString GetGameRootDirectory();

	//pause a main thread
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|System")
	static void PauseProcess(float timer);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|File IO")
	static bool SaveStringToFile(FString SaveDirectory, FString JoyfulFileName, FString SaveText, bool AllowOverWriting, bool AllowAppend);

	//verify if not exists folder and create
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|File IO")
	static bool VerifyOrCreateDirectory(const FString& TestDir);

	//delete file if exists in folder 
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|File IO")
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
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Integers")
	static int32 GenerateNumberWithoutRepeat(int32 value, int32 min, int32 max);

	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Strings")
	static FString ReplacePath(FString value, FString path);

	/** Replace name core Remove */
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Strings", Meta = (ReturnDisplayName = "Replaced Core Name"))
	static FString CoreReplace(FString Core);

	/** Replace $(Home) ClassicLauncher root directory  */
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Strings")
	static FString HomeDirectoryReplace(FString Directory);

	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Strings", Meta = (ReturnDisplayName = "Core Found"))
	static bool SwitchOnDefaultLibreto(FString Core, FString& CoreFormated, bool& CanUnzip);

	/**
	*Replace relative media path
	*
	*@param    OriginalPathMedia EX: "./game.png or c:\games\roms\game.png" <image> or <thumbnail> or <video> in gamelist.xml
	*@param    PathMedia EX: "c:\classiclauncher\media" <pathmedia> in config.xml
	*@param    RomName EX: "./game.zip or c:\games\roms\game.zip" in <path> gamelist.xml
	*@param    SystemName EX: snes  <systemname> in configsys
	*@param    TypeMedia 3 types "covers" "screenshots" "videos"
	*@param    Format  2 types .png . mp4
	*@return   Return new path EX: "c:\classiclauncher\media\covers\game.png"
	*/
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Strings")
	static FString ReplaceMedia(FString OriginalPathMedia, FString PathMedia, FString PathRom, FString RomName, FString SystemName, FString TypeMedia, FString Format);

	//create a file gamelist.xml for save 
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData|XML")
	static FString CreateXMLGameFile(TArray<FGameData> gameData, FVector2D IgnoreImageSize);

	//create a file config.xml for save 
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData|XML")
	static FString CreateXMLConfigFile(FConfig ConfigData);

	//generate specific tag ex:  <name>value</name>
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData|XML")
	static FString GenerateXmlTag(FString tagName, FString data);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData|XML")
	static TArray<UEasyXMLElement*> LoadXML(FString XMLString, FString AccessString);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData|XML")
	static UEasyXMLElement* LoadXMLSingle(FString XMLString, FString AccessString);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static void SetConfig(UEasyXMLElement* Element, FConfig& Config);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static void SetConfigSystem(TArray<UEasyXMLElement*>  Elements, TArray<FConfigSystem>& ConfigSystems);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static void SetGameData(TArray<UEasyXMLElement*>  Elements, TArray<FGameData>& GameDatas);

	//format the array game data with correct path system
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static void FormatGameData(TArray<FGameData>& GameDatas, FConfig Config, FConfigSystem ConfigSystem);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static bool FindGameData(TArray<FGameData> datas, FGameData  DataElement, int32& Index , int32 Find = -1);


	//Return filter GameDatas Array favorites are true
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GameData")
	static TArray<FGameData> FilterFavoriteGameData(TArray<FGameData> GameData, bool FilterFavorites);

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///these functions are not mine, all credits go to Rama

	//function credits Rama VictoryBPFunctionLibrary
	/** Obtain all files in a provided directory, with optional extension filter. All files are returned if Ext is left blank. Returns false if operation could not occur. */
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|File IO")
	static bool ClassicGetFiles(TArray<FString>& Files, FString RootFolderFullPath, FString Ext);

	//function credits Rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|File IO")
	static bool LoadStringFile(FString& Result, FString FullFilePath);

	//function credits Rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|LoadTexture")
	static UTexture2D* LoadTexture2DFromFile(const FString& FilePath, EClassicImageFormat ImageFormat, int32& Width, int32& Height);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|LoadTexture", meta = (DisplayName = "Async Load Texture", Keywords = "loadtexture"))
	static void AsyncLoadTexture2DFromFile(FLoadImageDelegate Out, const FString FullFilePath, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|LoadTexture", meta = (DisplayName = "Load Texture", Keywords = "loadtexture" , ReturnDisplayName = "TextureOut"))
	static UTexture2D* LoadTexture(const FString& FilePath, int32& Width, int32& Height);

	//function credits Rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|System")
	static void CreateProcess(int32& ProcessId, FString FullPath, TArray<FString> CommandlineArgs, bool Detach, bool Hidden, int32 Priority = 0, FString OptionalWorkingDirectory = "");

	//function credits Rama VictoryBPFunctionLibrary
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|System")
	static bool ClassicIsApplicationRunning(int32 ProcessId)
	{
		//Please note it should really be uint32 but that is not supported by BP yet
		return FPlatformProcess::IsApplicationRunning(ProcessId);
	}

	//formated date ex 19940619T000000 to 1994-06-19 
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Date")
	static FString FormatDateToView(FString DateXml);

	//formated date ex 19940619T224020 to 1994-06-19 22:40
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Date")
	static FString FormatDateTimeToView(FString DateXml);

	//formated date time now to ex 1994-06-19 22:40 to 19940619T224020 
	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|Date")
	static FString FormatDateToXml();

	/** Converts hex string to color. Supports formats RGB, RRGGBB, RRGGBBAA, RGB, #RRGGBB, #RRGGBBAA */
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|Color")
	static FColor HexToColor(FString HexString);

	/** Converts color to hex string */
	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|Color")
	static FString ColorToHex(FColor Color);

	UFUNCTION(BlueprintPure, Category = "ClassicFunctionLibrary|GeneralProjectSettings")
	static FString GetProjectVersion();

	UFUNCTION(BlueprintCallable, Category = "ClassicFunctionLibrary|GeneralProjectSettings")
	static FString GetProjectName();


};
