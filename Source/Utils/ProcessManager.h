#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <cstdint>

namespace ClassicLauncher
{
    class GameListManager;
    class GuiBlackScreen;
    class AudioManager;
    // class Application;


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


        void CreateProc(GameListManager* gameListManager);
        ProcessStatus UpdateRun();
        [[nodiscard]] bool IsApplicationRunning() const;
        void StatusProcessRun(GuiBlackScreen* guiBlackScreen, AudioManager* audioManager);

    private:

        ProcessStatus mStatus;
    };

} // namespace ClassicLauncher

#endif // PROCESS_MANAGER_H