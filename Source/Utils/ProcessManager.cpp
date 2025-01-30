#include "ProcessManager.h"
#include "Application.h"
#include "Core.h"
#include "Data/GameListManager.h"
#include "Process.h"

namespace ClassicLauncher
{

    ProcessManager::ProcessManager()
        : mStatus(ProcessStatus::None), mProcessId(0), mIsRunning(false)
    {
    }

    void ProcessManager::CreateProc(Application* pApplication)
    {
        GameListManager* gameListManager = pApplication->GetGameListManager();
        GameSystemList* system = gameListManager->GetCurrentSystemList();
        GameList* game = gameListManager->GetCurrentGameList();
        const std::string executable = (game->executable.empty()) ? system->executable : game->executable;
        const std::string arguments = (game->arguments.empty()) ? system->arguments : game->arguments;
        std::string path = (game->path.empty()) ? "" : " \"" + game->path + "\" ";
        std::string fullPath = executable;
        fullPath.append(" ");
        fullPath.append(arguments);
        fullPath.append(path);
        const std::string optionalWorkingDirectory = GetDirectoryPath(executable.c_str());
#if _WIN32
        int status = -1;
        Process::CreateProc(mProcessId, fullPath, optionalWorkingDirectory, status);
        mStatus = (status == 1) ? ProcessStatus::Open : ProcessStatus::Failed;
        StatusProcessRun(pApplication);
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

    bool ProcessManager::IsApplicationRunning()
    {
        return Process::IsApplicationRunning(mProcessId);
    }

    void ProcessManager::StatusProcessRun(Application* pApplication)
    {
        switch (mStatus)
        {
            case ProcessStatus::None:
            case ProcessStatus::Open:
                break;
            case ProcessStatus::Running:
                WaitTime(2.5);
                break;
            case ProcessStatus::Failed:
            case ProcessStatus::Close:
                pApplication->GetGuiBlackScreen()->KeepBlack();
                pApplication->GetAudioManager()->ChangeMusic();
                InputManager::EnableInput();
                break;
            default:
                break; 
        }
        mStatus = UpdateRun();
    }

}  // namespace ClassicLauncher
