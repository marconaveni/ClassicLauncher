#ifndef LOG_H
#define LOG_H

#include <cstdarg>
#include <string>

namespace ClassicLauncher
{
    class Print;

    // Trace log in level Classic Launcher
    // NOTE: Organized by priority level
    enum TraceLogLevel : int
    {
        LogAll = 0,             // Display all logs
        LogTrace = 1,           // Trace logging, intended for internal use only
        LogDebug = 2,           // Debug logging, used for internal debugging, it should be disabled on release builds
        LogInfo = 3,            // Info logging, used for program execution info
        LogWarning = 4,         // Warning logging, used on recoverable failures
        LogError = 5,           // Error logging, used on unrecoverable failures
        LogFatal = 6,           // Fatal logging, used to abort program: exit(EXIT_FAILURE)
        LogNone = 7,            // Disable logging
    };

    class Log
    {
    public:

        Log(Print* print);
        ~Log();
        static Log* Get();
        Print* GetPrint();
        void SetLevel(int classicLogType, int raylibLogType);
        void UpdateLog();
        void Flush();
        void LogClassic(int logType, int line, const char* file, const char* text, ...);
        void EnableLogFile(bool enable) { m_enableLogFile = enable; }
        friend void TraceLogger(int messageType, const char* text, va_list args);

    private:

        Print* m_print{nullptr};
        int m_logClassicLevel{6};
        std::string m_logCache{};
        std::string m_previousMessage{};
        bool m_isDirty{false};
        int m_count{0};
        bool m_truncFile{true};
        bool m_enableLogFile{true};
    };

    void TraceLogger(int messageType, const char* text, va_list args);

} // namespace ClassicLauncher

#define LOG(logLevel, ...) Log::Get()->LogClassic((logLevel), __LINE__, __FILE__, __VA_ARGS__)

#endif // LOG_H