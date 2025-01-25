#include "Themes.h"
#include <math.h> /* modf */
#include "Application.h"

namespace ClassicLauncher
{

    static Themes* sInstanceThemes = nullptr;

    Themes::Themes()
        : mScaleTexture(1.0f), mScaleSystem(1.0f)
    {
        if (sInstanceThemes == nullptr)
        {
            sInstanceThemes = this;
        }
    }

    Themes::~Themes()
    {
        sInstanceThemes = nullptr;
    }

    std::vector<std::string> Themes::GetThemeDirs()
    {
        std::string path = StringFunctionLibrary::NormalizePath(Resources::GetClassicLauncherDir() + "themes/" + mCurrentSystemName + "/");
        std::vector<std::string> paths;
        if (DirectoryExists(path.c_str()))
        {
            paths.push_back(path);
        }
        paths.push_back(Resources::GetResourcesPathFileAbs("Resources/textures/"));
        return paths;
    }

    bool Themes::GetPathTheme(std::string& file, int monitorWidth, int monitorCompare, const std::string& path, float numScale)
    {
        if (monitorWidth <= monitorCompare && FileExists(path.c_str()))
        {
            file = path;
            LOG(LOG_CLASSIC_DEBUG, "%.1f x sprite path [%s]", numScale, path.c_str());
            return true;
        }
        return false;
    }

    float Themes::GetSpriteByResolution(std::string& file)
    {
#ifdef FORCE_THEME_1X
        file = StringFunctionLibrary::NormalizePath(Resources::GetClassicLauncherDir() + "themes/debug/sprite-debug.png")  ;
        return 1;
#else
        std::vector<std::string> paths;
        paths = GetThemeDirs();
        const int monitorWidth = GetMonitorWidth(GetCurrentMonitor());
        float scales[4] = { 1.0f, 1.5f, 2.0f, 3.0f };
        int widths[4] = { 1280, 1920, 2580, 3840 };

        for (const auto& path : paths)
        {
            for (int i = 0; i < 4; i++)
            {
                float intPart;
                float fractpart = modf(scales[i], &intPart);
                std::string numText = (fractpart > 0) ? TEXT("%.1f", scales[i]) : TEXT("%.0f", intPart);

                if (GetPathTheme(file, monitorWidth, widths[i], TEXT("%ssprite%sx.png", path.c_str(), numText.c_str()), scales[i]))
                {
                    return scales[i];
                }
            }
        }

        file = Resources::GetSprite();
        LOG(LOG_CLASSIC_DEBUG, "Default sprite path  [%s] ", Resources::GetSprite().c_str());
        return 1.0f;
#endif
    }

    void Themes::Init(Application* pApplication)
    {
        std::vector<SystemList*> systems = pApplication->GetGameListManager()->GetAllSystemList();
        for (auto& system : systems)
        {
            mCurrentSystemName = system->systemName;
            std::string file;
            system->scale = GetSpriteByResolution(file);
            system->pathTheme = file;
        }
        mCurrentSystemName = "default";
        mScaleSystem = GetSpriteByResolution(mPathThemeSystem);
    }

    void Themes::LoadTheme(Application* pApplication)
    {
        std::string file;
        if (pApplication->GetGameListManager()->GetCurrentList() == CurrentList::GameListSelect)
        {
            SystemList* pList = pApplication->GetGameListManager()->GetCurrentSystemList();
            mCurrentSystemName = pList->systemName;
            file = pList->pathTheme;
            mScaleTexture = pList->scale;
        }
        else
        {
            mCurrentSystemName = "default";
            file = mPathThemeSystem;
            mScaleTexture = mScaleSystem;
        }

        if (mLastPathLoaded != file)
        {
            pApplication->GetSpriteManager()->DeleteSprite("sprite");
            pApplication->GetSpriteManager()->LoadSprite("sprite", file);
            mLastPathLoaded = file;
        }
    }

    Themes& Themes::Get()
    {
        return *sInstanceThemes;
    }

    float Themes::GetScaleTexture()
    {
        if (sInstanceThemes == nullptr) return 0;

        return sInstanceThemes->mScaleTexture;
    }


}  // namespace ClassicLauncher