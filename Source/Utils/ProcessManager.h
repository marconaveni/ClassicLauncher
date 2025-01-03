#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "Core.h"

namespace ClassicLauncher
{
    class GameListManager;
    class Application;

    enum class ProcessStatus
    {
        Failed = -1,
        None = 0,
        Open = 1,
        Running = 2,
        Close = 3
    };

    class ProcessManager
    {

        ProcessStatus mStatus;
    public:
#if WIN32
        unsigned int mProcessId;
#else
        int processId;
#endif
        bool mIsRunning;
        bool mIsReadyRunApp = false;

        ProcessManager();
        void CreateProc(Application* pApplication);
        ProcessStatus UpdateRun();
        bool IsApplicationRunning();
        void StatusProcessRun(Application* pApplication);
    };

}  // namespace ClassicLauncher

#endif  // PROCESS_MANAGER_H