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
        m_fullPath = fullPath;
        std::filesystem::path pathExec(executable);
        m_optionalWorkingDirectory = pathExec.parent_path().string();
        m_status = ProcessStatus::OPEN;
    }

    void ProcessManager::UpdateRun()
    {
        const bool isRun = IsApplicationRunning();
        if (isRun)
        {
            if (!m_isRunning)
            {
                m_isRunning = true;
            }
            m_status = ProcessStatus::RUNNING;
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
            m_status = ProcessStatus::NONE;
        }
    }

    void ProcessManager::Launch()
    {
#if _WIN32
        int status = -1;
        Platform::CreateProc(m_processId, m_fullPath, m_optionalWorkingDirectory, status);
        if (status != 1) 
        { 
            m_status = ProcessStatus::FAILED;
        }
#else
        Platform::CreateProc(m_processId, m_fullPath);
        // todo criar uma checagem quando o processo falhar igual no windows
#endif
    }

    bool ProcessManager::IsApplicationRunning() const
    {
        return Platform::IsApplicationRunning(m_processId);
    }

} // namespace ClassicLauncher
