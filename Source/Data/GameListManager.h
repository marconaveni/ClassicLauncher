#ifndef GAME_LIST_MANAGER_H
#define GAME_LIST_MANAGER_H


#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "Data/Date.h"
#include "Utils/Print.h"
#include "tinyxml2/tinyxml2.h"


namespace ClassicLauncher
{


	enum LoadXmlError 
	{
		Success = 0,
		FileNotFound,

	};

	enum CurrentList 
	{
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
			, bFavorite(false)
			, playCount(0)
		{
		}

		~GameList() = default;

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

		bool operator==(const SystemList& a) const
		{
			return (mapIndex == a.mapIndex);
		}
		bool operator>(const SystemList& a) const
		{
			return (mapIndex > a.mapIndex);
		}
		bool operator<(const SystemList& a) const
		{
			return (mapIndex < a.mapIndex);
		}


	};



	class GameListManager
	{

	public:

		GameListManager()
			: currentList(SystemListSelect)
			, idSystemList(0)
			, idGameList(0)
		{
		}
	
	private:

		CurrentList currentList;
		int idSystemList;
		int idGameList;
		tinyxml2::XMLDocument documentGameListXml;
		tinyxml2::XMLDocument documentSystemListXml;
		std::vector<GameList> gameList;
		std::vector<SystemList> systemList;
		void LoadGameList();
		void LoadSystemToGameList();

	public:

		void Initialize();
		void Update();
		void ChangeSystemToGameList();
		void ChangeGameToSystemList();
		void LoadList();
		void LoadSystemList();
		void AddId(const int newId);
		void ChangeId(const int newId);
		int GetGameId() const;
		int GetSystemId() const;
		int GetGameListSize() const;
		std::vector<GameList>& GetAllGameList();
		GameList* GetCurrentGameList(int index);
		GameList* GetCurrentGameList();
		SystemList* GetCurrentSystemList();
		void ClearSystemList();
		void ClearGameList();
		CurrentList GetCurrentList() const;
		void GameListSortByName();
		void SystemListSortByName();

	private:

		void ReplaceCurrentPath(GameList* game) const;
		static bool IsValidElement(const tinyxml2::XMLElement* pElement, const char* name = "");


	};

}

#endif // GAMELISTMANAGER_H
