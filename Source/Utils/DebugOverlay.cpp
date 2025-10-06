#include "DebugOverlay.h"

#ifdef _DEBUG

#include "Application.h"
#include "Audio/AudioManager.h"
#include "Data/GameListManager.h"
#include "Helper.h"
#include "Window/RayWindow.h"
#include "Input/InputManager.h"

namespace ClassicLauncher::DebugOverlay
{

    void DrawStatistics();
    void DrawAudioManagerStatus(AudioManager* audio);
    void DrawGameListStatus(GameListManager* gameListManager);

    static float s_delay = 0.1f;
    static int s_category = 0;

    void DrawStatistics()
    {
        Color color = Color::Green;
        PRINT("==============Statistics================", s_delay, "statics-line", color);
        PRINT(TEXT("%d fps", RayWindow::GetFPS()), s_delay, "fps", color);
        PRINT(TEXT("%.6f ms", RayWindow::GetFrameTime()), s_delay, "ms", color);
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
        GameList* pGameList = gameListManager->GetCurrentGameList();
        if (pGameList)
        {
            PRINT(TEXT("===========Current GameList============="), s_delay, "line-gameList", color);
            PRINT(TEXT("mapIndex: %d", pGameList->mapIndex), s_delay, "gameList-mapIndex", color);
            PRINT(TEXT("Name: %s", pGameList->name.c_str()), s_delay, "gameList-name", color);
            PRINT(TEXT("Path: %s", pGameList->path.c_str()), s_delay, "gameList-path", color);
            PRINT(TEXT("Favorite: %s", TEXTBOOL(pGameList->bFavorite)), s_delay, "gameList-favorite", color);
            PRINT(TEXT("========================================"), s_delay, "line-gameList1", color);
        }
    }


    void Update(AudioManager* audio, GameListManager* gameListManager)
    {

        if (Keyboard::IsReleased(Keyboard::KP_1))
        {
            s_category++;
            if (s_category > 2)
            {
                s_category = 0;
            }
        }
        else if (Keyboard::IsReleased(Keyboard::KP_2))
        {
            s_category--;
            if (s_category < 0)
            {
                s_category = 2;
            }      
        }
        
        switch (s_category)
        {
            case 0: DrawStatistics(); break;
            case 1: DrawAudioManagerStatus(audio); break;
            case 2: DrawGameListStatus(gameListManager); break;
            default: break;
        }
    }

} // namespace ClassicLauncher::DebugOverlay

#endif