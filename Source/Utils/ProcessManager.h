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

        ProcessManager() = default;

#if _WIN32
        unsigned int m_processId{0};
#else
        int m_processId{0};
#endif
        bool m_isRunning{false};
        bool m_isReadyRunApp{false};

        void CreateProc(GameListManager* gameListManager);
        ProcessStatus UpdateRun();
        [[nodiscard]] bool IsApplicationRunning() const;
        void StatusProcessRun(GuiBlackScreen* guiBlackScreen, AudioManager* audioManager);

    private:

        ProcessStatus m_status{ProcessStatus::NONE};
    };

} // namespace ClassicLauncher

#endif // PROCESS_MANAGER_H