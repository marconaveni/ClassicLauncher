#include "ProcessManager.h"

#include <filesystem>

#include "Audio/AudioManager.h"
#include "Data/GameListManager.h"
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

    void ProcessManager::UpdateRun()
    {
        const bool isRun = IsApplicationRunning();
        if (isRun)
        {
            if (!m_isRunning)
            {
                m_isRunning = true;
                m_status = ProcessStatus::OPEN;
                return;
            }
        }
        else
        {
            if (m_isRunning)
            {
                m_isRunning = false;
                m_processId = 0;
                m_status = ProcessStatus::CLOSE;
                return;
            }
        }

        m_status = isRun ? ProcessStatus::RUNNING : ProcessStatus::NONE;
    }

    bool ProcessManager::IsApplicationRunning() const
    {
        return Platform::IsApplicationRunning(m_processId);
    }

} // namespace ClassicLauncher
