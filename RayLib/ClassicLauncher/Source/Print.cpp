#include "Print.h"
#include "Math.h"
#include <iostream>



Print::Print()
{
#ifdef _DEBUG
	fontTtf = LoadFontEx("Resources/fonts/roboto.ttf", 16, nullptr, 250);
#endif
}

Print* Print::GetInstance()
{
	static Print object;
	return &object;
}

void Print::PrintOnScreen(const char* text, const float duration, const char* label, const Color textColor, const bool log)
{
#ifdef _DEBUG

	bool found = false;

	std::string labelCompare;

	if(label != nullptr)
	{
		labelCompare = label;
	}

	if (!labelCompare.empty())
	{

		for (Message& msg : messages)
		{
			if (labelCompare == msg.label)
			{
				msg.SetStart();
				msg.textMessage = text;
				msg.textColor = textColor;
				if (log)
					std::cout << " [LOGSCREEN] : " << msg.textMessage << "\n";
				found = true;
				break;
			}
		}
	}
	else
	{
		labelCompare = std::to_string(Math::Random(1, 3000));
	}

	if (!found)
	{
		Message message;
		message.SetStart();
		message.duration = duration;
		message.textMessage = text;
		message.label = labelCompare;
		message.textColor = textColor;
		messages.push_back(message);
	}

#endif
}

void Print::Log(const GameLog log, const char* text)
{
#ifdef _DEBUG

	switch (log)
	{
	case LOGINFO:
		std::cout << "[INFO]: ";
		break;
	case LOGWARNING:
		std::cout << "\x1B[33m" << "[WARNING]: ";
		break;
	case LOGERROR:
		std::cout << "\x1B[31m" << "[ERROR]: ";
		break;
	}
	std::cout << text << "\x1B[0m\n";

#endif
}

void Print::DrawMessage()
{

#ifdef _DEBUG
	for (int i = 0; i < static_cast<int>(messages.size()); ++i)
	{
		if (messages[i].IsTimeElapsed())
		{
			const float positionY = 18.0f * (i + 1.0f);
			const Vector2 position = { 30, positionY };
			DrawTextEx(fontTtf, messages[i].textMessage.data(), position, 16, 0, messages[i].textColor);
		}
		else
		{
			//DrawText("Acabou tempo", 20, 30 * (i + 1), 20, WHITE);
		}
	}

	std::vector<Message> tempMessages;
	for (Message& message : messages)
	{
		if (message.IsTimeElapsed())
		{
			tempMessages.push_back(message);
		}
	}
	if (!tempMessages.empty())
	{
		messages = tempMessages;
	}

#endif
}
