// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicFunctionLibrary.h"
#include "HAL/RunnableThread.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/DateTime.h"
#include "EasyXMLParseManager.h"



EUINavigation UClassicFunctionLibrary::GetInputnavigation(const FKeyEvent& InKeyEvent)
{
	const FString KeyEvent = InKeyEvent.GetKey().ToString();

	if (KeyEvent == "Left" || KeyEvent == "Gamepad_DPad_Left" || KeyEvent == "Gamepad_LeftStick_Left") {
		return EUINavigation::Left;
	}
	else if (KeyEvent == "Right" || KeyEvent == "Gamepad_DPad_Right" || KeyEvent == "Gamepad_LeftStick_Right") {
		return EUINavigation::Right;
	}
	else if (KeyEvent == "Up" || KeyEvent == "Gamepad_DPad_Up" || KeyEvent == "Gamepad_LeftStick_Up") {
		return EUINavigation::Up;
	}
	else if (KeyEvent == "Down" || KeyEvent == "Gamepad_DPad_Down" || KeyEvent == "Gamepad_LeftStick_Down") {
		return EUINavigation::Down;
	}
	else {
		return EUINavigation::Invalid;
	}
}

EButtonsGame UClassicFunctionLibrary::GetInputButton(const FKeyEvent& InKeyEvent)
{
	const FString KeyEvent = InKeyEvent.GetKey().ToString();

	if (KeyEvent == "Left" || KeyEvent == "Gamepad_DPad_Left" || KeyEvent == "Gamepad_LeftStick_Left") {
		return EButtonsGame::LEFT;
	}
	else if (KeyEvent == "Right" || KeyEvent == "Gamepad_DPad_Right" || KeyEvent == "Gamepad_LeftStick_Right") {
		return EButtonsGame::RIGHT;
	}
	else if (KeyEvent == "Up" || KeyEvent == "Gamepad_DPad_Up" || KeyEvent == "Gamepad_LeftStick_Up") {
		return EButtonsGame::UP;
	}
	else if (KeyEvent == "Down" || KeyEvent == "Gamepad_DPad_Down" || KeyEvent == "Gamepad_LeftStick_Down") {
		return EButtonsGame::DOWN;
	}
	else if (KeyEvent == "Enter" || KeyEvent == "Gamepad_FaceButton_Bottom") {
		return EButtonsGame::A;
	}
	else if (KeyEvent == "Backspace" || KeyEvent == "Gamepad_FaceButton_Right") {
		return EButtonsGame::B;
	}
	else if (KeyEvent == "F" || KeyEvent == "Gamepad_FaceButton_Top") {
		return EButtonsGame::Y;
	}
	else if (KeyEvent == "S" || KeyEvent == "Gamepad_FaceButton_Left") {
		return EButtonsGame::X;
	}
	else if (KeyEvent == "Q" || KeyEvent == "Gamepad_LeftShoulder") {
		return EButtonsGame::LB;
	}
	else if (KeyEvent == "E" || KeyEvent == "Gamepad_RightShoulder") {
		return EButtonsGame::RB;
	}
	else if (KeyEvent == "A" || KeyEvent == "Gamepad_LeftTrigger") {
		return EButtonsGame::LT;
	}
	else if (KeyEvent == "D" || KeyEvent == "Gamepad_RightTrigger") {
		return EButtonsGame::RT;
	}
	else if (KeyEvent == "LeftControl" || KeyEvent == "Gamepad_Special_Right") {
		return EButtonsGame::START;
	}
	else if (KeyEvent == "Alt" || KeyEvent == "Gamepad_Special_Left") {
		return EButtonsGame::SELECT;
	}
	else {
		return EButtonsGame::NONE;
	}
}

