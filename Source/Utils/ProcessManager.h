#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <cstdint>
#include <string>

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
        void UpdateRun();
        void Launch();
        [[nodiscard]] bool IsApplicationRunning() const;
        [[nodiscard]] ProcessStatus GetStatus() const { return m_status; }

    private:

        ProcessStatus m_status{ProcessStatus::NONE};
        std::string m_fullPath{};
    };

} // namespace ClassicLauncher

#endif // PROCESS_MANAGER_H