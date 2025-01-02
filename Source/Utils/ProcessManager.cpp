#include "ProcessManager.h"
#include "Application.h"
#include "Core.h"
#include "Data/GameListManager.h"
#include "Process.h"

namespace ClassicLauncher
{

    ProcessManager::ProcessManager()
        : processId(0), bRunning(false)
    {
    }

    void ProcessManager::CreateProc(Application* application)
    {
        GameListManager* gameListManager = application->GetGameListManager();
        std::string fullPath = gameListManager->GetCurrentSystemList()->executable;
        fullPath.append(" ");
        fullPath.append(gameListManager->GetCurrentSystemList()->arguments);
        fullPath.append(" \"");
        fullPath.append(gameListManager->GetCurrentGameList()->path);
        fullPath.append("\" ");
        const std::string optionalWorkingDirectory = GetDirectoryPath(gameListManager->GetCurrentSystemList()->executable.c_str());
#if WIN32
        int status = -1;
        Process::CreateProc(processId, fullPath, optionalWorkingDirectory, status);
        mStatus = (status == 1) ? ProcessStatus::Open : ProcessStatus::Failed;
        StatusProcessRun(application);
#else
        Process::CreateProc(processId, fullPath);
#endif
    }

    ProcessStatus ProcessManager::UpdateRun()
    {
        const bool bIsRun = Process::IsApplicationRunning(processId);
        if (bIsRun)
        {
            if (!bRunning)
            {
                bRunning = true;
                return ProcessStatus::Open;
            }
        }
        else
        {
            if (bRunning)
            {
                bRunning = false;
                processId = 0;
                return ProcessStatus::Close;
            }
        }

        return bIsRun ? ProcessStatus::Running : ProcessStatus::None;
    }

    bool ProcessManager::IsApplicationRunning()
    {
        return Process::IsApplicationRunning(processId);
    }

    void ProcessManager::StatusProcessRun(Application* application)
    {

        switch (mStatus)
        {
            case ProcessStatus::Open:
                break;
            case ProcessStatus::Running:
                WaitTime(2.5);
                break;
            case ProcessStatus::Failed:
            case ProcessStatus::Close:
                application->GetGuiBlackScreen()->KeepBlack();
                application->GetAudioManager()->ChangeMusic();
                InputManager::EnableInput();
                break;
        }
        mStatus = UpdateRun();
    }

}  // namespace ClassicLauncher
