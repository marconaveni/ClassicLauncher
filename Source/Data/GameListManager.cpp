#include "GameListManager.h"

#include <algorithm>
#include <filesystem>

#include "Utils/Math.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "Utils/Utils.h"
#include "Themes/ThemesManager.h"
#include "Helper.h"
#include "ClassicAssert.h"

namespace ClassicLauncher
{
    void GameListManager::LoadGameList()
    {
        using namespace String;

        m_documentGameListXml.Clear();
        const std::string pathXml = NormalizePath(m_systemList[m_idSystemList].romPath + "/gamelist.xml");
        if (m_documentGameListXml.LoadFile(pathXml.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        {
            return;
        }

        tinyxml2::XMLElement* pRootElement = m_documentGameListXml.RootElement();
        tinyxml2::XMLElement* pGame = pRootElement->FirstChildElement("game");
        int index = 0;

        while (pGame)
        {
            // clang-format off
            auto game = GameList();
            game.mapIndex = index;
            game.path = IsValidElement(pGame, "path") ? NormalizePath(pGame->FirstChildElement("path")->GetText()) : "";
            game.name = IsValidElement(pGame, "name") ? pGame->FirstChildElement("name")->GetText() : "";
            game.desc = IsValidElement(pGame, "desc") ? pGame->FirstChildElement("desc")->GetText() : "";
            game.rating = IsValidElement(pGame, "rating") ? pGame->FirstChildElement("rating")->GetText() : "";
            game.developer = IsValidElement(pGame, "developer") ? pGame->FirstChildElement("developer")->GetText() : "";
            game.publisher = IsValidElement(pGame, "publisher") ? pGame->FirstChildElement("publisher")->GetText() : "";
            game.genre = IsValidElement(pGame, "genre") ? pGame->FirstChildElement("genre")->GetText() : "";
            game.players = IsValidElement(pGame, "players") ? pGame->FirstChildElement("players")->GetText() : "";
            game.hash = IsValidElement(pGame, "hash") ? pGame->FirstChildElement("hash")->GetText() : "";
            game.image = IsValidElement(pGame, "image") ? NormalizePath(pGame->FirstChildElement("image")->GetText()) : "";
            game.thumbnail = IsValidElement(pGame, "thumbnail") ? NormalizePath(pGame->FirstChildElement("thumbnail")->GetText()) : "";
            game.video = IsValidElement(pGame, "video") ? NormalizePath(pGame->FirstChildElement("video")->GetText()) : "";
            game.genreId = IsValidElement(pGame, "genreid") ? pGame->FirstChildElement("genreid")->GetText() : "";
            game.bFavorite = IsValidElement(pGame, "favorite") ? pGame->FirstChildElement("favorite")->BoolText() : false;
            game.playCount = IsValidElement(pGame, "playcount") ? pGame->FirstChildElement("playcount")->IntText() : 0;
            game.executable = IsValidElement(pGame, "executable") ? NormalizePath(pGame->FirstChildElement("executable")->GetText()) : "";
            game.arguments = IsValidElement(pGame, "arguments") ? NormalizePath(pGame->FirstChildElement("arguments")->GetText()) : "";
            game.releaseDate = IsValidElement(pGame, "releasedate") ? NormalizePath(pGame->FirstChildElement("releasedate")->GetText()) : "";
            game.lastPlayed = IsValidElement(pGame, "lastplayed") ? NormalizePath(pGame->FirstChildElement("lastplayed")->GetText()) : "";
            ReplaceCurrentPath(&game, m_systemList[m_idSystemList].romPath);
            m_gameList.push_back(game);
            // clang-format on

            pGame = pGame->NextSiblingElement("game");
            index++;
        }
        m_gameList.shrink_to_fit();
        GameListSortByName();
    }

    void GameListManager::LoadSystemToGameList()
    {
        for (const auto& system : m_systemList)
        {
            auto game = GameList();
            game.mapIndex = system.mapIndex;
            game.name = system.systemLabel;
            game.desc = system.desc;
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
        m_currentList = GameListSelect;
        ClearGameList();
        LoadList();
        m_idGameList = GetCurrentSystemList()->history.id;
    }

    void GameListManager::ChangeGameToSystemList()
    {
        m_idGameList = m_idSystemList;
        m_idSystemList = -1;
        m_currentList = SystemListSelect;
        ClearGameList();
        LoadList();
    }

    void GameListManager::LoadList()
    {
        if (m_systemList.empty())
        {
            return;
        }

        switch (m_currentList)
        {
            case SystemListSelect: LoadSystemToGameList(); break;
            case GameListSelect: LoadGameList(); break;
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

        tinyxml2::XMLElement* pRootElement = m_documentSystemListXml.RootElement();
        tinyxml2::XMLElement* pSystem = pRootElement->FirstChildElement("system");
        int index = 0;

        while (pSystem)
        {
            // clang-format off
            GameSystemList systems;
            systems.mapIndex = index;
            systems.executable = IsValidElement(pSystem, "executable") ? NormalizePath(pSystem->FirstChildElement("executable")->GetText()) : "";
            systems.arguments = IsValidElement(pSystem, "arguments") ? NormalizePath(pSystem->FirstChildElement("arguments")->GetText()) : "";
            systems.romPath = IsValidElement(pSystem, "rompath") ? NormalizePath(pSystem->FirstChildElement("rompath")->GetText()) : "";
            systems.systemName = IsValidElement(pSystem, "systemname") ? pSystem->FirstChildElement("systemname")->GetText() : "";
            systems.systemLabel = IsValidElement(pSystem, "systemlabel") ? pSystem->FirstChildElement("systemlabel")->GetText() : "";
            systems.image = IsValidElement(pSystem, "image") ? pSystem->FirstChildElement("image")->GetText() : "";
            systems.screenshot = IsValidElement(pSystem, "thumbnail") ? pSystem->FirstChildElement("thumbnail")->GetText() : "";
            systems.video = IsValidElement(pSystem, "video") ? pSystem->FirstChildElement("video")->GetText() : "";
            systems.desc = IsValidElement(pSystem, "desc") ? pSystem->FirstChildElement("desc")->GetText() : "";
            const std::string theme = NormalizePath(Resources::GetThemeDirectory() + "/" + systems.systemName + TEXT("/sprite%.0fx.png", ThemesManager::GetScaleRenderer()));
            systems.theme = (std::filesystem::exists(theme)) ? theme : "sprite";          
            m_systemList.push_back(systems);

            pSystem = pSystem->NextSiblingElement("system");
            index++;
            // clang-format on
        }
        m_systemList.shrink_to_fit();
        SystemListSortByName();
    }

    void GameListManager::AddId(const int newId)
    {
        m_idGameList = Utils::SetIndexArray(m_idGameList += newId, static_cast<int>(m_gameList.size()));
        if (m_currentList == CurrentList::GameListSelect)
        {
            m_systemList[m_idSystemList].history.id = m_idGameList;
        }
    }

    void GameListManager::ChangeId(const int newId)
    {
        m_idGameList = Math::Clamp(newId, 0, static_cast<int>(m_gameList.size()) - 1);
        if (m_currentList == CurrentList::GameListSelect)
        {
            m_systemList[m_idSystemList].history.id = m_idGameList;
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
        std::vector<GameList*> pGameList;
        for (auto& game : m_gameList)
        {
            pGameList.push_back(&game);
        }
        return pGameList;
    }

    std::vector<GameSystemList*> GameListManager::GetAllSystemList()
    {
        std::vector<GameSystemList*> pSystemList;
        for (auto& game : m_systemList)
        {
            pSystemList.push_back(&game);
        }
        return pSystemList;
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
        CLASSIC_ASSERT(m_idSystemList >= 0, "must be greater than zero");
        return &m_systemList[m_idSystemList];
    }

    void GameListManager::ClearSystemList()
    {
        m_systemList.clear();
        m_systemList.shrink_to_fit();
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
        std::sort(m_gameList.begin(),
                  m_gameList.end(),
                  [](const GameList& a, const GameList& b) { return a.name < b.name; });
    }

    void GameListManager::SystemListSortByName()
    {
        std::sort(m_systemList.begin(),
                  m_systemList.end(),
                  [](const GameSystemList& a, const GameSystemList& b) { return a.systemLabel < b.systemLabel; });
    }

    void GameListManager::ReplaceCurrentPath(GameList* pGame, const std::string& romPath) const
    {
        std::string dotSlash = "./";
        std::string slash = "/";
#ifdef _WIN32
        dotSlash = ".\\";
        slash = "\\";
#endif
        String::ReplaceString(pGame->path, dotSlash, romPath + slash);
        String::ReplaceString(pGame->image, dotSlash, romPath + slash);
        String::ReplaceString(pGame->thumbnail, dotSlash, romPath + slash);
        String::ReplaceString(pGame->video, dotSlash, romPath + slash);
    }

    bool GameListManager::IsValidElement(const tinyxml2::XMLElement* pElement, const char* name)
    {
        bool bIsValid = pElement->FirstChildElement(name) != nullptr;
        if (bIsValid)
        {
            bIsValid = pElement->FirstChildElement(name)->GetText() != nullptr;
        }
        return bIsValid;
    }

} // namespace ClassicLauncher
