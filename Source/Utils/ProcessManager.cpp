#include "ProcessManager.h"

#include <filesystem>

#include "Application.h"
#include "Audio/AudioManager.h"
#include "Data/GameListManager.h"
#include "Guis/GuiBlackScreen.h"
#include "Utils/Process.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{

    ProcessManager::ProcessManager()
        : mStatus(ProcessStatus::None), mProcessId(0), mIsRunning(false)
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
        const std::string optionalWorkingDirectory = rlw::GetDirectoryPath(executable.c_str());
#if _WIN32
        int status = -1;
        Process::CreateProc(mProcessId, fullPath, optionalWorkingDirectory, status);
        mStatus = (status == 1) ? ProcessStatus::Open : ProcessStatus::Failed;
        //StatusProcessRun(pApplication);
#else
        Process::CreateProc(mProcessId, fullPath);
#endif
    }

    ProcessStatus ProcessManager::UpdateRun()
    {
        const bool bIsRun = Process::IsApplicationRunning(mProcessId);
        if (bIsRun)
        {
            if (!mIsRunning)
            {
                mIsRunning = true;
                return ProcessStatus::Open;
            }
        }
        else
        {
            if (mIsRunning)
            {
                mIsRunning = false;
                mProcessId = 0;
                return ProcessStatus::Close;
            }
        }

        return bIsRun ? ProcessStatus::Running : ProcessStatus::None;
    }

    bool ProcessManager::IsApplicationRunning() const
    {
        return Process::IsApplicationRunning(mProcessId);
    }

    void ProcessManager::StatusProcessRun(GuiBlackScreen* guiBlackScreen, AudioManager* audioManager)
    {
        switch (mStatus)
        {
            case ProcessStatus::None: break;
            case ProcessStatus::Open: break;
            case ProcessStatus::Running: rlw::WaitTime(2.5); break;
            case ProcessStatus::Failed: break;
            case ProcessStatus::Close:
                guiBlackScreen->KeepBlack();
                audioManager->ChangeMusic();
                InputManager::EnableInput();
                break;
            default: break;
        }
        mStatus = UpdateRun();
    }

} // namespace ClassicLauncher
