#ifndef LOG_H
#define LOG_H

#include <cstdarg>

namespace ClassicLauncher
{
    class Print;

    // Trace log in level Classic Launcher
    // NOTE: Organized by priority level
    enum TraceLogLevel : int
    {
        LogAll = 0,             // (raylib)Display all logs
        LogTrace = 1,           // (raylib)Trace logging, intended for internal use only
        LogDebug = 2,           // (raylib)Debug logging, used for internal debugging, it should be disabled on release builds
        LogInfo = 3,            // (raylib)Info logging, used for program execution info
        LogWarning = 4,         // (raylib)Warning logging, used on recoverable failures
        LogError = 5,           // (raylib)Error logging, used on unrecoverable failures
        LogFatal = 6,           // (raylib)Fatal logging, used to abort program: exit(EXIT_FAILURE)
        LogNone = 7,            // (raylib)Disable logging
        LogClassicAll = 8,      // (ClassicLauncher)Display all logs
        LogClassicTrace = 9,    // (ClassicLauncher)Trace logging, intended for internal use only
        LogClassicDebug = 10,   // (ClassicLauncher)Debug logging, used for internal debugging, it should be disabled on release builds
        LogClassicInfo = 11,    // (ClassicLauncher)Info logging, used for program execution info
        LogClassicWarning = 12, // (ClassicLauncher)Warning logging, used on recoverable failures
        LogClassicError = 13,   // (ClassicLauncher)Error logging, used on unrecoverable failures
        LogClassicFatal = 14,   // (ClassicLauncher)Fatal logging, used to abort program: exit(EXIT_FAILURE)
        LogClassicNone = 15     // (ClassicLauncher)Disable logging
    };

    void LogLevel(int classicLogType, int raylibLogType);
    void TraceLogger(int messageType, const char* text, va_list args);
    void LogClassic(int logType, int line, const char* file, const char* text, ...);
    void UpdateLogLevel();
    void RegistryPrint(Print* print);
    Print* GetPrint();

} // namespace ClassicLauncher

#define LOG(logLevel, ...) LogClassic((logLevel), __LINE__, __FILE__, __VA_ARGS__)

#endif // LOG_H