#include "GameListManager.h"

#include <algorithm>
#include "Utils/Math.h"
#include "Utils/Resources.h"
#include "Utils/StringFunctionLibrary.h"
#include "Utils/UtilsFunctionLibrary.h"

namespace ClassicLauncher
{
    void GameListManager::LoadGameList()
    {
        using namespace StringFunctionLibrary;

        documentGameListXml.Clear();
        const std::string pathXml = NormalizePath(systemList[idSystemList].romPath + "\\gamelist.xml");
        if (documentGameListXml.LoadFile(pathXml.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        {
            return;
        }

        tinyxml2::XMLElement* pRootElement = documentGameListXml.RootElement();
        tinyxml2::XMLElement* pGame = pRootElement->FirstChildElement("game");
        int index = 0;

        while (pGame)
        {
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
            ReplaceCurrentPath(&game);
            gameList.push_back(game);

            pGame = pGame->NextSiblingElement("game");
            index++;
        }
        gameList.shrink_to_fit();
        GameListSortByName();
    }

    void GameListManager::LoadSystemToGameList()
    {
        for (const auto& system : systemList)
        {
            auto game = GameList();
            game.mapIndex = system.mapIndex;
            game.name = system.systemLabel;
            game.desc = system.desc;
            game.image = system.image;
            game.executable = system.executable;
            game.arguments = system.arguments;
            ReplaceCurrentPath(&game);
            gameList.push_back(game);
        }
        gameList.shrink_to_fit();
    }

    void GameListManager::Initialize()
    {
        LoadSystemList();
        LoadList();
    }

    void GameListManager::Update()
    {
    }

    void GameListManager::ChangeSystemToGameList()
    {
        idSystemList = idGameList;
        idGameList = 0;
        currentList = GameListSelect;
        ClearGameList();
        LoadList();
    }

    void GameListManager::ChangeGameToSystemList()
    {
        idGameList = idSystemList;
        currentList = SystemListSelect;
        ClearGameList();
        LoadList();
    }

    void GameListManager::LoadList()
    {
        if (systemList.empty())
        {
            return;
        }

        switch (currentList)
        {
            case SystemListSelect:
                LoadSystemToGameList();
                break;
            case GameListSelect:
                LoadGameList();
                break;
            default:
                break;
        }
    }

    void GameListManager::LoadSystemList()
    {
        using namespace StringFunctionLibrary;

        documentSystemListXml.Clear();
        const std::string systemListPath = NormalizePath(UtilsFunctionLibrary::GetHomeDir() + Resources::systemList);
        if (documentSystemListXml.LoadFile(systemListPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        {
            return;
        }

        tinyxml2::XMLElement* pRootElement = documentSystemListXml.RootElement();
        tinyxml2::XMLElement* pSystem = pRootElement->FirstChildElement("system");
        int index = 0;

        while (pSystem)
        {
            auto systems = SystemList();
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
            systemList.push_back(systems);

            pSystem = pSystem->NextSiblingElement("system");
            index++;
        }
        systemList.shrink_to_fit();
        SystemListSortByName();
    }

    void GameListManager::AddId(const int newId)
    {
        idGameList = UtilsFunctionLibrary::SetIndexArray(idGameList += newId, static_cast<int>(gameList.size()));
    }

    void GameListManager::ChangeId(const int newId)
    {
        idGameList = Math::Clamp(newId, 0, static_cast<int>(gameList.size()) - 1);
    }

    int GameListManager::GetGameId() const
    {
        return idGameList;
    }

    int GameListManager::GetSystemId() const
    {
        return idSystemList;
    }

    int GameListManager::GetGameListSize()
    {
        return static_cast<int>(gameList.size());
    }

    std::vector<GameList>& GameListManager::GetAllGameList()
    {
        return gameList;
    }

    GameList* GameListManager::GetCurrentGameList(const int index)
    {
        return &gameList[index];
    }

    GameList* GameListManager::GetCurrentGameList()
    {
        return (!gameList.empty()) ? &gameList[idGameList] : nullptr;
    }

    SystemList* GameListManager::GetCurrentSystemList()
    {
        return &systemList[idSystemList];
    }

    void GameListManager::ClearSystemList()
    {
        systemList.clear();
        systemList.shrink_to_fit();
    }

    void GameListManager::ClearGameList()
    {
        gameList.clear();
        gameList.shrink_to_fit();
    }

    CurrentList GameListManager::GetCurrentList() const
    {
        return currentList;
    }

    void GameListManager::GameListSortByName()
    {
        std::sort(gameList.begin(), gameList.end(), [](const GameList& a, const GameList& b) { return a.name < b.name; });
    }

    void GameListManager::SystemListSortByName()
    {
        std::sort(systemList.begin(), systemList.end(), [](const SystemList& a, const SystemList& b) { return a.systemLabel < b.systemLabel; });
    }

    void GameListManager::ReplaceCurrentPath(GameList* game) const
    {
        std::string dotSlash = "./";
        std::string slash = "/";
#ifdef _WIN32
        dotSlash = ".\\";
        slash = "\\";
#endif
        StringFunctionLibrary::ReplaceString(game->path, dotSlash, systemList[idSystemList].romPath + slash);
        StringFunctionLibrary::ReplaceString(game->image, dotSlash, systemList[idSystemList].romPath + slash);
        StringFunctionLibrary::ReplaceString(game->thumbnail, dotSlash, systemList[idSystemList].romPath + slash);
        StringFunctionLibrary::ReplaceString(game->video, dotSlash, systemList[idSystemList].romPath + slash);
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

}  // namespace ClassicLauncher
