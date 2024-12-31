#include "Log.h"
#include <iostream>
#include <string>
#include "raylib.h"

namespace ClassicLauncher
{

    static int sLogClassicLevel = 10;

    void LogLevel(int classicLogType, int raylibLogType)
    {
#ifdef _DEBUG
        sLogClassicLevel = classicLogType;
        SetTraceLogLevel(raylibLogType);
#else
        sLogClassicLevel = 12;
        SetTraceLogLevel(5);
#endif
    }

    void TraceLogger(int messageType, const char* text, va_list args)
    {
        if (messageType > 7 && messageType < sLogClassicLevel) return;

        std::string textFinal;

        switch (messageType)
        {
            case LOG_TRACE:
                textFinal.append("\x1b[36m[RAYLIB TRACE] ");
                break;
            case LOG_DEBUG:
                textFinal.append("\x1b[34m[RAYLIB DEBUG] ");
                break;
            case LOG_INFO:
                textFinal.append("\x1b[37m[RAYLIB INFO] ");
                break;
            case LOG_WARNING:
                textFinal.append("\x1B[33m[RAYLIB WARNING] ");
                break;
            case LOG_ERROR:
                textFinal.append("\x1B[31m[RAYLIB ERROR] ");
                break;
            case LOG_FATAL:
                textFinal.append("\x1B[41m[RAYLIB FATAL] ");
                break;
            case LOG_CLASSIC_TRACE:
                textFinal.append("\x1b[36m[TRACE] ");
                break;
            case LOG_CLASSIC_DEBUG:
                textFinal.append("\x1b[34m[DEBUG] ");
                break;
            case LOG_CLASSIC_INFO:
                textFinal.append("\x1b[37m[INFO] ");
                break;
            case LOG_CLASSIC_WARNING:
                textFinal.append("\x1B[33m[WARNING] ");
                break;
            case LOG_CLASSIC_ERROR:
                textFinal.append("\x1B[31m[ERROR] ");
                break;
            case LOG_CLASSIC_FATAL:
                textFinal.append("\x1B[41m[FATAL] ");
                break;
        }
        textFinal.append(text);
        textFinal.append("\x1B[0m\n");

        vprintf(textFinal.c_str(), args);
    }

    void LogClassic(int logType, const char* text, ...)
    {
        if (logType > 7 && logType < sLogClassicLevel) return;

        va_list args;
        va_start(args, text);
        TraceLogger(logType, text, args);
        va_end(args);
    }

}  // namespace ClassicLauncher