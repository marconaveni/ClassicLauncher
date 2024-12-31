#include "ProcessManager.h"
#include "Data/GameListManager.h"
#include "Process.h"
#include "Core.h"

namespace ClassicLauncher
{

    ProcessManager::ProcessManager()
        : processId(0), bRunning(false)
    {
    }

    void ProcessManager::CreateProc(GameListManager* gameListManager)
    {
        std::string fullPath = gameListManager->GetCurrentSystemList()->executable;
        fullPath.append(" ");
        fullPath.append(gameListManager->GetCurrentSystemList()->arguments);
        fullPath.append(" \"");
        fullPath.append(gameListManager->GetCurrentGameList()->path);
        fullPath.append("\" ");
        const std::string optionalWorkingDirectory = GetDirectoryPath(gameListManager->GetCurrentSystemList()->executable.c_str());
        Process::CreateProc(processId, fullPath, optionalWorkingDirectory);
    }

    ProcessStatus ProcessManager::UpdateRun()
    {
        const bool bIsRun = Process::IsApplicationRunning(processId);
        if (bIsRun)
        {
            // LOG(LOGINFO, "running\n");
            if (!bRunning)
            {
                // SoundComponent::GetInstance()->PauseMusic();
                // LOG(LOGINFO, "open app\n");
                bRunning = true;
                return ProcessStatus::Open;
            }
        }
        else
        {
            if (bRunning)
            {
                // SoundComponent::GetInstance()->PlayMusic();
                // LOG(LOGINFO, "close app\n");
                bRunning = false;
                processId = 0;
                return ProcessStatus::Close;
            }
        }

        return bIsRun ? ProcessStatus::Running : ProcessStatus::None;
        // if (IsGamepadButtonReleased(0, 5))
        // {
        //     //LOG(LOGINFO, "pressed\n");
        //     Process::CloseApplicationRunning(processId);
        // }
    }

    bool ProcessManager::IsApplicationRunning()
    {
        return Process::IsApplicationRunning(processId);
    }

}  // namespace ClassicLauncher
