#include "ThemesManager.h"

#include <filesystem>

#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Helper.h"
#include "Utils/ConfigurationManager.h"
#include "Utils/Log.h"
#include "Utils/Math.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "Audio/AudioManager.h"

namespace ClassicLauncher
{

    static ThemesManager* s_instanceThemes = nullptr;

    ThemesManager::ThemesManager(GameListManager* gameListManager,
                                 SpriteManager* spriteManager,
                                 EntityManager* entityManagerRef,
                                 ConfigurationManager* configManager,
                                 AudioManager* audioManagerRef)
        : m_gameListManager(gameListManager)
        , m_spriteManager(spriteManager)
        , m_entityManagerRef(entityManagerRef)
        , m_configManagerRef(configManager)
        , m_audioManagerRef(audioManagerRef)
    {
        if (s_instanceThemes == nullptr)
        {
            s_instanceThemes = this;
        }
    }

    ThemesManager::~ThemesManager()
    {
        s_instanceThemes = nullptr;
    }

    void ThemesManager::Init()
    {
        m_scaleTexture = Math::Clamp(m_configManagerRef->GetInternalScale(), 1, 2);
        m_spriteManager->LoadSprite("sprite", Resources::GetSpriteFile());
        UpdateTheme();
    }

    void ThemesManager::UpdateTheme()
    {
        if (m_gameListManager->GetCurrentList() == CurrentList::SystemListSelect)
        {
            m_audioManagerRef->LoadSound(Resources::GetClickAudioFile(), "click");
            m_audioManagerRef->LoadSound(Resources::GetCursorAudioFile(), "cursor");
            m_entityManagerRef->SetThemeValue();
            return;
        }

        GameSystemList* systemList = m_gameListManager->GetCurrentSystemList();
        if (systemList->theme.isDirectoryExist)
        {
            const std::string& filename = String::NormalizePath(TEXT("/sprite%.0fx.png", ThemesManager::GetScaleRenderer()));
            std::filesystem::path path = systemList->theme.path.string() + filename;
            if (!systemList->theme.isLoaded)
            {
                systemList->theme.sprite = "sprite_" + systemList->systemName;
                m_spriteManager->LoadSprite(systemList->theme.sprite, path.string());
                path.replace_filename("config.cfg");
                systemList->configThemes.LoadConfigurations(path.string());
                systemList->theme.isLoaded = true;
            }
            
            path.replace_filename("click.wav");
            m_audioManagerRef->LoadSound(path, "click");
            path.replace_filename("cursor.wav");
            m_audioManagerRef->LoadSound(path, "cursor");
        }
        m_entityManagerRef->SetThemeValue();
    }

    ThemesManager& ThemesManager::Get()
    {
        return *s_instanceThemes;
    }

    float ThemesManager::GetScaleRenderer()
    {
        if (s_instanceThemes == nullptr)
        {
            return 1;
        }

        return s_instanceThemes->m_scaleTexture;
    }

    ConfigurationThemes ThemesManager::GetConfigurationThemes()
    {
        if (s_instanceThemes == nullptr)
        {
            return ConfigurationThemes();
        }

        if (s_instanceThemes->m_gameListManager->GetCurrentList() == CurrentList::GameListSelect)
        {
            return s_instanceThemes->m_gameListManager->GetCurrentSystemList()->configThemes;
        }
        return ConfigurationThemes();
    }

    std::string ThemesManager::GetSpriteTheme()
    {
        if (m_gameListManager->GetCurrentList() == CurrentList::SystemListSelect)
        {
            return "sprite";
        }

        GameSystemList* systemList = m_gameListManager->GetCurrentSystemList();
        return systemList->theme.sprite;
    }

} // namespace ClassicLauncher