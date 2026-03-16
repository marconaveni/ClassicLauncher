#include "GameListManager.h"

#include <algorithm>
#include <filesystem>

#include "ClassicAssert.h"
#include "Helper.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "Utils/Utils.h"


namespace ClassicLauncher
{
    void GameListManager::LoadGameList()
    {
        using namespace String;

        m_documentGameListXml.Clear();
        const std::string pathXml = NormalizePath(m_gameSystemList[m_idSystemList].romPath + "/gamelist.xml");
        if (m_documentGameListXml.LoadFile(pathXml.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        {
            return;
        }

        tinyxml2::XMLElement* rootElement = m_documentGameListXml.RootElement();
        tinyxml2::XMLElement* gameElement = rootElement->FirstChildElement("game");
        int index = 0;

        while (gameElement)
        {
            // clang-format off
            auto game = GameList();
            game.mapIndex = index;
            game.path = IsValidElement(gameElement, "path") ? NormalizePath(gameElement->FirstChildElement("path")->GetText()) : "";
            game.name = IsValidElement(gameElement, "name") ? gameElement->FirstChildElement("name")->GetText() : "";
            game.description = IsValidElement(gameElement, "desc") ? gameElement->FirstChildElement("desc")->GetText() : "";
            game.rating = IsValidElement(gameElement, "rating") ? gameElement->FirstChildElement("rating")->GetText() : "";
            game.developer = IsValidElement(gameElement, "developer") ? gameElement->FirstChildElement("developer")->GetText() : "";
            game.publisher = IsValidElement(gameElement, "publisher") ? gameElement->FirstChildElement("publisher")->GetText() : "";
            game.genre = IsValidElement(gameElement, "genre") ? gameElement->FirstChildElement("genre")->GetText() : "";
            game.players = IsValidElement(gameElement, "players") ? gameElement->FirstChildElement("players")->GetText() : "";
            game.hash = IsValidElement(gameElement, "hash") ? gameElement->FirstChildElement("hash")->GetText() : "";
            game.image = IsValidElement(gameElement, "image") ? NormalizePath(gameElement->FirstChildElement("image")->GetText()) : "";
            game.thumbnail = IsValidElement(gameElement, "thumbnail") ? NormalizePath(gameElement->FirstChildElement("thumbnail")->GetText()) : "";
            game.video = IsValidElement(gameElement, "video") ? NormalizePath(gameElement->FirstChildElement("video")->GetText()) : "";
            game.genreId = IsValidElement(gameElement, "genreid") ? gameElement->FirstChildElement("genreid")->GetText() : "";
            game.isFavorite = IsValidElement(gameElement, "favorite") ? gameElement->FirstChildElement("favorite")->BoolText() : false;
            game.playCount = IsValidElement(gameElement, "playcount") ? gameElement->FirstChildElement("playcount")->IntText() : 0;
            game.executable = IsValidElement(gameElement, "executable") ? NormalizePath(gameElement->FirstChildElement("executable")->GetText()) : "";
            game.arguments = IsValidElement(gameElement, "arguments") ? NormalizePath(gameElement->FirstChildElement("arguments")->GetText()) : "";
            game.releaseDate = IsValidElement(gameElement, "releasedate") ? NormalizePath(gameElement->FirstChildElement("releasedate")->GetText()) : "";
            game.lastPlayed = IsValidElement(gameElement, "lastplayed") ? NormalizePath(gameElement->FirstChildElement("lastplayed")->GetText()) : "";
            ReplaceCurrentPath(&game, m_gameSystemList[m_idSystemList].romPath);
            m_gameList.push_back(game);
            // clang-format on

            gameElement = gameElement->NextSiblingElement("game");
            index++;
        }
        m_gameList.shrink_to_fit();
        GameListSortByName();
    }

    void GameListManager::LoadSystemToGameList()
    {
        for (const auto& system : m_gameSystemList)
        {
            auto game = GameList();
            game.mapIndex = system.mapIndex;
            game.name = system.systemLabel;
            game.description = system.desc;
            game.image = system.image;
            game.executable = system.executable;
            game.arguments = system.arguments;
            ReplaceCurrentPath(&game, system.romPath);
            m_gameList.push_back(game);
        }
        m_gameList.shrink_to_fit();
    }

    void GameListManager::Initialize()
    {
        LoadSystemList();
        LoadList();
    }

    void GameListManager::ChangeSystemToGameList()
    {
        m_idSystemList = m_idGameList;
        m_currentList = CurrentList::GameListSelect;
        ClearGameList();
        LoadList();
        m_idGameList = GetCurrentSystemList()->history.id;
    }

    void GameListManager::ChangeGameToSystemList()
    {
        m_idGameList = m_idSystemList;
        m_idSystemList = -1;
        m_currentList = CurrentList::SystemListSelect;
        ClearGameList();
        LoadList();
    }

    void GameListManager::LoadList()
    {
        if (m_gameSystemList.empty())
        {
            return;
        }

        switch (m_currentList)
        {
            case CurrentList::SystemListSelect: LoadSystemToGameList(); break;
            case CurrentList::GameListSelect: LoadGameList(); break;
            default: break;
        }
    }

    void GameListManager::LoadSystemList()
    {
        using namespace String;

        m_documentSystemListXml.Clear();
        const std::string systemListPath = NormalizePath(Resources::GetClassicLauncherDirectory("systemlist.xml"));
        if (m_documentSystemListXml.LoadFile(systemListPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        {
            return;
        }

        tinyxml2::XMLElement* rootElement = m_documentSystemListXml.RootElement();
        tinyxml2::XMLElement* systemElement = rootElement->FirstChildElement("system");
        int index = 0;

        while (systemElement)
        {
            // clang-format off
            GameSystemList systems;
            systems.mapIndex = index;
            systems.executable = IsValidElement(systemElement, "executable") ? NormalizePath(systemElement->FirstChildElement("executable")->GetText()) : "";
            systems.arguments = IsValidElement(systemElement, "arguments") ? NormalizePath(systemElement->FirstChildElement("arguments")->GetText()) : "";
            systems.romPath = IsValidElement(systemElement, "rompath") ? NormalizePath(systemElement->FirstChildElement("rompath")->GetText()) : "";
            systems.systemName = IsValidElement(systemElement, "systemname") ? systemElement->FirstChildElement("systemname")->GetText() : "";
            systems.systemLabel = IsValidElement(systemElement, "systemlabel") ? systemElement->FirstChildElement("systemlabel")->GetText() : "";
            systems.image = IsValidElement(systemElement, "image") ? systemElement->FirstChildElement("image")->GetText() : "";
            systems.screenshot = IsValidElement(systemElement, "thumbnail") ? systemElement->FirstChildElement("thumbnail")->GetText() : "";
            systems.video = IsValidElement(systemElement, "video") ? systemElement->FirstChildElement("video")->GetText() : "";
            systems.desc = IsValidElement(systemElement, "desc") ? systemElement->FirstChildElement("desc")->GetText() : "";
            std::filesystem::path path = NormalizePath(Resources::GetThemeDirectory() + "/" + systems.systemName);
            std::filesystem::path file = NormalizePath(TEXT("/sprite%.0fx.png", ThemesManager::GetScaleRenderer()));
            if (std::filesystem::is_directory(path))
            {
                systems.theme.path = path;
                systems.theme.isDirectoryExist = true;
            }
            
            m_gameSystemList.push_back(systems);

            systemElement = systemElement->NextSiblingElement("system");
            index++;
            // clang-format on
        }
        m_gameSystemList.shrink_to_fit();
        SystemListSortByName();
    }

    void GameListManager::AddId(const int newId)
    {
        m_idGameList = Utils::SetIndexArray(m_idGameList += newId, static_cast<int>(m_gameList.size()));
        if (m_currentList == CurrentList::GameListSelect)
        {
            m_gameSystemList[m_idSystemList].history.id = m_idGameList;
        }
    }

    void GameListManager::ChangeId(const int newId)
    {
        m_idGameList = Math::Clamp(newId, 0, static_cast<int>(m_gameList.size()) - 1);
        if (m_currentList == CurrentList::GameListSelect)
        {
            m_gameSystemList[m_idSystemList].history.id = m_idGameList;
        }
    }

    int GameListManager::GetGameId() const
    {
        return m_idGameList;
    }

    int GameListManager::GetSystemId() const
    {
        return m_idSystemList;
    }

    int GameListManager::GetGameListSize()
    {
        return static_cast<int>(m_gameList.size());
    }

    std::vector<GameList*> GameListManager::GetAllGameList()
    {
        std::vector<GameList*> gameList;
        for (auto& game : m_gameList)
        {
            gameList.push_back(&game);
        }
        return gameList;
    }

    std::vector<GameSystemList*> GameListManager::GetAllSystemList()
    {
        std::vector<GameSystemList*> gameSystemList;
        for (auto& game : m_gameSystemList)
        {
            gameSystemList.push_back(&game);
        }
        return gameSystemList;
    }

    GameList* GameListManager::GetCurrentGameList(const int index)
    {
        return &m_gameList[index];
    }

    GameList* GameListManager::GetCurrentGameList()
    {
        return (!m_gameList.empty()) ? &m_gameList[m_idGameList] : nullptr;
    }

    GameSystemList* GameListManager::GetCurrentSystemList()
    {
        if (m_idSystemList < 0)
        {
            return nullptr;
        }

        CLASSIC_ASSERT(m_idSystemList >= 0, "must be greater than zero");
        return &m_gameSystemList[m_idSystemList];
    }

    void GameListManager::ClearSystemList()
    {
        m_gameSystemList.clear();
        m_gameSystemList.shrink_to_fit();
    }

    void GameListManager::ClearGameList()
    {
        m_gameList.clear();
        m_gameList.shrink_to_fit();
    }

    CurrentList GameListManager::GetCurrentList() const
    {
        return m_currentList;
    }

    void GameListManager::GameListSortByName()
    {
        std::sort(m_gameList.begin(), m_gameList.end(), [](const GameList& a, const GameList& b) { return a.name < b.name; });
    }

    void GameListManager::SystemListSortByName()
    {
        std::sort(m_gameSystemList.begin(),
                  m_gameSystemList.end(),
                  [](const GameSystemList& a, const GameSystemList& b) { return a.systemLabel < b.systemLabel; });
    }

    void GameListManager::ResetTheme()
    {
        for (auto& system : m_gameSystemList)
        {
            system.theme.isLoaded = false;
        }
    }

    void GameListManager::ReplaceCurrentPath(GameList* gameList, const std::string& romPath) const
    {
        std::string dotSlash = "./";
        std::string slash = "/";
#ifdef _WIN32
        dotSlash = ".\\";
        slash = "\\";
#endif
        String::ReplaceString(gameList->path, dotSlash, romPath + slash);
        String::ReplaceString(gameList->image, dotSlash, romPath + slash);
        String::ReplaceString(gameList->thumbnail, dotSlash, romPath + slash);
        String::ReplaceString(gameList->video, dotSlash, romPath + slash);
    }

    bool GameListManager::IsValidElement(const tinyxml2::XMLElement* element, const char* name)
    {
        bool isValid = element->FirstChildElement(name) != nullptr;
        if (isValid)
        {
            isValid = element->FirstChildElement(name)->GetText() != nullptr;
        }
        return isValid;
    }

} // namespace ClassicLauncher