TArray<FGameData> UClassicFunctionLibrary::SortGameDate(TArray<FGameData> gameData)
{
	TArray<FString> Names;
	TArray<FGameData> NewGameData;

	for (FGameData& data : gameData)
	{
		Names.Add(data.name);
	}
	Names.Sort();
	for (FString& name : Names)
	{
		for (FGameData& data : gameData)
		{
			if (data.name == name && data.name != TEXT("null_value")) {
				NewGameData.Add(data);
				data.name = TEXT("null_value");
			}
		}
	}

	return NewGameData;
}

TArray<FConfigSystem> UClassicFunctionLibrary::SortConfigSystem(TArray<FConfigSystem> configData)
{
	TArray<FString> Names;
	TArray<FConfigSystem> NewConfigData;

	for (FConfigSystem& data : configData)
	{
		Names.Add(data.SystemLabel);
	}
	Names.Sort();
	for (FString& name : Names)
	{
		for (FConfigSystem& data : configData)
		{
			if (data.SystemLabel == name && data.SystemLabel != TEXT("null_value")) {
				NewConfigData.Add(data);
				data.SystemLabel = TEXT("null_value");
			}
		}
	}

	return NewConfigData;
}

FString UClassicFunctionLibrary::GetGameRootDirectory()
{
	FString RootGameDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	RootGameDir = RootGameDir.FString::Replace(TEXT("/"), TEXT("\\"));
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

bool UClassicFunctionLibrary::InvertBool(bool value)
{
	return !value;
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
FString UClassicFunctionLibrary::ReplaceMedia(FString OriginalPathMedia, FString PathMedia, FString PathRom, FString RomName, FString SystemName, FString TypeMedia, FString Format)
{
	FString NewImage = ReplacePath(OriginalPathMedia, PathRom);

	FString Rom = RomName.Replace(TEXT("./"), TEXT("\\"), ESearchCase::IgnoreCase);
	Rom = RomName.Replace(TEXT("/"), TEXT("\\"), ESearchCase::IgnoreCase);

	if (!FPaths::FileExists(NewImage))
	{
		for (int32 StartIndex = Rom.Len(); StartIndex >= 0 ; StartIndex--)
		{
			if (Rom.Mid(StartIndex, 1) == TEXT("\\"))
			{
				NewImage = Rom.Mid(StartIndex, Rom.Len());
				StartIndex = -1;
			}
			else if (StartIndex == 0)
			{
				NewImage = Rom;
			}
		}

		for (int32 StartIndex = NewImage.Len(); StartIndex >= NewImage.Len() - 4; StartIndex--)
		{
			if (NewImage.Mid(StartIndex, 1) == TEXT("."))
			{
				NewImage = NewImage.Mid(0, StartIndex);
				StartIndex = -1;
			}
		}

		NewImage = PathMedia  + TEXT("\\") + SystemName + TEXT("\\") + TypeMedia + NewImage + Format;
		UE_LOG(LogTemp, Warning, TEXT("Path out is  %s"), *NewImage);
	}
	
	return NewImage;

}



FString UClassicFunctionLibrary::CreateXMLGameFile(TArray<FGameData> gameData, FVector2D IgnoreImageSize)
{
	FString finalData;

	finalData += TEXT("<?xml version=\"1.0\"?>\n<gameList>\n");

	for (FGameData& data : gameData)
	{
		finalData += TEXT("<game>\n");
		finalData += GenerateXmlTag(TEXT("path"), data.Path);
		finalData += GenerateXmlTag(TEXT("name"), data.name);
		finalData += GenerateXmlTag(TEXT("desc"), data.desc);
		finalData += GenerateXmlTag(TEXT("rating"), data.rating);
		finalData += GenerateXmlTag(TEXT("releasedate"), data.releasedate);
		finalData += GenerateXmlTag(TEXT("developer"), data.developer);
		finalData += GenerateXmlTag(TEXT("publisher"), data.publisher);
		finalData += GenerateXmlTag(TEXT("genre"), data.genre);
		finalData += GenerateXmlTag(TEXT("players"), data.players);
		finalData += GenerateXmlTag(TEXT("hash"), data.hash);
		finalData += GenerateXmlTag(TEXT("image"), data.image);
		finalData += GenerateXmlTag(TEXT("thumbnail"), data.thumbnail);
		finalData += GenerateXmlTag(TEXT("video"), data.video);
		finalData += GenerateXmlTag(TEXT("genreid"), data.genreid);
		finalData += GenerateXmlTag(TEXT("favorite"), (data.favorite) ? TEXT("true") : TEXT("false"));
		finalData += GenerateXmlTag(TEXT("playcount"), FString::SanitizeFloat(data.playcount, 0));
		finalData += GenerateXmlTag(TEXT("lastplayed"), data.lastplayed);
		finalData += GenerateXmlTag(TEXT("executable"), data.Executable);
		finalData += GenerateXmlTag(TEXT("arguments"), data.Arguments);
		if (data.ImageX != IgnoreImageSize.X)
		{
			finalData += GenerateXmlTag(TEXT("imagex"), FString::SanitizeFloat(data.ImageX, 0));
		}
		if (data.ImageX != IgnoreImageSize.X)
		{
			finalData += GenerateXmlTag(TEXT("imagey"), FString::SanitizeFloat(data.ImageY, 0));
		}

		finalData += TEXT("</game>\n");

	}

	finalData += TEXT("</gameList>\n");
	return finalData;
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

FConfig UClassicFunctionLibrary::SetConfig(UEasyXMLElement* element)
{
	FConfig  data;
	data.defaultstartsystem = element->ReadString(TEXT("defaultstartsystem"));
	data.pathmedia = element->ReadString(TEXT("pathmedia"));
	data.rendering = element->ReadBool(TEXT("rendering"));

	return data;
}

TArray<FConfigSystem> UClassicFunctionLibrary::SetConfigSystem(TArray<UEasyXMLElement*>  elements)
{
	TArray<FConfigSystem> datas;
	FConfigSystem  data;

	for (UEasyXMLElement* element : elements)
	{
		data.Arguments = element->ReadString(TEXT("arguments"));
		data.Executable = element->ReadString(TEXT("executable"));
		data.ImageX = element->ReadInt(TEXT("imagex"));
		data.ImageY = element->ReadInt(TEXT("imagey"));
		data.RomPath = element->ReadString(TEXT("rompath"));
		data.SystemLabel = element->ReadString(TEXT("systemlabel"));
		data.SystemName = element->ReadString(TEXT("systemname"));
		datas.Add(data);
	}

	return datas;
}


TArray<FGameData> UClassicFunctionLibrary::SetGameData(TArray<UEasyXMLElement*>  elements)
{

	TArray<FGameData> datas;
	FGameData  data;

	for (UEasyXMLElement* element : elements)
	{
		data.Path = element->ReadString(TEXT("path"));
		data.name = element->ReadString(TEXT("name"));
		data.desc = element->ReadString(TEXT("desc"));
		data.rating = element->ReadString(TEXT("rating"));
		data.releasedate = element->ReadString(TEXT("releasedate"));
		data.developer = element->ReadString(TEXT("developer"));
		data.publisher = element->ReadString(TEXT("publisher"));
		data.genre = element->ReadString(TEXT("genre"));
		data.players = element->ReadString(TEXT("players"));
		data.hash = element->ReadString(TEXT("hash"));
		data.image = element->ReadString(TEXT("image"));
		data.thumbnail = element->ReadString(TEXT("thumbnail"));
		data.video = element->ReadString(TEXT("video"));
		data.genreid = element->ReadString(TEXT("genreid"));
		data.favorite = element->ReadBool(TEXT("favorite"));
		data.playcount = element->ReadInt(TEXT("playcount"));
		data.lastplayed = element->ReadString(TEXT("lastplayed"));
		data.Executable = element->ReadString(TEXT("executable"));
		data.Arguments = element->ReadString(TEXT("arguments"));
		data.ImageX = element->ReadInt(TEXT("imagex"));
		data.ImageY = element->ReadInt(TEXT("imagey"));
		datas.Add(data);
	}

	return datas;
}

TArray<FGameData> UClassicFunctionLibrary::FormatGameData(TArray<FGameData> datas, FConfig config, FConfigSystem configSystem)
{
	for (FGameData& data : datas)
	{
		data.imageFormated = ReplaceMedia(data.image, config.pathmedia, configSystem.RomPath, data.Path, configSystem.SystemName, TEXT("covers"), TEXT(".png"));
		data.thumbnailFormated = ReplaceMedia(data.thumbnail, config.pathmedia, configSystem.RomPath, data.Path, configSystem.SystemName, TEXT("screenshots"), TEXT(".png"));
		data.videoFormated = ReplaceMedia  (data.video, config.pathmedia , configSystem.RomPath , data.Path , configSystem.SystemName , TEXT("videos") , TEXT(".mp4"));
		data.PathFormated = ReplacePath(data.Path, configSystem.RomPath);
		data.PathFormated = TEXT("\"") + data.PathFormated + TEXT("\"");
		data.nameFormated = data.name.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
		data.descFormated = data.desc.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
		data.ImageX = (data.ImageX == 0) ? configSystem.ImageX : data.ImageX;
		data.ImageY = (data.ImageY == 0) ? configSystem.ImageY : data.ImageY;
	}

	return datas;
}

bool UClassicFunctionLibrary::FindGameData(TArray<FGameData> datas, FGameData DataElement, int32& Index)
{
	Index = -1;
	for (FGameData& data : datas)
	{	
		Index++;
		if (data.PathFormated == DataElement.PathFormated) 
		{
			return true;
		}
	}
	Index = -1;
	return false;
}

TArray<FGameData> UClassicFunctionLibrary::FilterFavoriteGameData(TArray<FGameData> datas, bool filterFavorites)
{
	if (filterFavorites) {
		TArray<FGameData> FilterData;
		for (FGameData& data : datas)
		{
			if (data.favorite) {
				FilterData.Add(data);
			}
		}
		return FilterData;
	}
	else {
		return datas;
	}

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

	FString FinalPath = RootFolderFullPath + "/" + Ext;

	FileManager.FindFiles(Files, *FinalPath, true, false);
	return true;
}

bool UClassicFunctionLibrary::LoadStringFile(FString& Result, FString FullFilePath)
{
	return FFileHelper::LoadFileToString(Result, *FullFilePath);
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



UTexture2D* UClassicFunctionLibrary::LoadTexture2DFromFile(const FString& FullFilePath, bool& IsValid, EClassicImageFormat ImageFormat, int32& Width, int32& Height)
{

	IsValid = false;
	UTexture2D* NewTexture = NULL;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(GetJoyImageFormat(ImageFormat));

	//Load From File
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath))
	{
		return NULL;
	}

	//Create Texture2D!
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		TArray<uint8> UncompressedBGRA;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			//Update!
			NewTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			//Valid?
			if (!NewTexture)
			{
				return NULL;
			}

			//Out!
			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			//Copy!
			void* TextureData = NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

			NewTexture->UpdateResource();

		}
	}


	IsValid = true;
	return NewTexture;
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







void UClassicFunctionLibrary::AsyncLoadTexture2DFromFile(FLoadImageDelegate Out, const FString FullFilePath, int32 Index)
{
	AsyncTask(ENamedThreads::GameThread_Local, [Out, FullFilePath, Index]()
		{
			bool isValid = false;
			int32 Size = 64;
			UTexture2D* Texture = LoadTexture2DFromFile(FullFilePath, isValid, EClassicImageFormat::PNG, Size, Size);
			if (isValid)
			{
				Out.ExecuteIfBound(Texture, Index);
			}
		});

}

FString UClassicFunctionLibrary::FormatDateToView(FString DateXml)
{

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
	FDateTime DateTime = FDateTime::Now();
	return DateTime.ToString(TEXT("%Y%m%dT%H%M%S"));
}








