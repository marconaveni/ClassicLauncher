#pragma once


#include <chrono>
#include <string>
#include <vector>
#include "RaylibCpp.h"



typedef enum {
    LOGINFO,           
    LOGWARNING,        
    LOGERROR         

} GameLog;


struct Message
{
    std::string textMessage;
    float duration;
    std::string label;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    Color textColor;

    void SetStart()
    {
        start = std::chrono::steady_clock::now();
    }

    void SetEnd()
    {
        end = std::chrono::steady_clock::now();
    }

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

    
    void PrintOnScreen(const char* text, float duration = 2.0f, const char* label = nullptr, Color textColor = BLUE, bool log = false);

    void Log(GameLog log, const char* text);

    void DrawMessage();

    static Print* GetInstance();


private:

    Print();

    Font fontTtf;

    std::vector<Message> messages;
   


};


#define LOG(category, text )  Print::GetInstance()->Log(category, text)

#define PRINT_STRING(...)  Print::GetInstance()->PrintOnScreen(__VA_ARGS__)

