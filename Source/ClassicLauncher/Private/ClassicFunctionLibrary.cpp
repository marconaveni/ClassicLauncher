// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicFunctionLibrary.h"
#include "HAL/RunnableThread.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/DateTime.h"
#include "EasyXMLParseManager.h"
#include "DynamicRHI.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/Runnable.h"

EUINavigation UClassicFunctionLibrary::GetInputnavigation(const FKeyEvent& InKeyEvent)
{
	const FString KeyEvent = InKeyEvent.GetKey().ToString();

	if (KeyEvent == "Left" || KeyEvent == "Gamepad_DPad_Left" || KeyEvent == "Gamepad_LeftStick_Left") return EUINavigation::Left;
	else if (KeyEvent == "Right" || KeyEvent == "Gamepad_DPad_Right" || KeyEvent == "Gamepad_LeftStick_Right") return EUINavigation::Right;
	else if (KeyEvent == "Up" || KeyEvent == "Gamepad_DPad_Up" || KeyEvent == "Gamepad_LeftStick_Up") 	return EUINavigation::Up;
	else if (KeyEvent == "Down" || KeyEvent == "Gamepad_DPad_Down" || KeyEvent == "Gamepad_LeftStick_Down") return EUINavigation::Down;
	else return EUINavigation::Invalid;

}

EButtonsGame UClassicFunctionLibrary::GetInputButton(const FKeyEvent& InKeyEvent)
{
	const FString KeyEvent = InKeyEvent.GetKey().ToString();

	if (KeyEvent == "Left" || KeyEvent == "Gamepad_DPad_Left" || KeyEvent == "Gamepad_LeftStick_Left")	return EButtonsGame::LEFT;
	else if (KeyEvent == "Right" || KeyEvent == "Gamepad_DPad_Right" || KeyEvent == "Gamepad_LeftStick_Right")	return EButtonsGame::RIGHT;
	else if (KeyEvent == "Up" || KeyEvent == "Gamepad_DPad_Up" || KeyEvent == "Gamepad_LeftStick_Up")	return EButtonsGame::UP;
	else if (KeyEvent == "Down" || KeyEvent == "Gamepad_DPad_Down" || KeyEvent == "Gamepad_LeftStick_Down")	return EButtonsGame::DOWN;
	else if (KeyEvent == "Enter" || KeyEvent == "Gamepad_FaceButton_Bottom")	return EButtonsGame::A;
	else if (KeyEvent == "Backspace" || KeyEvent == "Escape" || KeyEvent == "Gamepad_FaceButton_Right")	return EButtonsGame::B;
	else if (KeyEvent == "F" || KeyEvent == "Gamepad_FaceButton_Top")	return EButtonsGame::Y;
	else if (KeyEvent == "S" || KeyEvent == "Gamepad_FaceButton_Left")	return EButtonsGame::X;
	else if (KeyEvent == "Q" || KeyEvent == "Gamepad_LeftShoulder")	return EButtonsGame::LB;
	else if (KeyEvent == "E" || KeyEvent == "Gamepad_RightShoulder")  return EButtonsGame::RB;
	else if (KeyEvent == "A" || KeyEvent == "Gamepad_LeftTrigger")  return EButtonsGame::LT;
	else if (KeyEvent == "D" || KeyEvent == "Gamepad_RightTrigger")  return EButtonsGame::RT;
	else if (KeyEvent == "LeftControl" || KeyEvent == "Gamepad_Special_Right")  return EButtonsGame::START;
	else if (KeyEvent == "Alt" || KeyEvent == "Gamepad_Special_Left")  return EButtonsGame::SELECT;
	else if (KeyEvent == "Mouse_Scroll_Up" || KeyEvent == "Gamepad_RightStick_Up")  return EButtonsGame::SCROLLUP;
	else if (KeyEvent == "Mouse_Scroll_Down" || KeyEvent == "Gamepad_RightStick_Down")  return EButtonsGame::SCROLLDOWN;
	else if (KeyEvent == "M" || KeyEvent == "Gamepad_RightThumbstick")  return EButtonsGame::M;

	else return EButtonsGame::NONE;

}

