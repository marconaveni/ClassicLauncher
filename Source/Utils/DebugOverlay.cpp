#include "DebugOverlay.h"

#ifdef _DEBUG

    #include "Application.h"
    #include "Audio/AudioManager.h"
    #include "Data/GameListManager.h"
    #include "Engine.h"
    #include "Graphics/Texture.h"
    #include "Helper.h"
    #include "Input/InputManager.h"
    #include "Utils/Math.h"
    #include "Window/Window.h"


namespace ClassicLauncher::DebugOverlay
{

    void DrawStatistics(Window* window);
    void DrawAudioManagerStatus(AudioManager* audio);
    void DrawGameListStatus(GameListManager* gameListManager);

    static float s_delay = 0.1f;
    static int s_category = 0;

    void DrawStatistics(Window* window)
    {
        Color color = Color::Green;
        PRINT("==============Statistics================", s_delay, "statics-line", color);
        PRINT(TEXT("%d fps", window->GetFPS()), s_delay, "fps", color);
        PRINT(TEXT("%.6f ms", window->GetFrameTime()), s_delay, "ms", color);
        PRINT(TEXT("Vram %.2fMB", Math::BytesToMegabytes(Texture::GetTextureSizeBytes())), s_delay, "vram", color);
        PRINT("========================================", s_delay, "statics-line2", color);
    }

    void DrawAudioManagerStatus(AudioManager* audio)
    {
        Color color = Color::Lime;
        PRINT(TEXT("================Audio==================="), s_delay, "line-audio", color);
        PRINT(TEXT("Music Playing: \"%s\"", audio->GetMusicName().c_str()), s_delay, "music", color);
        PRINT(TEXT("Is Playing: \"%s\"", TEXTBOOL(audio->IsPlayMusic())), s_delay, "ismusic", color);
        PRINT(TEXT("========================================"), s_delay, "line-audio1", color);
    }

    void DrawGameListStatus(GameListManager* gameListManager)
    {
        Color color = Color::SkyBlue;
        GameList* gameList = gameListManager->GetCurrentGameList();
        if (gameList)
        {
            PRINT(TEXT("===========Current GameList============="), s_delay, "line-gameList", color);
            PRINT(TEXT("mapIndex: %d", gameList->mapIndex), s_delay, "gameList-mapIndex", color);
            PRINT(TEXT("Name: %s", gameList->name.c_str()), s_delay, "gameList-name", color);
            PRINT(TEXT("Path: %s", gameList->path.c_str()), s_delay, "gameList-path", color);
            PRINT(TEXT("Favorite: %s", TEXTBOOL(gameList->isFavorite)), s_delay, "gameList-favorite", color);
            PRINT(TEXT("Release Date: %s", gameList->releaseDate.ToLocaleString().c_str()), s_delay, "gameList-releaseDate", color);
            PRINT(TEXT("========================================"), s_delay, "line-gameList1", color);
        }
    }


    void Update(AudioManager* audio, GameListManager* gameListManager, Window* window)
    {

        if (Keyboard::IsReleased(Keyboard::KP_2))
        {
            s_category++;
            if (s_category > 3)
            {
                s_category = 0;
            }
        }
        else if (Keyboard::IsReleased(Keyboard::KP_1))
        {
            s_category--;
            if (s_category < 0)
            {
                s_category = 3;
            }
        }

        switch (s_category)
        {
            default: break;
            case 1: DrawStatistics(window); break;
            case 2: DrawAudioManagerStatus(audio); break;
            case 3: DrawGameListStatus(gameListManager); break;
        }
    }

} // namespace ClassicLauncher::DebugOverlay

#endif
