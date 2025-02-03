#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Core.h"
#include "Utils/Timer.h"

namespace ClassicLauncher
{
    class Timer;
    class Entity;
    class Application;

    struct TimerHandling
    {
        friend class TimerManager;

    private:

        int id = -1;
    };

    class TimerManager
    {
        friend class Application;

    private:

        std::unordered_map<int, std::unique_ptr<Timer>> mTimers;
        void ValidTimerHandling(TimerHandling& timerHandling);
        void Update();

    public:

        TimerManager() = default;
        ~TimerManager() = default;
        void SetTimer(TimerHandling& timerHandling, std::function<void()> callbackFunction, Entity* targetEntity, float delay, bool bLooped = false);
        void ClearAllTimers();
    };

}  // namespace ClassicLauncher

#endif