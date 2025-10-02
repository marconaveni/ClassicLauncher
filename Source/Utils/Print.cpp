#include "Print.h"

#include <algorithm>
#include <iostream>

#include "Data/Vector2.h"
#include "Math.h"
#include "Utils/Resources.h"
#include "Window/RayWindow.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{
    Print::Print()
        : m_size(20), m_spacing(1), m_font()
    {
    }

    void Print::InternalPrintOnScreen(const std::string& text,
                                      float duration,
                                      const std::string& label,
                                      const Color& textColor,
                                      bool bLog,
                                      int sizeY)
    {
#ifdef _DEBUG

        if (!m_font.IsValid())
        {
            LoadFont(Resources::GetFont(), 16, 0);
        }

        bool bFound = false;
        for (Message& msg : m_messages)
        {
            if (label == msg.label)
            {
                msg.SetStart();
                msg.textMessage = text;
                msg.textColor = textColor;
                if (bLog)
                {
                    std::cout << "LOG_SCREEN: " << msg.textMessage << "\n";
                }
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            Message message;
            message.SetStart();
            message.duration = duration;
            message.textMessage = text;
            message.label = label;
            message.textColor = textColor;
            message.size = sizeY;
            m_messages.emplace_back(message);
        }
#endif
    }

    void Print::PrintOnScreen(const char* text,
                              const float duration,
                              const char* label,
                              const Color& textColor,
                              const bool bLog)
    {

#ifdef _DEBUG

        int count = 0;
        std::string message = text;
        std::string splitMessage;
        std::string labelCompare = label;
        for (const char c : message)
        {
            if (c == '\n')
            {
                std::string labelCount;
                labelCount.append((!labelCompare.empty()) ? labelCompare : std::to_string(Math::Random(1, 3000)));
                labelCount.append(std::to_string(count));
                InternalPrintOnScreen(splitMessage, duration, labelCount.c_str(), textColor, bLog, m_size - 3);
                splitMessage = "";
                count++;
                continue;
            }
            splitMessage += c;
        }

        labelCompare = (!labelCompare.empty()) ? labelCompare : std::to_string(Math::Random(1, 3000));
        InternalPrintOnScreen(splitMessage, duration, labelCompare, textColor, bLog, m_size);
#endif
    }

    void Print::DrawMessage()
    {
#ifdef _DEBUG
        if (!m_font.IsValid())
        {
            return;
        }

        m_messages.erase(std::remove_if(m_messages.begin(),
                                       m_messages.end(),
                                       [](Message& message) { return !message.IsTimeElapsed(); }),
                        m_messages.end());

        float y = 16;

        for (const auto& message : m_messages)
        {
            const Vector2f positionRender = Vector2f{30, y};
            const Vector2f positionRenderShadow = Vector2f{31, (y + 1)};

            rlw::DrawTextEx(m_font, message.textMessage.data(), positionRenderShadow, m_size, m_spacing, Color::Black);
            rlw::DrawTextEx(m_font, message.textMessage.data(), positionRender, m_size, m_spacing, message.textColor);

            y += message.size;

            if (y > RayWindow::GetScreenHeight())
            {
                break;
            }
        }
#endif
    }

    void Print::LoadFont(const std::string& path, int size, float spacing)
    {
#ifdef _DEBUG
        m_size = size;
        m_spacing = spacing;
        m_font.LoadFromFile(path.c_str(), size, nullptr, 250);
        if (!m_font.IsValid())
        {
            m_font = Font::GetFontDefault();
        }
#endif
    }

    void Print::Unload()
    {
#ifdef _DEBUG
        m_font.Unload();
#endif
    }

} // namespace ClassicLauncher