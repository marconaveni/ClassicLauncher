#ifndef PRINT_H
#define PRINT_H

#include <chrono>
#include <string>
#include <vector>

#include "Data/Color.h"


namespace ClassicLauncher
{

    class FontManager;
    class Window;

    struct Message
    {
        Message() = default;

        std::string textMessage{};
        float duration{0.0f};
        std::string label{};
        std::chrono::time_point<std::chrono::steady_clock> start{};
        std::chrono::time_point<std::chrono::steady_clock> end{};
        Color textColor{};
        int size{0};

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

        Print(FontManager& fontManager, Window* window);
        void Init();
        void PrintOnScreen(const char* text, float duration = 2.0f, const char* label = "", const Color& textColor = Color::Cyan, bool enableLog = false);
        void DrawMessage();

    private:

        void InternalPrintOnScreen(const std::string& text, float duration, const std::string& label, const Color& textColor, bool enableLog, int sizeY);
        int m_size{16};
        float m_spacing{0};
        std::string m_fontName{};
        std::vector<Message> m_messages{};
        FontManager* m_fontManagerRef{nullptr};
        Window* m_windowRef{nullptr};
    };

} // namespace ClassicLauncher

#endif // PRINT
