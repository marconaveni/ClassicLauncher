#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <cstdint>

namespace ClassicLauncher
{
    class GameListManager;
    class Application;

    enum class ProcessStatus : std::int8_t
    {
        Failed = -1,
        None = 0,
        Open = 1,
        Running = 2,
        Close = 3
    };

    class ProcessManager
    {
    public:

#if _WIN32
        unsigned int mProcessId;
#else
        int mProcessId;
#endif
        bool mIsRunning;
        bool mIsReadyRunApp = false;

        ProcessManager();
        void CreateProc(Application* pApplication);
        ProcessStatus UpdateRun();
        [[nodiscard]]bool IsApplicationRunning() const;
        void StatusProcessRun(Application* pApplication);

    private:

        ProcessStatus mStatus;
    };

}  // namespace ClassicLauncher

#endif  // PROCESS_MANAGER_H