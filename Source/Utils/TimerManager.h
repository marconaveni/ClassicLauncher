#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <functional>
#include <memory>
#include <unordered_map>
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
    public:

        TimerManager() = default;
        ~TimerManager() = default;
        void SetTimer(TimerHandling& timerHandling,
                      std::function<void()> callbackFunction,
                      Entity* targetEntity,
                      float delay,
                      bool bLooped = false);
        void ClearTimer(const TimerHandling& timerHandling);
        void ClearAllTimers();

    private:

        friend class Application;

        std::unordered_map<int, std::unique_ptr<Timer>> mTimers;
        void ValidTimerHandling(TimerHandling& timerHandling);
        void Update();
    };

}  // namespace ClassicLauncher

#endif