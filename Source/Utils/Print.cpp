#include "Print.h"

#include <algorithm>
#include <iostream>

#include "Data/Vector2.h"
#include "Math.h"
#include "Utils/Resources.h"
#include "Window/RayWindow.h"
#include "rl_wrap.h"
#include "Graphics/FontManager.h"

namespace ClassicLauncher
{



    
    Print::Print(FontManager& fontManager)
        : m_fontManagerRef(&fontManager)
    {
    }

    void Print::Init()
    {
        m_fontName = m_fontManagerRef->Load(Resources::GetFontFile(), m_size);
    }

    void Print::InternalPrintOnScreen(const std::string& text,
                                      float duration,
                                      const std::string& label,
                                      const Color& textColor,
                                      bool enableLog,
                                      int sizeY)
    {
#ifdef _DEBUG

        bool bFound = false;
        for (Message& msg : m_messages)
        {
            if (label == msg.label)
            {
                msg.SetStart();
                msg.textMessage = text;
                msg.textColor = textColor;
                if (enableLog)
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
                              const bool enableLog)
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
                InternalPrintOnScreen(splitMessage, duration, labelCount.c_str(), textColor, enableLog, m_size - 3);
                splitMessage = "";
                count++;
                continue;
            }
            splitMessage += c;
        }

        labelCompare = (!labelCompare.empty()) ? labelCompare : std::to_string(Math::Random(1, 3000));
        InternalPrintOnScreen(splitMessage, duration, labelCompare, textColor, enableLog, m_size);
#endif
    }

    void Print::DrawMessage()
    {
#ifdef _DEBUG

        Font* font = m_fontManagerRef->GetFont(m_fontName);
        if (!font)
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

            rlw::DrawTextEx(*font, message.textMessage.data(), positionRenderShadow, m_size, m_spacing, Color::Black);
            rlw::DrawTextEx(*font, message.textMessage.data(), positionRender, m_size, m_spacing, message.textColor);

            y += message.size;

            if (y > RayWindow::GetScreenHeight())
            {
                break;
            }
        }
#endif
    }


} // namespace ClassicLauncher