#ifndef LOG_H
#define LOG_H

#include <string>

namespace ClassicLauncher
{

    // Trace log in level Classic Launcher
    // NOTE: Organized by priority level
    typedef enum
    {
        LOG_CLASSIC_ALL = 8,       // Display all logs
        LOG_CLASSIC_TRACE = 9,     // Trace logging, intended for internal use only
        LOG_CLASSIC_DEBUG = 10,    // Debug logging, used for internal debugging, it should be disabled on release builds
        LOG_CLASSIC_INFO = 11,     // Info logging, used for program execution info
        LOG_CLASSIC_WARNING = 12,  // Warning logging, used on recoverable failures
        LOG_CLASSIC_ERROR = 13,    // Error logging, used on unrecoverable failures
        LOG_CLASSIC_FATAL = 14,    // Fatal logging, used to abort program: exit(EXIT_FAILURE)
        LOG_CLASSIC_NONE = 15      // Disable logging
    } ClassicTraceLogLevel;

    void LogLevel(int classicLogType, int raylibLogType);
    void TraceLogger(int messageType, const char* text, va_list args);
    void LogClassic(int logType, const char* text, ...);

}  // namespace ClassicLauncher



#endif  // LOG_H