#include "ProcessManager.h"

#include <filesystem>


#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Data/GameListManager.h"
#include "Guis/GuiBlackScreen.h"
#include "Utils/Process.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{

    ProcessManager::ProcessManager()
        : m_status(ProcessStatus::NONE), m_processId(0), m_isRunning(false)
    {
    }

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
        const std::string optionalWorkingDirectory = rlw::GetDirectoryPath(executable.c_str());
        int status = -1;
        Process::CreateProc(m_processId, fullPath, optionalWorkingDirectory, status);
        m_status = (status == 1) ? ProcessStatus::OPEN : ProcessStatus::FAILED;
#else
        Process::CreateProc(m_processId, fullPath);
#endif
    }

    ProcessStatus ProcessManager::UpdateRun()
    {
        const bool bIsRun = Process::IsApplicationRunning(m_processId);
        if (bIsRun)
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

        return bIsRun ? ProcessStatus::RUNNING : ProcessStatus::NONE;
    }

    bool ProcessManager::IsApplicationRunning() const
    {
        return Process::IsApplicationRunning(m_processId);
    }

    void ProcessManager::StatusProcessRun(GuiBlackScreen* guiBlackScreen, AudioManager* audioManager)
    {
        switch (m_status)
        {
            case ProcessStatus::NONE: break;
            case ProcessStatus::OPEN: break;
            case ProcessStatus::RUNNING: rlw::WaitTime(2.5); break;
            case ProcessStatus::FAILED: break;
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
