#include "Themes.h"
#include "Application.h"

namespace ClassicLauncher
{

    static Themes* sInstanceThemes = nullptr;

    float Themes::GetSpriteByResolution(std::string& file)
    {
        const int monitorWidth = GetMonitorWidth(GetCurrentMonitor());
        if (monitorWidth <= 1280 && FileExists(Resources::GetSprite("1").c_str()))
        {
            file = Resources::GetSprite("1");
            LOG(LOG_CLASSIC_DEBUG, "1x sprite path [%s]", Resources::GetSprite("1").c_str());
            return 1.0f;
        }
        else if (monitorWidth <= 1920 && FileExists(Resources::GetSprite("1.5").c_str()))
        {
            file = Resources::GetSprite("1.5");
            LOG(LOG_CLASSIC_DEBUG, "1.5x sprite path [%s]", Resources::GetSprite("1.5").c_str());
            return 1.5f;
        }
        else if (monitorWidth <= 2560 && FileExists(Resources::GetSprite("2").c_str()))
        {
            file = Resources::GetSprite("2");
            LOG(LOG_CLASSIC_DEBUG, "2x sprite path [%s]", Resources::GetSprite("2").c_str());
            return 2.0f;
        }
        else if (monitorWidth <= 3840 && FileExists(Resources::GetSprite("3").c_str()))
        {
            file = Resources::GetSprite("3");
            LOG(LOG_CLASSIC_DEBUG, "3x sprite path [%s]", Resources::GetSprite("3").c_str());
            return 3.0f;
        }
        else
        {
            file = Resources::GetSprite();
            LOG(LOG_CLASSIC_DEBUG, "Default sprite path  [%s] ", Resources::GetSprite("1.5").c_str());
            return 0;
        }
    }

    Themes::Themes()
        : mScaleTexture(0.0f)
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

    void Themes::LoadSprite(Application* pApplication)
    {
        std::string file;
        const float newSize = GetSpriteByResolution(file);
        const float scale = newSize > 0 ? newSize : Themes::GetScaleTexture();

        Image img = LoadImage(file.c_str());
        SetScaleTexture(scale);

        if (newSize == 0)
        {
            const float w = 1290.0f * scale;
            const float h = 1290.0f * scale;
            pApplication->GetSpriteManager()->LoadSprite("sprite", img, (int)w, (int)h);
        }
        else
        {
            const float w = 1290.0f * scale;
            const float h = 1290.0f * scale;
            pApplication->GetSpriteManager()->LoadSprite("sprite", img, (int)w, (int)h);
            //pApplication->GetSpriteManager()->LoadSprite("sprite", img);
        }

        UnloadImage(img);
    }

    Themes& Themes::Get()
    {
        return *sInstanceThemes;
    }

    void Themes::SetScaleTexture(float scale)
    {
        const float width = GetMonitorWidth(GetCurrentMonitor());
        const float height = GetMonitorHeight(GetCurrentMonitor());
        const float size = Math::Max(width, height);
        mScaleTexture = scale > 0 ? scale : size / 1280.0f;
        // sScaleTexture = size / 1280.0f;
    }

    float Themes::GetScaleTexture()
    {
        if (sInstanceThemes == nullptr) return 0;
        if (sInstanceThemes->mScaleTexture == 0)
        {
            sInstanceThemes->SetScaleTexture();
        }
        return sInstanceThemes->mScaleTexture;
    }

}  // namespace ClassicLauncher