#ifndef LOG_H
#define LOG_H

#include <cstdarg>

namespace ClassicLauncher
{

    // Trace log in level Classic Launcher
    // NOTE: Organized by priority level
    typedef enum
    {
        LOG_ALL = 0,               // (raylib)Display all logs
        LOG_TRACE = 1,             // (raylib)Trace logging, intended for internal use only
        LOG_DEBUG = 2,             // (raylib)Debug logging, used for internal debugging, it should be disabled on release builds
        LOG_INFO = 3,              // (raylib)Info logging, used for program execution info
        LOG_WARNING = 4,           // (raylib)Warning logging, used on recoverable failures
        LOG_ERROR = 5,             // (raylib)Error logging, used on unrecoverable failures
        LOG_FATAL = 6,             // (raylib)Fatal logging, used to abort program: exit(EXIT_FAILURE)
        LOG_NONE = 7,              // (raylib)Disable logging
        LOG_CLASSIC_ALL = 8,       // (ClassicLauncher)Display all logs
        LOG_CLASSIC_TRACE = 9,     // (ClassicLauncher)Trace logging, intended for internal use only
        LOG_CLASSIC_DEBUG = 10,    // (ClassicLauncher)Debug logging, used for internal debugging, it should be disabled on release builds
        LOG_CLASSIC_INFO = 11,     // (ClassicLauncher)Info logging, used for program execution info
        LOG_CLASSIC_WARNING = 12,  // (ClassicLauncher)Warning logging, used on recoverable failures
        LOG_CLASSIC_ERROR = 13,    // (ClassicLauncher)Error logging, used on unrecoverable failures
        LOG_CLASSIC_FATAL = 14,    // (ClassicLauncher)Fatal logging, used to abort program: exit(EXIT_FAILURE)
        LOG_CLASSIC_NONE = 15      // (ClassicLauncher)Disable logging
    } ClassicTraceLogLevel;

    void LogLevel(int classicLogType, int raylibLogType);
    void TraceLogger(int messageType, const char* text, va_list args);
    void LogClassic(int logType, const char* text, ...);

}  // namespace ClassicLauncher

#define LOG(logLevel, ...) LogClassic((logLevel), __VA_ARGS__)

#endif  // LOG_H