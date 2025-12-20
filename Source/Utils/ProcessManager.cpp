#include "ProcessManager.h"

#include <filesystem>


#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Data/GameListManager.h"
#include "Guis/GuiBlackScreen.h"
#include "Utils/Platform.h"


namespace ClassicLauncher
{

    void ProcessManager::CreateProc(GameListManager* gameListManager)
    {
        GameSystemList* system = gameListManager->GetCurrentSystemList();
        GameList* game = gameListManager->GetCurrentGameList();
        const std::string executable = (game->executable.empty()) ? system->executable : game->executable;
        const std::string arguments = (game->arguments.empty()) ? system->arguments : game->arguments;
        std::string path = (game->path.empty()) ? "" : " \"" + game->path + "\" ";
        std::string fullPath = executable;
        fullPath.append(" ");
        fullPath.append(arguments);
        fullPath.append(path);
#if _WIN32
        std::filesystem::path pathExec(executable);
        const std::string optionalWorkingDirectory = pathExec.parent_path().string();
        int status = -1;
        Platform::CreateProc(m_processId, fullPath, optionalWorkingDirectory, status);
        m_status = (status == 1) ? ProcessStatus::OPEN : ProcessStatus::FAILED;
#else
        Platform::CreateProc(m_processId, fullPath);
#endif
    }

    ProcessStatus ProcessManager::UpdateRun()
    {
        const bool isRun = Platform::IsApplicationRunning(m_processId);
        if (isRun)
        {
            if (!m_isRunning)
            {
                m_isRunning = true;
                return ProcessStatus::OPEN;
            }
        }
        else
        {
            if (m_isRunning)
            {
                m_isRunning = false;
                m_processId = 0;
                return ProcessStatus::CLOSE;
            }
        }

        return isRun ? ProcessStatus::RUNNING : ProcessStatus::NONE;
    }

    bool ProcessManager::IsApplicationRunning() const
    {
        return Platform::IsApplicationRunning(m_processId);
    }

    void ProcessManager::StatusProcessRun(GuiBlackScreen* guiBlackScreen, AudioManager* audioManager)
    {
        switch (m_status)
        {
            case ProcessStatus::NONE: break;
            case ProcessStatus::OPEN: break;
            case ProcessStatus::RUNNING: std::this_thread::sleep_for(std::chrono::seconds(3)); break;
            case ProcessStatus::FAILED:
            case ProcessStatus::CLOSE:
                guiBlackScreen->FadeOut();
                audioManager->ChangeMusic();
                InputManager::EnableInput();
                break;
            default: break;
        }
        m_status = UpdateRun();
    }

} // namespace ClassicLauncher