void UClassicFunctionLibrary::SortGameDate(TArray<FGameData>& GameDatas)
{
	TArray<FString> Names;
	TArray<FGameData> NewGameData;

	for (FGameData& GameData : GameDatas)
	{
		Names.Add(GameData.name);
	}
	Names.Sort();
	for (FString& Name : Names)
	{
		for (FGameData& GameData : GameDatas)
		{
			if (GameData.name == Name && GameData.name != TEXT("null_value")) {
				NewGameData.Add(GameData);
				GameData.name = TEXT("null_value");
				break;
			}
		}
	}
	GameDatas.Empty();
	GameDatas = NewGameData;
	GameDatas.Shrink();
}

TArray<FGameSystem> UClassicFunctionLibrary::SortConfigSystem(TArray<FGameSystem> configData)
{
	TArray<FString> Names;
	TArray<FGameSystem> NewConfigData;

	for (FGameSystem& data : configData)
	{
		Names.Add(data.SystemLabel);
	}
	Names.Sort();
	for (FString& name : Names)
	{
		for (FGameSystem& data : configData)
		{
			if (data.SystemLabel == name && data.SystemLabel != TEXT("null_value")) {
				NewConfigData.Add(data);
				data.SystemLabel = TEXT("null_value");
			}
		}
	}

	return NewConfigData;
}

void UClassicFunctionLibrary::SaveConfig(const FConfig ConfigurationData)
{
	FString XmlConfig = CreateXMLConfigFile(ConfigurationData);
	XmlConfig = XmlConfig.Replace(TEXT("$(remove)"), TEXT(""), ESearchCase::IgnoreCase);
	const FString PathToSave = GetGameRootDirectory() + TEXT("config");
	const bool Saved = (SaveStringToFile(PathToSave, TEXT("config.xml"), XmlConfig, true, false));
	UE_LOG(LogTemp, Warning, TEXT("%s"), (Saved) ? TEXT("Saved File") : TEXT("Not Saved File"));
}

FString UClassicFunctionLibrary::GetGameRootDirectory()
{
	FString RootGameDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	RootGameDir = RootGameDir.Replace(TEXT("/"), TEXT("\\"));
	return RootGameDir;
}

void UClassicFunctionLibrary::PauseProcess(float timer)
{
	FPlatformProcess::Sleep(timer);
}

bool UClassicFunctionLibrary::SaveStringToFile(FString SaveDirectory, FString JoyfulFileName, FString SaveText, bool AllowOverWriting, bool AllowAppend)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveDirectory))
	{
		//Could not make the specified directory
		return false;
		//~~~~~~~~~~~~~~~~~~~~~~
	}

	//get complete file path
	SaveDirectory += "\\";
	SaveDirectory += JoyfulFileName;

	//No over-writing?
	if (!AllowOverWriting)
	{
		//Check if file exists already
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			//no overwriting
			return false;
		}
	}

	if (AllowAppend)
	{
		SaveText += "\n";
		return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}
	return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory, FFileHelper::EEncodingOptions::ForceUTF8);
}

