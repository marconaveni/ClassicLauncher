#ifndef PRINT_H
#define PRINT_H

#include <chrono>
#include <string>
#include <vector>
#include "Core.h"
#include "raylib.h"

namespace ClassicLauncher
{

    struct Message
    {
        std::string textMessage;
        float duration;
        std::string label;
        std::chrono::time_point<std::chrono::steady_clock> start;
        std::chrono::time_point<std::chrono::steady_clock> end;
        Color textColor;
        int size;

        Message() : textMessage(""), duration(0), label(""), start(), end(), textColor(), size(0) {}

        void SetStart() { start = std::chrono::steady_clock::now(); }

        void SetEnd() { end = std::chrono::steady_clock::now(); }

        bool IsTimeElapsed()
        {
            SetEnd();
            const std::chrono::duration<float> elapsedTime = end - start;
            const float time = elapsedTime.count();
            return time < duration;
        }
    };

    class Print
    {
    public:

        Print();
        void PrintOnScreen(const char* text, float duration = 2.0f, const char* label = "", const Color& textColor = Color{ 0, 230, 230, 230 }, bool bLog = false);
        static void Log(int messageType, const char* text, va_list args);
        static void Log(TraceLogLevel logLevel, const char* text);
        void DrawMessage();
        void LoadFont(const std::string& path, int size = 20, float spacing = 1);
        void Unload();

    private:

        void InternalPrintOnScreen(const std::string& text, float duration, const std::string& label, const Color& textColor, bool bLog, int sizeY);
        Font fontTtf;
        int size;
        float spacing;
        std::vector<Message> messages;
        TraceLogLevel levelLog;
    };

}  // namespace ClassicLauncher

#endif  // PRINT
