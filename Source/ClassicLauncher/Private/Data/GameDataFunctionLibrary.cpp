// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Data/GameDataFunctionLibrary.h"

#include "EasyXMLParseManager.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"

void UGameDataFunctionLibrary::SortConfigSystem(TArray<FGameSystem>& GameSystems, const bool bAscending)
{
	GameSystems.Sort([bAscending](const FGameSystem& A, const FGameSystem& B)
	{
		return (bAscending) ? A.SystemLabel < B.SystemLabel : A.SystemLabel > B.SystemLabel;
	});
}

void UGameDataFunctionLibrary::SortGameData(TArray<FGameData>& GameData, const bool bAscending)
{
	GameData.Sort([bAscending](const FGameData& A, const FGameData& B)
	{
		return (bAscending) ? A.name < B.name : A.name > B.name;
	});
}

TArray<FGameData> UGameDataFunctionLibrary::FilterGameDataFavoritesFirst(TArray<FGameData>& GameData, const bool bOnlyFavorites)
{
	auto Favorites = GameData.FilterByPredicate([](const FGameData& A)
	{
		return A.favorite == true;
	});

	if (!bOnlyFavorites)
	{
		const auto NoFavorites = GameData.FilterByPredicate([](const FGameData& A)
		{
			return A.favorite == false;
		});
		Favorites += NoFavorites;
	}
	return Favorites;
}

void UGameDataFunctionLibrary::FilterGameDataMostPlayed(TArray<FGameData>& GameData)
{
	GameData.Sort([](const FGameData& A, const FGameData& B)
	{
		return A.playcount < B.playcount;
	});
}

void UGameDataFunctionLibrary::FilterGameDataLastPlayed(TArray<FGameData>& GameData)
{
	GameData.Sort([](const FGameData& A, const FGameData& B)
	{
		return A.lastplayed < B.lastplayed; //is not working todo create a class XMLDate 
	});
}

bool UGameDataFunctionLibrary::SaveGameListXML(FString& GameListPath, TArray<FGameData>& NewGames)
{
	const FString NewXMLFile = CreateXMLGameFile(NewGames);
	return UClassicFunctionLibrary::SaveStringToFile(GameListPath, TEXT("gamelist.xml"), NewXMLFile, true, false);
}

void UGameDataFunctionLibrary::SaveConfigurationData(const FConfig ConfigurationData)
{
	FString XmlConfig = CreateXMLConfigFile(ConfigurationData);
	XmlConfig = XmlConfig.Replace(TEXT("$(remove)"), TEXT(""), ESearchCase::IgnoreCase);
	const FString PathToSave = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config");
	const bool Saved = (UClassicFunctionLibrary::SaveStringToFile(PathToSave, TEXT("config.xml"), XmlConfig, true, false));
	UE_LOG(LogTemp, Warning, TEXT("%s"), (Saved) ? TEXT("Saved File") : TEXT("Not Saved File"));
}

FString UGameDataFunctionLibrary::ReplacePath(FString Value, FString Path)
{
	Path = Path + TEXT("\\");
	Value = Value.Replace(TEXT("/"), TEXT("\\"), ESearchCase::IgnoreCase);
	Value = Value.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
	Value = Value.Replace(TEXT(".\\"), *Path, ESearchCase::IgnoreCase);
	return Value;
}

FString UGameDataFunctionLibrary::CreateXMLGameFile(TArray<FGameData> GameData)
{
	FString NewGameData;

	NewGameData += TEXT("<?xml version=\"1.0\"?>\n<gameList>\n");

	for (FGameData& Data : GameData)
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
		NewGameData += TEXT("</game>\n");
	}

	NewGameData += TEXT("</gameList>\n");
	return NewGameData;
}

FString UGameDataFunctionLibrary::CreateXMLConfigFile(FConfig ConfigData)
{
	FString NewConfigXml;

	NewConfigXml += TEXT("<?xml version=\"1.0\"?>\n");

	NewConfigXml += TEXT("<config>\n");
	NewConfigXml += GenerateXmlTag(TEXT("pathmedia"), ConfigData.PathMedia);
	NewConfigXml += GenerateXmlTag(TEXT("defaultstartsystem"), ConfigData.DefaultStartSystem);
	NewConfigXml += GenerateXmlTag(TEXT("rendering"), (ConfigData.Rendering) ? TEXT("true") : TEXT("false"));
	NewConfigXml += GenerateXmlTag(TEXT("volumemaster"), FString::SanitizeFloat(ConfigData.VolumeMaster, 0));
	NewConfigXml += GenerateXmlTag(TEXT("volumemusic"), FString::SanitizeFloat(ConfigData.VolumeMusic, 0));
	NewConfigXml += GenerateXmlTag(TEXT("volumevideo"), FString::SanitizeFloat(ConfigData.VolumeVideo, 0));
	NewConfigXml += TEXT("</config>\n");

	return NewConfigXml;
}