bool UClassicFunctionLibrary::VerifyOrCreateDirectory(const FString& TestDir)
{
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

int32 UClassicFunctionLibrary::GenerateNumberWithoutRepeat(int32 value, int32 min, int32 max)
{
	int32 NewValue = value;
	if (max > min) {
		while (true) {
			NewValue = FMath::RandRange(min, max);
			if (value != NewValue) {
				return NewValue;
			}
		}
	}
	return NewValue;
}


FString UClassicFunctionLibrary::ReplacePath(FString value, FString path)
{
	path = path + TEXT("\\");
	const TCHAR* cpath = *path;
	value = value.Replace(TEXT("/"), TEXT("\\"), ESearchCase::IgnoreCase);
	value = value.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
	value = value.Replace(TEXT(".\\"), cpath, ESearchCase::IgnoreCase);
	return value;
}

FString UClassicFunctionLibrary::CoreReplace(FString Core)
{
	Core = Core.Replace(TEXT("$("), TEXT(""), ESearchCase::IgnoreCase);
	Core = Core.Replace(TEXT(")"), TEXT(""), ESearchCase::IgnoreCase);
	return Core;
}

FString UClassicFunctionLibrary::HomeDirectoryReplace(FString Directory)
{
	FString RootDirectory = GetGameRootDirectory();
	return Directory.Replace(TEXT("$(home)"), *RootDirectory, ESearchCase::IgnoreCase);
}

bool UClassicFunctionLibrary::SwitchOnDefaultLibreto(FString Core, FString& CoreFormated, bool& CanUnzip)
{
	CanUnzip = true;
	if (Core == TEXT("$(fbneo_libretro.dll)")) {
		CoreFormated = CoreReplace(Core); CanUnzip = false;
	}
	else if (Core == TEXT("$(gearboy_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else if (Core == TEXT("$(genesis_plus_gx_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else if (Core == TEXT("$(mame2003_libretro.dll)")) {
		CoreFormated = CoreReplace(Core); CanUnzip = false;
	}
	else if (Core == TEXT("$(mupen64plus_next_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else if (Core == TEXT("$(nestopia_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else if (Core == TEXT("$(smsplus_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else if (Core == TEXT("$(snes9x_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else if (Core == TEXT("$(stella_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else if (Core == TEXT("$(vbam_libretro.dll)")) {
		CoreFormated = CoreReplace(Core);
	}
	else {
		return false;
	}
	return true;
}

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
FString UClassicFunctionLibrary::ReplaceMedia(FString OriginalPathMedia, FString PathMedia, FString PathRom, FString RomName, FString SystemName, FString TypeMedia, FString Format, FString RomFormated)
{
	FString NewPath = ReplacePath(OriginalPathMedia, PathRom);

	if (!FPaths::FileExists(NewPath))
	{
		const FString BaseFilename = FPaths::GetBaseFilename(RomFormated);

		NewPath = PathMedia + TEXT("\\") + SystemName + TEXT("\\") + TypeMedia + TEXT("\\") + BaseFilename + Format;
		UE_LOG(LogTemp, Warning, TEXT("Path out is  %s"), *NewPath);
	}

	return NewPath;

}

FString UClassicFunctionLibrary::CreateXMLGameFile(TArray<FGameData> gameData, FVector2D IgnoreImageSize)
{
	FString NewGameData;

	NewGameData += TEXT("<?xml version=\"1.0\"?>\n<gameList>\n");

	for (FGameData& Data : gameData)
	{
		NewGameData += TEXT("<game>\n");
		NewGameData += GenerateXmlTag(TEXT("path"), Data.Path);
		NewGameData += GenerateXmlTag(TEXT("name"), Data.name);
		NewGameData += GenerateXmlTag(TEXT("desc"), Data.desc);
		NewGameData += GenerateXmlTag(TEXT("rating"), Data.rating);
		NewGameData += GenerateXmlTag(TEXT("releasedate"), Data.releasedate);
		NewGameData += GenerateXmlTag(TEXT("developer"), Data.developer);
		NewGameData += GenerateXmlTag(TEXT("publisher"), Data.publisher);
		NewGameData += GenerateXmlTag(TEXT("genre"), Data.genre);
		NewGameData += GenerateXmlTag(TEXT("players"), Data.players);
		NewGameData += GenerateXmlTag(TEXT("hash"), Data.hash);
		NewGameData += GenerateXmlTag(TEXT("image"), Data.image);
		NewGameData += GenerateXmlTag(TEXT("thumbnail"), Data.thumbnail);
		NewGameData += GenerateXmlTag(TEXT("video"), Data.video);
		NewGameData += GenerateXmlTag(TEXT("genreid"), Data.genreid);
		NewGameData += GenerateXmlTag(TEXT("favorite"), (Data.favorite) ? TEXT("true") : TEXT("false"));
		NewGameData += GenerateXmlTag(TEXT("playcount"), FString::SanitizeFloat(Data.playcount, 0));
		NewGameData += GenerateXmlTag(TEXT("lastplayed"), Data.lastplayed);
		NewGameData += GenerateXmlTag(TEXT("executable"), Data.Executable);
		NewGameData += GenerateXmlTag(TEXT("arguments"), Data.Arguments);
		//if (Data.ImageX != IgnoreImageSize.X)
		//{
		//	NewGameData += GenerateXmlTag(TEXT("imagex"), FString::SanitizeFloat(Data.ImageX, 0));
		//}
		//if (Data.ImageX != IgnoreImageSize.X)
		//{
		//	NewGameData += GenerateXmlTag(TEXT("imagey"), FString::SanitizeFloat(Data.ImageY, 0));
		//}

		NewGameData += TEXT("</game>\n");

	}

	NewGameData += TEXT("</gameList>\n");
	return NewGameData;
}

FString UClassicFunctionLibrary::CreateXMLConfigFile(FConfig ConfigData)
{
	FString NewConfigXml;

	NewConfigXml += TEXT("<?xml version=\"1.0\"?>\n");

	NewConfigXml += TEXT("<config>\n");
	NewConfigXml += GenerateXmlTag(TEXT("pathmedia"), ConfigData.PathMedia);
	NewConfigXml += GenerateXmlTag(TEXT("defaultstartsystem"), ConfigData.DefaultStartSystem);
	NewConfigXml += GenerateXmlTag(TEXT("rendering"), (ConfigData.Rendering) ? TEXT("true") : TEXT("false"));
	NewConfigXml += GenerateXmlTag(TEXT("volume"), FString::SanitizeFloat(ConfigData.Volume, 0));
	NewConfigXml += TEXT("</config>\n");

	return NewConfigXml;
}

FString UClassicFunctionLibrary::GenerateXmlTag(FString tagName, FString data)
{
	if (data == TEXT("") || data == TEXT("0")) {
		return TEXT("");
	}
	FString gameDt;
	gameDt += TEXT("\t<");
	gameDt += tagName;
	gameDt += TEXT(">");
	gameDt += data;
	gameDt += TEXT("</");
	gameDt += tagName;
	gameDt += TEXT(">\n");

	return gameDt;
}

TArray<UEasyXMLElement*> UClassicFunctionLibrary::LoadXML(FString XMLString, FString AccessString)
{
	EEasyXMLParserErrorCode Result;
	FString ErrorMessage;
	UEasyXMLElement* ElementXML = UEasyXMLParseManager::LoadFromString(XMLString, Result, ErrorMessage);
	EEasyXMLParserFound Results;
	return ElementXML->ReadElements(AccessString, Results);
}

UEasyXMLElement* UClassicFunctionLibrary::LoadXMLSingle(FString XMLString, FString AccessString)
{
	EEasyXMLParserErrorCode Result;
	FString ErrorMessage;
	UEasyXMLElement* ElementXML = UEasyXMLParseManager::LoadFromString(XMLString, Result, ErrorMessage);
	EEasyXMLParserFound Results;
	return ElementXML->ReadElement(AccessString, Results);
}

void UClassicFunctionLibrary::SetConfig(UEasyXMLElement* Element, FConfig& Config)
{
	Config.DefaultStartSystem = Element->ReadString(TEXT("defaultstartsystem"));
	Config.PathMedia = Element->ReadString(TEXT("pathmedia"));
	Config.Rendering = Element->ReadBool(TEXT("rendering"));
	Config.Volume = Element->ReadInt(TEXT("volume"));
}

void UClassicFunctionLibrary::SetGameSystem(TArray<UEasyXMLElement*>  Elements, TArray<FGameSystem>& ConfigSystems)
{
	FGameSystem  ConfigSystem;

	for (UEasyXMLElement* Element : Elements)
	{
		ConfigSystem.Arguments = Element->ReadString(TEXT("arguments"));
		ConfigSystem.Executable = Element->ReadString(TEXT("executable"));
		//ConfigSystem.ImageX = Element->ReadInt(TEXT("imagex"));
		//ConfigSystem.ImageY = Element->ReadInt(TEXT("imagey"));
		ConfigSystem.RomPath = Element->ReadString(TEXT("rompath"));
		ConfigSystem.SystemLabel = Element->ReadString(TEXT("systemlabel"));
		ConfigSystem.SystemName = Element->ReadString(TEXT("systemname"));
		ConfigSystems.Add(ConfigSystem);
	}
}

void UClassicFunctionLibrary::SetGameData(TArray<UEasyXMLElement*> Elements, TArray<FGameData>& GameDatas, UTexture2D* Texture)
{

	FGameData  GameData;
	int32 Index = 0;

	for (UEasyXMLElement* Element : Elements)
	{
		GameData.MapIndex = Index;
		GameData.Path = Element->ReadString(TEXT("path"));
		GameData.name = Element->ReadString(TEXT("name"));
		GameData.desc = Element->ReadString(TEXT("desc"));
		GameData.rating = Element->ReadString(TEXT("rating"));
		GameData.releasedate = Element->ReadString(TEXT("releasedate"));
		GameData.developer = Element->ReadString(TEXT("developer"));
		GameData.publisher = Element->ReadString(TEXT("publisher"));
		GameData.genre = Element->ReadString(TEXT("genre"));
		GameData.players = Element->ReadString(TEXT("players"));
		GameData.hash = Element->ReadString(TEXT("hash"));
		GameData.image = Element->ReadString(TEXT("image"));
		GameData.thumbnail = Element->ReadString(TEXT("thumbnail"));
		GameData.video = Element->ReadString(TEXT("video"));
		GameData.genreid = Element->ReadString(TEXT("genreid"));
		GameData.favorite = Element->ReadBool(TEXT("favorite"));
		GameData.playcount = Element->ReadInt(TEXT("playcount"));
		GameData.lastplayed = Element->ReadString(TEXT("lastplayed"));
		GameData.Executable = Element->ReadString(TEXT("executable"));
		GameData.Arguments = Element->ReadString(TEXT("arguments"));
		//GameData.ImageX = Element->ReadInt(TEXT("imagex"));
		//GameData.ImageY = Element->ReadInt(TEXT("imagey"));
		//GameData.Texture = Texture;
		GameDatas.Add(GameData);

		Index += 1;
	}
}

void UClassicFunctionLibrary::FormatGameData(TArray<FGameData>& GameDatas, FConfig Config, FGameSystem GameSystem)
{
	for (FGameData& GameData : GameDatas)
	{
		GameData.PathFormated = ReplacePath(GameData.Path, GameSystem.RomPath);
		GameData.PathFormated = TEXT("\"") + GameData.PathFormated + TEXT("\"");
		GameData.nameFormated = GameData.name.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
		GameData.descFormated = GameData.desc.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
		//GameData.ImageX = (GameData.ImageX == 0) ? GameSystem.ImageX : GameData.ImageX;
		//GameData.ImageY = (GameData.ImageY == 0) ? GameSystem.ImageY : GameData.ImageY;
		GameData.imageFormated = ReplaceMedia(GameData.image, Config.PathMedia, GameSystem.RomPath, GameData.Path, GameSystem.SystemName, TEXT("covers"), TEXT(".png"), GameData.PathFormated);
		GameData.thumbnailFormated = ReplaceMedia(GameData.thumbnail, Config.PathMedia, GameSystem.RomPath, GameData.Path, GameSystem.SystemName, TEXT("screenshots"), TEXT(".png"), GameData.PathFormated);
		GameData.videoFormated = ReplaceMedia(GameData.video, Config.PathMedia, GameSystem.RomPath, GameData.Path, GameSystem.SystemName, TEXT("videos"), TEXT(".mp4"), GameData.PathFormated);
	}
}

bool UClassicFunctionLibrary::FindGameData(TArray<FGameData> datas, FGameData DataElement, int32& Index, int32 Find)
{

	if (Find == DataElement.MapIndex)
	{
		Index = DataElement.MapIndex;
		return true;
	}

	Index = -1;

	if (DataElement.MapIndex == -1) return false;

	for (FGameData& Data : datas)
	{
		Index++;
		if (Data.MapIndex == DataElement.MapIndex)
		{
			return true;
		}
	}
	Index = -1;

	return false;
}

TArray<FGameData> UClassicFunctionLibrary::FilterGameData(TArray<FGameData> GameData, EGamesFilter Filter, int32& Num)
{
	Num = 0;
	switch (Filter)
	{
	case EGamesFilter::DEFAULT:
		Num = CountFavorites(GameData);
		return GameData;
	case EGamesFilter::FAVORITES_FIRST:
		Num = FilterFavoriteGameData(GameData, false);
		break;
	case EGamesFilter::FAVORITES_ONLY:
		Num = FilterFavoriteGameData(GameData, true);
		break;
	default:
		return GameData;
	}
	return GameData;
}

int32 UClassicFunctionLibrary::FilterFavoriteGameData(TArray<FGameData>& GameData, bool bOnlyFavorites)
{
	TArray<FGameData> FilterGameDataFavorite;
	TArray<FGameData> FilterGameData;
	int32 Num = 0;

	for (FGameData& Data : GameData)
	{
		if (Data.favorite)
		{
			FilterGameDataFavorite.Add(Data);
			Num++;
			continue;
		}
		if (bOnlyFavorites) continue;

		FilterGameData.Add(Data);
	}
	if (!bOnlyFavorites)
	{
		for (FGameData& Data : FilterGameData)
		{
			FilterGameDataFavorite.Add(Data);
		}
	}
	GameData.Empty();
	GameData = FilterGameDataFavorite;
	return Num;

}

int32 UClassicFunctionLibrary::CountFavorites(TArray<FGameData> GameData)
{
	int32 Num = 0;
	for (FGameData& Data : GameData)
	{
		if (Data.favorite)
		{
			Num++;
		}
	}
	return Num;
}

bool UClassicFunctionLibrary::ClassicGetFiles(TArray<FString>& Files, FString RootFolderFullPath, FString Ext)
{
	if (RootFolderFullPath.Len() < 1) return false;

	FPaths::NormalizeDirectoryName(RootFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();

	if (!Ext.Contains(TEXT("*")))
	{
		if (Ext == "")
		{
			Ext = "*.*";
		}
		else
		{
			Ext = (Ext.Left(1) == ".") ? "*" + Ext : "*." + Ext;
		}
	}

	const FString FinalPath = RootFolderFullPath + "/" + Ext;

	FileManager.FindFiles(Files, *FinalPath, true, false);
	return true;
}

bool UClassicFunctionLibrary::LoadStringFile(FString& Result, FString FullFilePath)
{
	return FFileHelper::LoadFileToString(Result, *FullFilePath);
}

void UClassicFunctionLibrary::CreateFolders(FString Path, TArray<FGameSystem> GameSystems)
{
	const FString PathMedia = (Path != TEXT("")) ? Path : GetGameRootDirectory() + TEXT("media");

	VerifyOrCreateDirectory(PathMedia);

	for (FGameSystem& GameSystemElement : GameSystems)
	{
		VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName);
		VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName + TEXT("\\covers"));
		VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName + TEXT("\\screenshots"));
		VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName + TEXT("\\videos"));
	}
}

static EImageFormat GetJoyImageFormat(EClassicImageFormat JoyFormat)
{
	switch (JoyFormat)
	{
	case EClassicImageFormat::JPG: return EImageFormat::JPEG;
	case EClassicImageFormat::PNG: return EImageFormat::PNG;
	case EClassicImageFormat::BMP: return EImageFormat::BMP;
	case EClassicImageFormat::ICO: return EImageFormat::ICO;
	case EClassicImageFormat::EXR: return EImageFormat::EXR;
	case EClassicImageFormat::ICNS: return EImageFormat::ICNS;
	}
	return EImageFormat::JPEG;
}

static TextureFilter GetTextureFilter(EClassicTextureFilter Filter)
{
	switch (Filter)
	{
	case EClassicTextureFilter::NEAREST: return TF_Nearest;
	case EClassicTextureFilter::DEFAULT: return TF_Default;
	case EClassicTextureFilter::MAX: return TF_MAX;
	case EClassicTextureFilter::BILINEAR: return TF_Bilinear;
	case EClassicTextureFilter::TRILINEAR: return TF_Trilinear;
	}
	return TF_Nearest;
}

EClassicImageFormat UClassicFunctionLibrary::GetFormatImage(const FString& FullFilePath)
{
	if (FullFilePath.Contains("png")) {
		return EClassicImageFormat::PNG;
	}
	if (FullFilePath.Contains("jpg")) {
		return EClassicImageFormat::JPG;
	}
	if (FullFilePath.Contains("bmp")) {
		return EClassicImageFormat::BMP;
	}
	if (FullFilePath.Contains("ico")) {
		return EClassicImageFormat::ICO;
	}
	return EClassicImageFormat::PNG;
}

UTexture2D* UClassicFunctionLibrary::LoadTexture2DFromFile(const FString& FullFilePath, EClassicImageFormat ImageFormat, EClassicTextureFilter Filter, int32& Width, int32& Height)
{

	IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

	UTexture2D* NewTexture = nullptr;
	TArray64<uint8> Buffer;

	if (FFileHelper::LoadFileToArray(Buffer, *FullFilePath))
	{
		EPixelFormat PixelFormat = PF_Unknown;
		EImageFormat Format = GetJoyImageFormat(ImageFormat);


		if (Format != EImageFormat::Invalid)
		{
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);

			int32 BitDepth = 0;

			// Create texture
			if (ImageWrapper->SetCompressed((void*)Buffer.GetData(), Buffer.Num()))
			{

				PixelFormat = PF_Unknown;
				ERGBFormat RGBFormat = ERGBFormat::Invalid;

				BitDepth = ImageWrapper->GetBitDepth();

				Width = ImageWrapper->GetWidth();
				Height = ImageWrapper->GetHeight();

				if (BitDepth == 16)
				{
					PixelFormat = PF_FloatRGBA;
					RGBFormat = ERGBFormat::RGBAF;
				}
				else if (BitDepth == 8)
				{
					PixelFormat = PF_B8G8R8A8;
					RGBFormat = ERGBFormat::BGRA;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Bit depth is unsupported. (%d)"), BitDepth);
					return nullptr;
				}

				TArray64<uint8> UncompressedData;
				ImageWrapper->GetRaw(RGBFormat, BitDepth, UncompressedData);

				// Load texture
				const FString& BaseFilename = FPaths::GetBaseFilename(FullFilePath);

				// Load texture
				NewTexture = CreateUniqueTransient(Width, Height, PixelFormat, *BaseFilename);
				if (NewTexture)
				{
					NewTexture->bNotOfflineProcessed = true;
					uint8* MipData = static_cast<uint8*>(NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

					// Bulk data was already allocated for the correct size when we called CreateTransient above
					FMemory::Memcpy(MipData, UncompressedData.GetData(), NewTexture->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize());

					// Copy texture and update
					NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
					NewTexture->Filter = GetTextureFilter(Filter);
					NewTexture->UpdateResource();
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Couldn't determine the file format"));
		}
	}
	return NewTexture;
}

void UClassicFunctionLibrary::AsyncLoadTexture2DFromFile(FLoadImageDelegate Out, const FString FullFilePath, int32 Index, EClassicImageFormat ImageFormat, EClassicTextureFilter Filter)
{
	if (!FPaths::FileExists(FullFilePath)) 
	{
		Out.ExecuteIfBound(nullptr, Index, false);
		UE_LOG(LogTemp, Warning, TEXT("File Not Exists. %s"), *FullFilePath);
		return;
	}

	AsyncTask(ENamedThreads::AnyThread, [=]()
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
		
		TArray64<uint8> Buffer;

		if (FFileHelper::LoadFileToArray(Buffer, *FullFilePath))
		{
			EPixelFormat PixelFormat = PF_Unknown;
			EImageFormat Format = GetJoyImageFormat(ImageFormat);
			int32 Width = 0;
			int32 Height = 0;

			if (Format != EImageFormat::Invalid)
			{
				TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);

				int32 BitDepth = 0;

				// Create texture
				if (ImageWrapper->SetCompressed((void*)Buffer.GetData(), Buffer.Num()))
				{

					PixelFormat = PF_Unknown;
					ERGBFormat RGBFormat = ERGBFormat::Invalid;

					BitDepth = ImageWrapper->GetBitDepth();

					Width = ImageWrapper->GetWidth();
					Height = ImageWrapper->GetHeight();

					if (BitDepth == 16)
					{
						PixelFormat = PF_FloatRGBA;
						RGBFormat = ERGBFormat::RGBAF;
					}
					else if (BitDepth == 8)
					{
						PixelFormat = PF_B8G8R8A8;
						RGBFormat = ERGBFormat::BGRA;
					}
					else
					{
						Out.ExecuteIfBound(nullptr, Index, false);
						UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Bit depth is unsupported. (%d)"), BitDepth);
						return;
					}

					TArray64<uint8> UncompressedData;
					ImageWrapper->GetRaw(RGBFormat, BitDepth, UncompressedData);

					AsyncTask(ENamedThreads::GameThread, [=]()
					{
						const FString& BaseFilename = FPaths::GetBaseFilename(FullFilePath);
						bool bSuccessfull = false;

						// Load texture
						UTexture2D* NewTexture = CreateUniqueTransient(Width, Height, PixelFormat, *BaseFilename);
						if (NewTexture)
						{

							NewTexture->NeverStream = true;
							NewTexture->bNotOfflineProcessed = true;

							uint8* MipData = static_cast<uint8*>(NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
							// Bulk data was already allocated for the correct size when we called CreateTransient above
							FMemory::Memcpy(MipData, UncompressedData.GetData(), NewTexture->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize());
							NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

							// Copy texture set parameters and update
							NewTexture->Filter = GetTextureFilter(Filter);
							NewTexture->UpdateResource();
							bSuccessfull = true;
						}
						// execute function 
						Out.ExecuteIfBound(NewTexture, Index, bSuccessfull);
						
					});
					return; // early return successful!!
				}
			}
		}
		// execute function
		Out.ExecuteIfBound(nullptr, Index, false);
	});
}

UTexture2D* UClassicFunctionLibrary::CreateUniqueTransient(int32 Width, int32 Height, EPixelFormat PixelFormat, const FName BaseFilename)
{
	if (Width > 0 && Height > 0 && (Width % GPixelFormats[PixelFormat].BlockSizeX) == 0 && (Height % GPixelFormats[PixelFormat].BlockSizeY) == 0)
	{
		UTexture2D* NewTexture = NewObject<UTexture2D>(
			GetTransientPackage(),
			MakeUniqueObjectName(GetTransientPackage(), UTexture2D::StaticClass(), BaseFilename),
			RF_Public | RF_Transient
		);

		NewTexture->SetPlatformData(new FTexturePlatformData());
		NewTexture->GetPlatformData()->SizeX = Width;
		NewTexture->GetPlatformData()->SizeY = Height;
		NewTexture->GetPlatformData()->PixelFormat = PixelFormat;

		// Allocate first mipmap.
		const int32 NumBlocksX = Width / GPixelFormats[PixelFormat].BlockSizeX;
		const int32 NumBlocksY = Height / GPixelFormats[PixelFormat].BlockSizeY;
		FTexture2DMipMap* Mip = new FTexture2DMipMap();
		NewTexture->GetPlatformData()->Mips.Add(Mip);
		Mip->SizeX = Width;
		Mip->SizeY = Height;
		Mip->BulkData.Lock(LOCK_READ_WRITE);
		Mip->BulkData.Realloc(NumBlocksX * NumBlocksY * GPixelFormats[PixelFormat].BlockBytes);
		Mip->BulkData.Unlock();
		return NewTexture;
	}
	return nullptr;
}

void UClassicFunctionLibrary::CreateProcess(int32& ProcessId, FString FullPath, TArray<FString> CommandlineArgs, bool Detach, bool Hidden, int32 Priority, FString OptionalWorkingDirectory)
{
	FString Args = "";
	if (CommandlineArgs.Num() > 1)
	{
		Args = CommandlineArgs[0];
		for (int32 v = 1; v < CommandlineArgs.Num(); v++)
		{
			Args += " " + CommandlineArgs[v];
		}
	}
	else if (CommandlineArgs.Num() > 0)
	{
		Args = CommandlineArgs[0];
	}

	uint32 NeedBPUINT32 = 0;
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*FullPath,
		*Args,
		Detach,//* @param bLaunchDetached		if true, the new process will have its own window
		false,//* @param bLaunchHidded			if true, the new process will be minimized in the task bar
		Hidden,//* @param bLaunchReallyHidden	if true, the new process will not have a window or be in the task bar
		&NeedBPUINT32,
		Priority,
		(OptionalWorkingDirectory != "") ? *OptionalWorkingDirectory : nullptr,//const TCHAR* OptionalWorkingDirectory, 
		nullptr
	);

	//Not sure what to do to expose UINT32 to BP
	ProcessId = NeedBPUINT32;
}

FString UClassicFunctionLibrary::FormatDateToView(FString DateXml)
{
	if (DateXml.IsEmpty()) return TEXT("");

	const TArray<FString> DateFormat = UKismetStringLibrary::GetCharacterArrayFromString(DateXml);
	FString Date;
	FString Mounth;
	FString Year;


	for (int32 i = 0; i < DateFormat.Num(); i++)
	{
		if (i < 4)
		{
			Year += DateFormat[i];
		}
		else if (i < 6)
		{
			Mounth += DateFormat[i];
		}
		else if (i < 8)
		{
			Date += DateFormat[i];
		}
	}
	//2022-12-31
	return Year + TEXT("-") + Mounth + TEXT("-") + Date;
}

FString UClassicFunctionLibrary::FormatDateTimeToView(FString DateXml)
{
	const TArray<FString> DateFormat = UKismetStringLibrary::GetCharacterArrayFromString(DateXml);
	FString Date = FormatDateToView(DateXml);

	FString Hour;
	FString Minute;

	for (int32 i = 9; i < DateFormat.Num(); i++)
	{
		if (i < 11)
		{
			Hour += DateFormat[i];
		}
		else if (i < 13)
		{
			Minute += DateFormat[i];
		}

	}
	//2022-12-31 22:30
	return Date + TEXT("  ") + Hour + TEXT(":") + Minute;
}

FString UClassicFunctionLibrary::FormatDateToXml()
{
	const FDateTime DateTime = FDateTime::Now();
	return DateTime.ToString(TEXT("%Y%m%dT%H%M%S"));
}

FColor UClassicFunctionLibrary::HexToColor(FString HexString)
{
	return FColor::FromHex(HexString);
}

FString UClassicFunctionLibrary::ColorToHex(FColor Color)
{
	return Color.ToHex();
}

FString UClassicFunctionLibrary::GetProjectVersion()
{
	if (!GConfig) return "";

	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}

FString UClassicFunctionLibrary::GetProjectName()
{
	if (!GConfig) return "";

	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectName"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}



