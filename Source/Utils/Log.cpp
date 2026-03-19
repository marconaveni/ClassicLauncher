#include "Log.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include "ClassicAssert.h"
#include "Input/InputManager.h"
#include "Utils/Print.h"
#include "Utils/Resources.h"
#include "Utils/String.h"
#include "Wrap.h"


namespace ClassicLauncher
{

    struct AnsiColor
    {
        inline static constexpr std::string_view Clear = "\x1b[0m";
        inline static constexpr std::string_view Cyan = "\x1b[36m";
        inline static constexpr std::string_view Blue = "\x1b[34m";
        inline static constexpr std::string_view White = "\x1b[37m";
        inline static constexpr std::string_view Yellow = "\x1b[33m";
        inline static constexpr std::string_view Red = "\x1b[31m";
        inline static constexpr std::string_view RedBackground = "\x1b[41m";
    };

    inline static constexpr int ClassicLogEnum = 7;
    static Log* s_instance = nullptr;

    Log::Log(Print* print)
    {
        if (!s_instance)
        {
            m_print = print;
            s_instance = this;
            rlw::SetTraceLogCallback(TraceLogger);
            std::printf("\033[?25l"); // hide cursor
            Flush();
        }
    }

    Log::~Log()
    {
        Flush();
        std::printf("\033[?25h"); // show cursor
    }

    Log* Log::Get()
    {
        return s_instance;
    }

    void Log::SetLevel(int classicLogType, int raylibLogType)
    {
        m_logClassicLevel = classicLogType;
        rlw::SetTraceLogLevel(raylibLogType);
    }

    void Log::UpdateLog()
    {
        Flush();
    }

    Print* Log::GetPrint()
    {
        CLASSIC_ASSERT(m_print, "m_print is null! Do bind before calling this function.");
        return m_print;
    }

    void Log::Flush()
    {
        if (!m_isDirty)
        {
            return;
        }

        const std::filesystem::path path = Resources::GetExecutableDirectory("cs_log.txt");
        if (path.empty())
        {
            std::printf("No settings to save.\n");
            return;
        }

        std::ofstream outStream;
        outStream.open(path, m_truncFile ? std::ios::trunc : std::ios::app);
        m_truncFile = false; // NOTE: the first time we use trunc to ensure a new file.

        if (!outStream.is_open())
        {
            std::printf("Error to open file: %s\n", path.string().c_str());
            return;
        }

        outStream << Log::m_logCache;
        outStream.close();

        m_logCache.clear(); // clean cache log
        m_isDirty = false;
    }

    void Log::LogClassic(int logType, int line, const char* file, const char* text, ...)
    {

        if (logType < m_logClassicLevel)
        {
            return;
        }

        std::filesystem::path fileName = file;

        std::string textFmt = String::TextFormat("[line:%d file:%s] %s", line, fileName.filename().string().c_str(), text);

        va_list args;
        va_start(args, text);
        TraceLogger(logType + ClassicLogEnum, textFmt.c_str(), args);
        va_end(args);
    }

    void TraceLogger(int messageType, const char* text, va_list args)
    {

        std::string prefixType{};
        std::string prefixPlain{};
        std::string prefixColored{};

        if (messageType > ClassicLogEnum)
        {
            prefixType = "[";
            messageType -= ClassicLogEnum;
        }
        else
        {
            prefixType = "[RAYLIB ";
        }

        switch (messageType)
        {
            case LogTrace:
                prefixColored = AnsiColor::Cyan;
                prefixPlain = prefixType + "TRACE] ";
                break;
            case LogDebug:
                prefixColored = AnsiColor::Blue;
                prefixPlain = prefixType + "DEBUG] ";
                break;
            case LogInfo:
                prefixColored = AnsiColor::White;
                prefixPlain = prefixType + "INFO] ";
                break;
            case LogWarning:
                prefixColored = AnsiColor::Yellow;
                prefixPlain = prefixType + "WARNING] ";
                break;
            case LogError:
                prefixColored = AnsiColor::Red;
                prefixPlain = prefixType + "ERROR] ";
                break;
            case LogFatal:
                prefixColored = AnsiColor::RedBackground;
                prefixPlain = prefixType + "FATAL] ";
                break;
        }

        std::va_list argsCopy;
        va_copy(argsCopy, args);

        int size = std::vsnprintf(nullptr, 0, text, argsCopy);
        va_end(argsCopy);

        if (size < 0)
        {
            return;
        }
        
        std::string message(size, '\0');
        std::vsnprintf(message.data(), message.size() + 1, text, args);

        const std::string textFinalColored = prefixColored + prefixPlain + message + AnsiColor::Clear.data();
        const std::string textFinal = prefixPlain + message;

        Log* log = Log::Get();

        CLASSIC_ASSERT(log, "log is null!");
        
        if (log->m_previousMessage != textFinalColored)
        {
            if (log->m_count > 0 && log->m_enableLogFile)
            {
                log->m_logCache.append(" (" + std::to_string(log->m_count) + ")");
            }

            log->m_previousMessage = textFinalColored;
            std::printf("\n%s", textFinalColored.c_str());
            
            if (log->m_enableLogFile)
            {
                log->m_count = 0;
                log->m_logCache.append("\n" + textFinal);
                log->m_isDirty = true;
            }     
        }
        else
        {
            log->m_count++;

            std::printf("\r%s (%d)", textFinalColored.c_str(), log->m_count);
            std::fflush(stdout);
        }
    }


} // namespace ClassicLauncher