FString UGameDataFunctionLibrary::GenerateXmlTag(FString TagName, FString Data)
{
	if (Data == TEXT("") || Data == TEXT("0"))
	{
		return TEXT("");
	}
	FString TagFinal;
	TagFinal += TEXT("\t<");
	TagFinal += TagName;
	TagFinal += TEXT(">");
	TagFinal += Data;
	TagFinal += TEXT("</");
	TagFinal += TagName;
	TagFinal += TEXT(">\n");

	return TagFinal;
}

TArray<UEasyXMLElement*> UGameDataFunctionLibrary::LoadXML(FString XMLString, FString AccessString)
{
	EEasyXMLParserErrorCode Result;
	FString ErrorMessage;
	UEasyXMLElement* ElementXML = UEasyXMLParseManager::LoadFromString(XMLString, Result, ErrorMessage);
	EEasyXMLParserFound Results;
	return ElementXML->ReadElements(AccessString, Results);
}

UEasyXMLElement* UGameDataFunctionLibrary::LoadXMLSingle(FString XMLString, FString AccessString)
{
	EEasyXMLParserErrorCode Result;
	FString ErrorMessage;
	UEasyXMLElement* ElementXML = UEasyXMLParseManager::LoadFromString(XMLString, Result, ErrorMessage);
	EEasyXMLParserFound Results;
	return ElementXML->ReadElement(AccessString, Results);
}

void UGameDataFunctionLibrary::SetConfig(UEasyXMLElement* Element, FConfig& Config)
{
	Config.DefaultStartSystem = Element->ReadString(TEXT("defaultstartsystem"));
	Config.PathMedia = Element->ReadString(TEXT("pathmedia"));
	Config.Rendering = Element->ReadBool(TEXT("rendering"));
	Config.VolumeMaster = Element->ReadInt(TEXT("volumemaster"));
	Config.VolumeMusic = Element->ReadInt(TEXT("volumemusic"));
	Config.VolumeVideo = Element->ReadInt(TEXT("volumevideo"));
}

void UGameDataFunctionLibrary::SetGameSystem(TArray<UEasyXMLElement*> Elements, TArray<FGameSystem>& ConfigSystems)
{
	FGameSystem ConfigSystem;
	for (UEasyXMLElement* Element : Elements)
	{
		ConfigSystem.RomPath = Element->ReadString(TEXT("rompath"));
		ConfigSystem.Arguments = Element->ReadString(TEXT("arguments"));
		ConfigSystem.Executable = Element->ReadString(TEXT("executable"));
		ConfigSystem.SystemLabel = Element->ReadString(TEXT("systemlabel"));
		ConfigSystem.SystemName = Element->ReadString(TEXT("systemname"));
		const FString ReadImage = Element->ReadString(TEXT("image"));
		const FString ReadScreenshot = Element->ReadString(TEXT("screenshot"));
		ConfigSystem.Image = (ReadImage.IsEmpty()) ? UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media\\") + ConfigSystem.SystemName + TEXT("\\system.png") : ReadImage;
		ConfigSystem.Screenshot = (ReadImage.IsEmpty()) ? UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media\\") + ConfigSystem.SystemName + TEXT("\\screenshot.png") : ReadScreenshot;
		ConfigSystem.Description = Element->ReadString(TEXT("description"));
		if (FPaths::FileExists(ConfigSystem.RomPath + TEXT("\\gamelist.xml")) && UClassicFunctionLibrary::VerifyDirectory(ConfigSystem.RomPath))
		{
			ConfigSystems.Add(ConfigSystem);
		}
	}
}

void UGameDataFunctionLibrary::SetGameData(TArray<UEasyXMLElement*> Elements, TArray<FGameData>& GameData, UTexture2D* Texture)
{
	FGameData Data;
	int32 Index = 0;

	for (UEasyXMLElement* Element : Elements)
	{
		Data.MapIndex = Index;
		Data.Path = Element->ReadString(TEXT("path"));
		Data.name = Element->ReadString(TEXT("name"));
		Data.desc = Element->ReadString(TEXT("desc"));
		Data.rating = Element->ReadString(TEXT("rating"));
		Data.releasedate = Element->ReadString(TEXT("releasedate"));
		Data.developer = Element->ReadString(TEXT("developer"));
		Data.publisher = Element->ReadString(TEXT("publisher"));
		Data.genre = Element->ReadString(TEXT("genre"));
		Data.players = Element->ReadString(TEXT("players"));
		Data.hash = Element->ReadString(TEXT("hash"));
		Data.image = Element->ReadString(TEXT("image"));
		Data.thumbnail = Element->ReadString(TEXT("thumbnail"));
		Data.video = Element->ReadString(TEXT("video"));
		Data.genreid = Element->ReadString(TEXT("genreid"));
		Data.favorite = Element->ReadBool(TEXT("favorite"));
		Data.playcount = Element->ReadInt(TEXT("playcount"));
		Data.lastplayed = Element->ReadString(TEXT("lastplayed"));
		Data.Executable = Element->ReadString(TEXT("executable"));
		Data.Arguments = Element->ReadString(TEXT("arguments"));
		GameData.Add(Data);

		Index += 1;
	}
}

