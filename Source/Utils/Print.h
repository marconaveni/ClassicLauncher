#ifndef PRINT_H
#define PRINT_H

#include <chrono>
#include <string>
#include <vector>
#include "Core.h"
#include "Log.h"

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

        Message()
            : textMessage(""), duration(0), label(""), start(), end(), textColor(), size(0)
        {
        }

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
        void PrintOnScreen(const char* text, float duration = 2.0f, const char* label = "", const Color& textColor = Color::Cyan(), bool bLog = false);
        void DrawMessage();
        void LoadFont(const std::string& path, int size = 20, float spacing = 1);
        void Unload();

    private:

        void InternalPrintOnScreen(const std::string& text, float duration, const std::string& label, const Color& textColor, bool bLog, int sizeY);
        int mSize;
        float mSpacing;
        Font mFont;
        std::vector<Message> mMessages;
    };

}  // namespace ClassicLauncher

#endif  // PRINT
