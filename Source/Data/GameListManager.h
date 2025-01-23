#ifndef GAME_LIST_MANAGER_H
#define GAME_LIST_MANAGER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Data/DateTime.h"
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
            : mapIndex(-1), bFavorite(false), playCount(0) {};
        ~GameList() = default;

        bool operator==(const GameList& a) const { return (mapIndex == a.mapIndex); }
        bool operator>(const GameList& a) const { return (mapIndex > a.mapIndex); }
        bool operator<(const GameList& a) const { return (mapIndex < a.mapIndex); }
    };

    struct HistoryPosition
    {
        int id = 0;
        int indexCardFocus = 3;
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
        HistoryPosition history;
        std::string pathTheme;
        float scale;

        SystemList()
            : mapIndex(-1), scale(1.0f)
        {
        }

        bool operator==(const SystemList& a) const { return (mapIndex == a.mapIndex); }
        bool operator>(const SystemList& a) const { return (mapIndex > a.mapIndex); }
        bool operator<(const SystemList& a) const { return (mapIndex < a.mapIndex); }
    };

    class GameListManager
    {
    public:

        GameListManager()
            : mCurrentList(SystemListSelect), mIdSystemList(0), mIdGameList(0) {};

    private:

        CurrentList mCurrentList;
        int mIdSystemList;
        int mIdGameList;
        tinyxml2::XMLDocument mDocumentGameListXml;
        tinyxml2::XMLDocument mDocumentSystemListXml;
        std::vector<GameList> mGameList;
        std::vector<SystemList> mSystemList;
        void LoadGameList();
        void LoadSystemToGameList();

    public:

        void Initialize();
        void ChangeSystemToGameList();
        void ChangeGameToSystemList();
        void LoadList();
        void LoadSystemList();
        void AddId(const int newId);
        void ChangeId(const int newId);
        int GetGameId() const;
        int GetSystemId() const;
        int GetGameListSize();
        std::vector<GameList*> GetAllGameList();
        std::vector<SystemList*> GetAllSystemList();
        GameList* GetCurrentGameList(int index);
        GameList* GetCurrentGameList();
        SystemList* GetCurrentSystemList();
        void ClearSystemList();
        void ClearGameList();
        CurrentList GetCurrentList() const;
        void GameListSortByName();
        void SystemListSortByName();

    private:

        void ReplaceCurrentPath(GameList* pGame) const;
        static bool IsValidElement(const tinyxml2::XMLElement* pElement, const char* name = "");
    };

}  // namespace ClassicLauncher

#endif  // GAMELISTMANAGER_H
