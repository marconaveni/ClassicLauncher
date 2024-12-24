#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "Core.h"

namespace ClassicLauncher
{
    class GameListManager;

    enum class ProcessStatus
    {
        None = 0,
        Open = 1,
        Running = 2,
        Close = 3
    };

    class ProcessManager
    {
    public:

        unsigned int processId;
        bool bRunning;

        ProcessManager();
        void CreateProc(GameListManager* gameListManager);
        ProcessStatus UpdateRun();
        bool IsApplicationRunning();
    };

}  // namespace ClassicLauncher

#endif  // PROCESS_MANAGER_H