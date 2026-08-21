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
        Failed = -1,
        None = 0,
        Open = 1,
        Running = 2,
        Close = 3
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

        ProcessStatus m_status{ProcessStatus::None};
        std::string m_fullPath{};
        std::string m_optionalWorkingDirectory{};
    };

} // namespace ClassicLauncher

#endif // PROCESS_MANAGER_H