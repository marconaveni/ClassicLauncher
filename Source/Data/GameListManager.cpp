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

        mDocumentGameListXml.Clear();
        const std::string pathXml = NormalizePath(mSystemList[mIdSystemList].romPath + "\\gamelist.xml");
        if (mDocumentGameListXml.LoadFile(pathXml.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        {
            return;
        }

        tinyxml2::XMLElement* pRootElement = mDocumentGameListXml.RootElement();
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
            mGameList.push_back(game);

            pGame = pGame->NextSiblingElement("game");
            index++;
        }
        mGameList.shrink_to_fit();
        GameListSortByName();
    }

    void GameListManager::LoadSystemToGameList()
    {
        for (const auto& system : mSystemList)
        {
            auto game = GameList();
            game.mapIndex = system.mapIndex;
            game.name = system.systemLabel;
            game.desc = system.desc;
            game.image = system.image;
            game.executable = system.executable;
            game.arguments = system.arguments;
            ReplaceCurrentPath(&game);
            mGameList.push_back(game);
        }
        mGameList.shrink_to_fit();
    }

    void GameListManager::Initialize()
    {
        LoadSystemList();
        LoadList();
    }


    void GameListManager::ChangeSystemToGameList()
    {
        mIdSystemList = mIdGameList;
        mIdGameList = 0;
        mCurrentList = GameListSelect;
        ClearGameList();
        LoadList();
    }

    void GameListManager::ChangeGameToSystemList()
    {
        mIdGameList = mIdSystemList;
        mCurrentList = SystemListSelect;
        ClearGameList();
        LoadList();
    }

    void GameListManager::LoadList()
    {
        if (mSystemList.empty())
        {
            return;
        }

        switch (mCurrentList)
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

        mDocumentSystemListXml.Clear();
        const std::string systemListPath = NormalizePath(Resources::GetClassicLauncherDir() + Resources::systemList);
        if (mDocumentSystemListXml.LoadFile(systemListPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
        {
            return;
        }

        tinyxml2::XMLElement* pRootElement = mDocumentSystemListXml.RootElement();
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
            mSystemList.push_back(systems);

            pSystem = pSystem->NextSiblingElement("system");
            index++;
        }
        mSystemList.shrink_to_fit();
        SystemListSortByName();
    }

    void GameListManager::AddId(const int newId)
    {
        mIdGameList = UtilsFunctionLibrary::SetIndexArray(mIdGameList += newId, static_cast<int>(mGameList.size()));
    }

    void GameListManager::ChangeId(const int newId)
    {
        mIdGameList = Math::Clamp(newId, 0, static_cast<int>(mGameList.size()) - 1);
    }

    int GameListManager::GetGameId() const
    {
        return mIdGameList;
    }

    int GameListManager::GetSystemId() const
    {
        return mIdSystemList;
    }

    int GameListManager::GetGameListSize()
    {
        return static_cast<int>(mGameList.size());
    }

    std::vector<GameList>& GameListManager::GetAllGameList()
    {
        return mGameList;
    }

    GameList* GameListManager::GetCurrentGameList(const int index)
    {
        return &mGameList[index];
    }

    GameList* GameListManager::GetCurrentGameList()
    {
        return (!mGameList.empty()) ? &mGameList[mIdGameList] : nullptr;
    }

    SystemList* GameListManager::GetCurrentSystemList()
    {
        return &mSystemList[mIdSystemList];
    }

    void GameListManager::ClearSystemList()
    {
        mSystemList.clear();
        mSystemList.shrink_to_fit();
    }

    void GameListManager::ClearGameList()
    {
        mGameList.clear();
        mGameList.shrink_to_fit();
    }

    CurrentList GameListManager::GetCurrentList() const
    {
        return mCurrentList;
    }

    void GameListManager::GameListSortByName()
    {
        std::sort(mGameList.begin(), mGameList.end(), [](const GameList& a, const GameList& b) { return a.name < b.name; });
    }

    void GameListManager::SystemListSortByName()
    {
        std::sort(mSystemList.begin(), mSystemList.end(), [](const SystemList& a, const SystemList& b) { return a.systemLabel < b.systemLabel; });
    }

    void GameListManager::ReplaceCurrentPath(GameList* pGame) const
    {
        std::string dotSlash = "./";
        std::string slash = "/";
#ifdef _WIN32
        dotSlash = ".\\";
        slash = "\\";
#endif
        StringFunctionLibrary::ReplaceString(pGame->path, dotSlash, mSystemList[mIdSystemList].romPath + slash);
        StringFunctionLibrary::ReplaceString(pGame->image, dotSlash, mSystemList[mIdSystemList].romPath + slash);
        StringFunctionLibrary::ReplaceString(pGame->thumbnail, dotSlash, mSystemList[mIdSystemList].romPath + slash);
        StringFunctionLibrary::ReplaceString(pGame->video, dotSlash, mSystemList[mIdSystemList].romPath + slash);
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
