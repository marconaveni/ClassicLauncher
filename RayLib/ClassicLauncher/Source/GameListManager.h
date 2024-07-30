#pragma once


#include <string>
#include <vector>

#include "Date.h"
#include "External/tinyxml2.h"


enum LoadXmlError {
	Success = 0,
	FileNotFound,

};

enum CurrentList {
	SystemListSelect,
	GameListSelect,
};

struct GameList
{

	int mapIndex;
	std::string path;
	std::string name;
	std::string desc;
	std::string rating;
	std::string developer;
	std::string publisher;
	std::string genre;
	std::string players;
	std::string hash;
	std::string image;
	std::string thumbnail;
	std::string video;
	std::string genreId;
	bool bFavorite;
	int playCount;
	std::string executable;
	std::string arguments;
	DateTime releaseDate;
	DateTime lastPlayed;

	GameList()
		: mapIndex(-1)
		, players("1")
		, bFavorite(false)
		, playCount(0)
	{}

	bool operator==(const GameList& a) const
	{
		return (mapIndex == a.mapIndex);
	}
	bool operator>(const GameList& a) const
	{
		return (mapIndex > a.mapIndex);
	}
	bool operator<(const GameList& a) const
	{
		return (mapIndex < a.mapIndex);
	}
};


struct SystemList
{
	int mapIndex;
	std::string executable;
	std::string arguments;
	std::string romPath;
	std::string systemName;
	std::string systemLabel;
	std::string image;
	std::string screenshot;
	std::string video;
	std::string desc;

	SystemList()
		: mapIndex(-1)
	{
	}


};



class GameListManager
{

private:

	GameListManager()
		: currentList(SystemListSelect)
		, idSystemList(0)
		, idGameList(0)
	{
	}

public:

	static GameListManager* GetInstance();

private:

	CurrentList currentList;

	int idSystemList;
	int idGameList;

	tinyxml2::XMLDocument documentGameListXml;
	tinyxml2::XMLDocument documentSystemListXml;
	std::vector<GameList> gameList;
	std::vector<SystemList> systemList;

public:

	void LoadGameList();
	void LoadSystemList();

	void AddId(const int newId);
	void ChangeId(const int newId);
	int GetId() const;

private:

	void ReplaceCurrentPath(GameList& game) const;
	static bool IsValidElement(const tinyxml2::XMLElement* pElement, const char* name = "");


};
