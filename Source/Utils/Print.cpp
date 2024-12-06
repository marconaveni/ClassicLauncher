#include "Print.h"
#include "Math.h"
#include "UtilsFunctionLibrary.h"
#include <iostream>
#include <algorithm>


namespace ClassicLauncher
{
	Print::Print()
	: size(20)
	, spacing(1)
	{
#ifdef _DEBUG
		SetTraceLogLevel(LOG_ALL);
		levelLog = LOG_ALL;
#else
		SetTraceLogLevel(LOG_ERROR);
		levelLog = LOG_ERROR;
#endif
		SetTraceLogCallback(&Print::Log);
	}

    void Print::InternalPrintOnScreen(const std::string& text, float duration, const std::string& label, const Color& textColor, bool bLog, int sizeY)
    {
#ifdef _DEBUG

		if(!IsFontValid(fontTtf))
		{
			fontTtf = GetFontDefault();
		}

		bool bFound = false;
		for (Message& msg : messages)
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
			messages.emplace_back(message);
		}
#endif
    }	

	void Print::PrintOnScreen(const char* text, const float duration, const char* label, const Color& textColor, const bool bLog)
	{

    	int count = 0;
		std::string message = text;
		std::string splitMessage = "";
		std::string labelCompare = label;
		for (const char c : message)		
		{
			if (c == '\n') 
			{
				std::string labelCount;
				labelCount.append((!labelCompare.empty()) ? labelCompare : std::to_string(  Math::Random(1, 3000) ));
				labelCount.append(std::to_string(count));
				InternalPrintOnScreen(splitMessage, duration, labelCount.c_str(), textColor, bLog, size - 3);
				splitMessage = "";
				count++;
				continue;
			}
			splitMessage += c;
		}

		labelCompare = (labelCompare != "") ? labelCompare : std::to_string(Math::Random(1, 3000));
		InternalPrintOnScreen(splitMessage, duration, labelCompare, textColor, bLog, size);
	
	}

	void Print::Log(int messageType, const char* text, va_list args)
	{
#ifndef _DEBUG
		if(messageType < LOG_ERROR)
		{
			return;
		}
#endif
		switch (messageType)
		{
		case LOG_TRACE: std::cout << "\x1b[36m" << "TRACE: "; break;
		case LOG_DEBUG: std::cout << "\x1b[34m" << "DEBUG: "; break;
		case LOG_INFO: std::cout << "\x1b[37m" << "INFO: "; break;
		case LOG_WARNING: std::cout << "\x1B[33m" << "WARNING: "; break;
		case LOG_ERROR: std::cout << "\x1B[31m" << "ERROR: "; break;
		case LOG_FATAL: std::cout << "\x1B[41m" << "FATAL: "; break;
		}

		vprintf(text, args);
		std::cout << "\x1B[0m\n";
	}

    void Print::Log(TraceLogLevel logLevel, const char* text)
    {
		TraceLog(logLevel, text);
    }

    void Print::DrawMessage()
	{
#ifdef _DEBUG
		if(!IsFontValid(fontTtf))
		{
			return;
		}

    	messages.erase(
        std::remove_if(messages.begin(), messages.end(), 
                       [](Message& message) { return !message.IsTimeElapsed(); }),
        messages.end());

		float y = 16; 

		for (const auto& message : messages) 
		{
			const Vector2 positionRender = { 30, y };
			const Vector2 positionRenderShadow = { 31, y + 1 };
		
			DrawTextEx(fontTtf, message.textMessage.data(), positionRenderShadow, size, spacing, BLACK);
			DrawTextEx(fontTtf, message.textMessage.data(), positionRender, size, spacing, message.textColor);

			y += message.size; 

			if(y > GetScreenHeight())
			{
				break;
			}
		}
#endif
	}

    void Print::LoadFont(const std::string& path, int size, float spacing)
    {
#ifdef _DEBUG
		this->size = size;
		this->spacing = spacing;
		fontTtf = LoadFontEx(path.c_str(), size, nullptr, 250);
		if(!IsFontValid(fontTtf))
		{
			fontTtf = GetFontDefault();
		}
#endif
    }

    void Print::Unload()
    {
#ifdef _DEBUG
		UnloadFont(fontTtf);
#endif
    }



} // namespace ClassicLauncher