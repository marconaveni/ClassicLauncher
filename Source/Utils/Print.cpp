#include "Print.h"
#include <algorithm>
#include <iostream>
#include "Math.h"


namespace ClassicLauncher
{
    Print::Print()
        : mSize(20), mSpacing(1), mFont()
    {
    }

    void Print::InternalPrintOnScreen(const std::string& text, float duration, const std::string& label, const Color& textColor, bool bLog, int sizeY)
    {
#ifdef _DEBUG

        if (!IsFontValid(mFont))
        {
            mFont = GetFontDefault();
        }

        bool bFound = false;
        for (Message& msg : mMessages)
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
            mMessages.emplace_back(message);
        }
#endif
    }

    void Print::PrintOnScreen(const char* text, const float duration, const char* label, const Color& textColor, const bool bLog)
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
                InternalPrintOnScreen(splitMessage, duration, labelCount.c_str(), textColor, bLog, mSize - 3);
                splitMessage = "";
                count++;
                continue;
            }
            splitMessage += c;
        }

        labelCompare = (!labelCompare.empty()) ? labelCompare : std::to_string(Math::Random(1, 3000));
        InternalPrintOnScreen(splitMessage, duration, labelCompare, textColor, bLog, mSize);
#endif
    }

    void Print::DrawMessage()
    {
#ifdef _DEBUG
        if (!IsFontValid(mFont))
        {
            return;
        }

        mMessages.erase(std::remove_if(mMessages.begin(),
                                      mMessages.end(),
                                      [](Message& message)
                                      {
                                          return !message.IsTimeElapsed();
                                      }),
                       mMessages.end());

        float y = 16;

        for (const auto& message : mMessages)
        {
            const Vector2 positionRender = { 30, y };
            const Vector2 positionRenderShadow = { 31, y + 1 };

            DrawTextEx(mFont, message.textMessage.data(), positionRenderShadow, mSize, mSpacing, BLACK);
            DrawTextEx(mFont, message.textMessage.data(), positionRender, mSize, mSpacing, message.textColor);

            y += message.size;

            if (y > GetScreenHeight())
            {
                break;
            }
        }
#endif
    }

    void Print::LoadFont(const std::string& path, int size, float spacing)
    {
#ifdef _DEBUG
        this->mSize = size;
        this->mSpacing = spacing;
        mFont = LoadFontEx(path.c_str(), size, nullptr, 250);
        if (!IsFontValid(mFont))
        {
            mFont = GetFontDefault();
        }
#endif
    }

    void Print::Unload()
    {
#ifdef _DEBUG
        UnloadFont(mFont);
#endif
    }

}  // namespace ClassicLauncher