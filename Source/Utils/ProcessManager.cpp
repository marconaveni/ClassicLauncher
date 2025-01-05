#include "ProcessManager.h"
#include "Application.h"
#include "Core.h"
#include "Data/GameListManager.h"
#include "Process.h"

namespace ClassicLauncher
{

    ProcessManager::ProcessManager()
        : mProcessId(0), mIsRunning(false)
    {
    }

    void ProcessManager::CreateProc(Application* pApplication)
    {
        GameListManager* gameListManager = pApplication->GetGameListManager();
        std::string fullPath = gameListManager->GetCurrentSystemList()->executable;
        fullPath.append(" ");
        fullPath.append(gameListManager->GetCurrentSystemList()->arguments);
        fullPath.append(" \"");
        fullPath.append(gameListManager->GetCurrentGameList()->path);
        fullPath.append("\" ");
        const std::string optionalWorkingDirectory = GetDirectoryPath(gameListManager->GetCurrentSystemList()->executable.c_str());
#if WIN32
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
        }
        mStatus = UpdateRun();
    }

}  // namespace ClassicLauncher
