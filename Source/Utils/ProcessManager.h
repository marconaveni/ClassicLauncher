#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <cstdint>

namespace ClassicLauncher
{
    class GameListManager;
    class GuiBlackScreen;
    class AudioManager;


    enum class ProcessStatus : std::int8_t
    {
        FAILED = -1,
        NONE = 0,
        OPEN = 1,
        RUNNING = 2,
        CLOSE = 3
    };

    class ProcessManager
    {
    public:

#if _WIN32
        unsigned int m_processId;
#else
        int m_processId;
#endif
        bool m_isRunning;
        bool m_isReadyRunApp = false;

        ProcessManager();


        void CreateProc(GameListManager* gameListManager);
        ProcessStatus UpdateRun();
        [[nodiscard]] bool IsApplicationRunning() const;
        void StatusProcessRun(GuiBlackScreen* guiBlackScreen, AudioManager* audioManager);

    private:

        ProcessStatus m_status;
    };

} // namespace ClassicLauncher

#endif // PROCESS_MANAGER_H