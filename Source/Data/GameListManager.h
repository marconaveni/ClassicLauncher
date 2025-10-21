#ifndef GAME_LIST_MANAGER_H
#define GAME_LIST_MANAGER_H


#include <string>
#include <filesystem>
#include <vector>

#include "Data/DateTime.h"
#include "Themes/ConfigurationThemes.h"
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
        int mapIndex{-1};
        std::string path{};
        std::string name{};
        std::string description{};
        std::string rating{};
        std::string developer{};
        std::string publisher{};
        std::string genre{};
        std::string players{};
        std::string hash{};
        std::string image{};
        std::string thumbnail{};
        std::string video{};
        std::string genreId{};
        bool isFavorite{false};
        int playCount{0};
        std::string executable{};
        std::string arguments{};
        DateTime releaseDate{};
        DateTime lastPlayed{};

        GameList() = default;

        bool operator==(const GameList& a) const { return (mapIndex == a.mapIndex); }
        bool operator>(const GameList& a) const { return (mapIndex > a.mapIndex); }
        bool operator<(const GameList& a) const { return (mapIndex < a.mapIndex); }
    };

    struct GameSystemList
    {
        struct HistoryPosition
        {
            int id{0};
            int indexCardFocus{3};

            HistoryPosition() = default;
        };

        struct Theme
        {
            std::filesystem::path path{};
            std::string sprite{"sprite"};
            bool isDirectoryExist{false};
            bool isLoaded{false};
        };
        

        int mapIndex{-1};
        std::string executable{};
        std::string arguments{};
        std::string romPath{};
        std::string systemName{};
        std::string systemLabel{};
        std::string image{};
        std::string screenshot{};
        std::string video{};
        std::string desc{};
        HistoryPosition history{};
        Theme theme{};
        ConfigurationThemes configThemes{};

        ~GameSystemList() = default;

        bool operator==(const GameSystemList& a) const { return (mapIndex == a.mapIndex); }
        bool operator>(const GameSystemList& a) const { return (mapIndex > a.mapIndex); }
        bool operator<(const GameSystemList& a) const { return (mapIndex < a.mapIndex); }
    };

    class GameListManager
    {

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
        std::vector<GameSystemList*> GetAllSystemList();
        GameList* GetCurrentGameList(int index);
        GameList* GetCurrentGameList();
        GameSystemList* GetCurrentSystemList();
        void ClearSystemList();
        void ClearGameList();
        CurrentList GetCurrentList() const;
        void GameListSortByName();
        void SystemListSortByName();

    private:

        void LoadGameList();
        void LoadSystemToGameList();
        void ReplaceCurrentPath(GameList* gameList, const std::string& romPath) const;
        static bool IsValidElement(const tinyxml2::XMLElement* element, const char* name = "");

        CurrentList m_currentList{CurrentList::SystemListSelect};
        int m_idSystemList{-1};
        int m_idGameList{0};
        tinyxml2::XMLDocument m_documentGameListXml{};
        tinyxml2::XMLDocument m_documentSystemListXml{};
        std::vector<GameList> m_gameList{};
        std::vector<GameSystemList> m_gameSystemList{};
    };

} // namespace ClassicLauncher

#endif // GAMELISTMANAGER_H