FGameSystem UGameDataFunctionLibrary::SetSystemToGameData(TArray<FGameSystem> Systems)
{
	TArray<FGameData> GameData;
	for (int32 i = 0; i < Systems.Num(); i++)
	{
		FGameData NewGameData;
		NewGameData.MapIndex = i;
		NewGameData.PathFormated = Systems[i].RomPath;
		NewGameData.nameFormated = Systems[i].SystemLabel;
		NewGameData.Executable = Systems[i].Executable;
		NewGameData.imageFormated = Systems[i].Image;
		NewGameData.thumbnailFormated = Systems[i].Screenshot;
		NewGameData.descFormated = Systems[i].Description;
		GameData.Add(NewGameData);
	}

	FGameSystem NewSystem;
	NewSystem.SystemName = TEXT("${System}");
	NewSystem.SystemLabel = TEXT("Systems");
	NewSystem.GameDatas = GameData;
	return NewSystem;
}

void UGameDataFunctionLibrary::FormatGameData(FGameSystem& GameSystems, FConfig Configuration)
{
	for (FGameData& GameData : GameSystems.GameDatas)
	{
		GameData.PathFormated = TEXT("\"") + ReplacePath(GameData.Path, GameSystems.RomPath) + TEXT("\"");
		if (GameData.PathFormated.Equals(TEXT("\"\"")))
		{
			GameData.PathFormated = TEXT("");
		}
		GameData.imageFormated = ReplacePath(GameData.image, GameSystems.RomPath);
		GameData.thumbnailFormated = ReplacePath(GameData.thumbnail, GameSystems.RomPath);
		GameData.videoFormated = ReplacePath(GameData.video, GameSystems.RomPath);
		GameData.nameFormated = GameData.name.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
		GameData.descFormated = GameData.desc.Replace(TEXT("&amp;"), TEXT("&"), ESearchCase::IgnoreCase);
	}
}

bool UGameDataFunctionLibrary::FindGameData(TArray<FGameData> GameData, FGameData GameDataElement, int32& Index)
{
	Index = GameData.IndexOfByPredicate([GameDataElement](const FGameData& A)
	{
		return A.MapIndex == GameDataElement.MapIndex;
	});

	return Index != INDEX_NONE; //Index none -1;
}

void UGameDataFunctionLibrary::CreateFolders(FString Path, TArray<FGameSystem> GameSystems)
{
	const FString PathMedia = (PathMedia != TEXT("")) ? Path : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("media");

	UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia);

	for (FGameSystem& GameSystemElement : GameSystems)
	{
		if (GameSystemElement.SystemName.Equals(TEXT("${System}"))) continue; //ignore System
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName);
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName + TEXT("\\covers"));
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName + TEXT("\\screenshots"));
		UClassicFunctionLibrary::VerifyOrCreateDirectory(PathMedia + TEXT("\\") + GameSystemElement.SystemName + TEXT("\\videos"));
	}
}

TArray<FGameData> UGameDataFunctionLibrary::FilterGameData(TArray<FGameData> GameData, EGamesFilter& Filter)
{
	if(!IsHasFavorites(GameData))
	{
		Filter = EGamesFilter::DEFAULT;
	}
	if(Filter == EGamesFilter::DEFAULT)
	{
		return  GameData;
	}
	if(Filter == EGamesFilter::FAVORITES_FIRST)
	{
		return  FilterGameDataFavoritesFirst(GameData, false);
	}
	if(Filter == EGamesFilter::FAVORITES_ONLY)
	{
		return  FilterGameDataFavoritesFirst(GameData, true);
	}
	return GameData;
}

bool UGameDataFunctionLibrary::IsHasFavorites(TArray<FGameData>& GameData)
{
	for (const FGameData& Data : GameData)
	{
		if (Data.favorite)
		{
			return true;
		}
	}
	return false;
}

void UGameDataFunctionLibrary::BreakGameDataDateTime(FGameData GameData, FDateTime& DateTimeReleaseDate, FString& StringReleaseDate, FDateTime& DateTimeLastPlayed, FString& StringLastPlayed, FString& ReleaseDateFormatted, FString& StringLastPlayedFormatted)
{
	DateTimeLastPlayed = GameData.DTLastPlayed.ToDateTime();
	StringLastPlayed = GameData.DTLastPlayed.ToString();
	DateTimeReleaseDate = GameData.DTReleaseDate.ToDateTime();
	StringReleaseDate = GameData.DTReleaseDate.ToString();
	StringLastPlayedFormatted = GameData.DTLastPlayed.FormatDateTime();
	ReleaseDateFormatted = GameData.DTReleaseDate.FormatDateTime();
}

FGameData UGameDataFunctionLibrary::MakeGameDataDateTime_DateTime(FGameData GameData, FDateTime DateTimeReleaseDate, FDateTime DateTimeLastPlayed)
{
	GameData.DTReleaseDate = DateTimeReleaseDate;
	GameData.DTLastPlayed = DateTimeLastPlayed;
	return GameData;
}

FGameData UGameDataFunctionLibrary::MakeGameDataDateTime_String(FGameData GameData, FString StringReleaseDate, FString StringLastPlayed)
{
	GameData.DTReleaseDate = StringReleaseDate;
	GameData.DTLastPlayed = StringLastPlayed;
	return GameData;
}











