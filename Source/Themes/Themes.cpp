#include "Themes.h"

#include <filesystem>

#include "Utils/ConfigurationManager.h"
#include "Graphics/SpriteManager.h"
#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Helper.h"
#include "Utils/Log.h"
#include "Utils/Math.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{

    static ThemesManager* sInstanceThemes = nullptr;

    ThemesManager::ThemesManager(GameListManager* gameListManager,
                                 SpriteManager* spriteManager,
                                 EntityManager* entityManagerRef,
                                 ConfigurationManager* configManager)
        : mScaleTexture(1.0f)
        , mScaleSystem(1.0f)
        , m_gameListManager(gameListManager)
        , m_spriteManager(spriteManager)
        , m_entityManagerRef(entityManagerRef)
        , m_configManagerRef(configManager)
    {
        if (sInstanceThemes == nullptr)
        {
            sInstanceThemes = this;
        }
    }

    ThemesManager::~ThemesManager()
    {
        sInstanceThemes = nullptr;
    }

    std::vector<std::string> ThemesManager::GetThemeDirs()
    {
        // repeat code todo remove this after refactor
        std::string path = String::NormalizePath(Resources::GetClassicLauncherDir() + "themes/" + mCurrentSystemName + "/");
        std::vector<std::string> paths;
        // if (rlw::DirectoryExists(path.c_str()))
        if (std::filesystem::exists(path))
        {
            paths.emplace_back(path);
        }
        paths.emplace_back(Resources::GetResourcesPathFileAbs("Resources/textures/"));
        return paths;
    }

    bool ThemesManager::GetPathTheme(std::string& file,
                                     int monitorWidth,
                                     int monitorCompare,
                                     const std::string& path,
                                     float numScale)
    {
        // if (monitorWidth <= monitorCompare && rlw::FileExists(path.c_str()))
        if (monitorWidth <= monitorCompare && std::filesystem::exists(path))
        {
            file = path;
            LOG(LOG_CLASSIC_DEBUG, "%.1f x sprite path [%s]", numScale, path.c_str());
            return true;
        }
        return false;
    }

    float ThemesManager::GetSpriteByResolution(std::string& file)
    {
        if (m_configManagerRef->GetForceInternalScale())
        {
            file = String::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/sprite.png");
            return Math::Clamp(m_configManagerRef->GetInternalScale(), 1, 3);
        }

        std::vector<std::string> paths;
        paths = GetThemeDirs();
        const int monitorWidth = 1920; // todo refactor  parte delicada precisa de refactor urgente rlw::GetMonitorWidth(rlw::GetCurrentMonitor());
        int scales[3] = {1, 2, 3};
        int widths[3] = {1280, 2560, 3840}; //2560

        for (const auto& path : paths)
        {
            for (int i = 0; i < 3; i++)
            {
                const bool result = GetPathTheme(file, monitorWidth, widths[i],
                                                 TEXT("%ssprite%dx.png", path.c_str(), scales[i]), scales[i]);
                if (result)
                {
                    return static_cast<float>(scales[i]);
                }
            }
        }

        file = Resources::GetSprite();
        LOG(LOG_CLASSIC_DEBUG, "Default sprite path  [%s] ", Resources::GetSprite().c_str());
        return 1.0f;
    }

    void ThemesManager::Init()
    {
        std::vector<GameSystemList*> systems = m_gameListManager->GetAllSystemList();
        for (auto& system : systems)
        {
            mCurrentSystemName = system->systemName;
            std::string file;
            system->scale = GetSpriteByResolution(file);
            system->pathImageTheme = file;
        }
        mCurrentSystemName = "default";
        mScaleSystem = GetSpriteByResolution(mPathThemeSystem);
    }

    void ThemesManager::LoadTheme()
    {
        std::string file;
        if (m_gameListManager->GetCurrentList() == CurrentList::GameListSelect)
        {
            GameSystemList* pList = m_gameListManager->GetCurrentSystemList();
            mCurrentSystemName = pList->systemName;
            file = pList->pathImageTheme;
            mScaleTexture = pList->scale;
        }
        else
        {
            mCurrentSystemName = "default";
            file = mPathThemeSystem;
            mScaleTexture = mScaleSystem;
        }

        LoadConfigurationThemes();

        if (mLastPathLoaded != file)
        {
            m_spriteManager->DeleteSprite("sprite");
            m_spriteManager->LoadSprite("sprite", file);
            mLastPathLoaded = file;
        }
    }

    ThemesManager& ThemesManager::Get()
    {
        return *sInstanceThemes;
    }

    void ThemesManager::LoadConfigurationThemes()
    {
        const std::string path = Resources::GetClassicLauncherDir() + "themes/" + mCurrentSystemName + "/config.cfg";
        mConfigurationThemes.LoadConfigurations(String::NormalizePath(path));
        m_entityManagerRef->SetThemeValue();
    }

    float ThemesManager::GetScaleTexture()
    {
        if (sInstanceThemes == nullptr)
        {
            return 0;
        }

        return sInstanceThemes->mScaleTexture;
    }

} // namespace ClassicLauncher