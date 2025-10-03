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
#include "rl_wrap.h"

namespace ClassicLauncher
{

    static ThemesManager* s_instanceThemes = nullptr;

    ThemesManager::ThemesManager(GameListManager* gameListManager,
                                 SpriteManager* spriteManager,
                                 EntityManager* entityManagerRef,
                                 ConfigurationManager* configManager)
        : m_scaleTexture(1.0f)
        , m_gameListManager(gameListManager)
        , m_spriteManager(spriteManager)
        , m_entityManagerRef(entityManagerRef)
        , m_configManagerRef(configManager)
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
    }

    void ThemesManager::UpdateTheme()
    {
        GameSystemList* systemList = m_gameListManager->GetCurrentSystemList();
        if (systemList->theme != "sprite")
        {
            m_spriteManager->LoadSprite(systemList->systemName, systemList->theme);
        }
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

    std::string ThemesManager::GetTheme()
    {
        GameSystemList* systemList = m_gameListManager->GetCurrentSystemList();
        if (systemList->theme != "sprite" && m_gameListManager->GetCurrentList() == CurrentList::GameListSelect)
        {
           return systemList->systemName;
        }
        return "sprite";
    }

} // namespace ClassicLauncher