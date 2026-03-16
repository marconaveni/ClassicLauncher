#include "Log.h"

#include <filesystem>
#include <string>

#include "Input/InputManager.h"
#include "Utils/Print.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "Wrap.h"


namespace ClassicLauncher
{
    static Print* s_print = nullptr;
    static int s_logClassicLevel = 10;

    void LogLevel(const int classicLogType, const int raylibLogType)
    {
        s_logClassicLevel = classicLogType;
        rlw::SetTraceLogLevel(raylibLogType);
    }

    void TraceLogger(int messageType, const char* text, va_list args)
    {
        if (messageType > 7 && messageType < s_logClassicLevel)
        {
            return;
        }

        std::string textFinal;

        switch (messageType)
        {
            case LogTrace: textFinal.append("\x1b[36m[RAYLIB TRACE] "); break;
            case LogDebug: textFinal.append("\x1b[34m[RAYLIB DEBUG] "); break;
            case LogInfo: textFinal.append("\x1b[37m[RAYLIB INFO] "); break;
            case LogWarning: textFinal.append("\x1B[33m[RAYLIB WARNING] "); break;
            case LogError: textFinal.append("\x1B[31m[RAYLIB ERROR] "); break;
            case LogFatal: textFinal.append("\x1B[41m[RAYLIB FATAL] "); break;
            case LogClassicTrace: textFinal.append("\x1b[36m[TRACE] "); break;
            case LogClassicDebug: textFinal.append("\x1b[34m[DEBUG] "); break;
            case LogClassicInfo: textFinal.append("\x1b[37m[INFO] "); break;
            case LogClassicWarning: textFinal.append("\x1B[33m[WARNING] "); break;
            case LogClassicError: textFinal.append("\x1B[31m[ERROR] "); break;
            case LogClassicFatal: textFinal.append("\x1B[41m[FATAL] "); break;
        }
        textFinal.append(text);
        textFinal.append("\x1B[0m\n");

        vprintf(textFinal.c_str(), args);
    }

    void LogClassic(const int logType, int line, const char* file, const char* text, ...)
    {
        if (logType > 7 && logType < s_logClassicLevel)
        {
            return;
        }

        std::filesystem::path fileName = file;

        std::string textFmt = String::TextFormat("[line:%d file:%s] %s", line, fileName.filename().string().c_str(), text);

        va_list args;
        va_start(args, text);
        TraceLogger(logType, textFmt.c_str(), args);
        va_end(args);
    }

    void UpdateLogLevel()
    {
        if (!s_print)
        {
            return;
        }

        if (Keyboard::IsReleased(Keyboard::F1))
        {
            LogLevel(LogClassicDebug, LogWarning);
            LOG(LogClassicDebug, "Enabled LOG_CLASSIC_DEBUG, LOG_WARNING");
            s_print->PrintOnScreen("Enabled LOG_CLASSIC_DEBUG, LOG_WARNING", 5.0f);
        }
        if (Keyboard::IsReleased(Keyboard::F2))
        {
            LogLevel(LogClassicDebug, LogAll);
            LOG(LogClassicDebug, "Enabled LOG_CLASSIC_DEBUG, LOG_ALL");
            s_print->PrintOnScreen("Enabled LOG_CLASSIC_DEBUG, LOG_ALL", 5.0f);
        }
        if (Keyboard::IsReleased(Keyboard::F3))
        {
            LogLevel(LogClassicAll, LogAll);
            LOG(LogClassicDebug, "Enabled LOG_CLASSIC_ALL, LOG_ALL");
            s_print->PrintOnScreen("Enabled LOG_CLASSIC_ALL, LOG_ALL", 5.0f);
        }
    }

    void RegistryPrint(Print* print)
    {
        s_print = print;
    }


    Print* GetPrint()
    {
        return s_print;
    }

} // namespace ClassicLauncher