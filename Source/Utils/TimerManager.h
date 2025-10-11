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

        int id{-1};
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
        void ClearTimer(TimerHandling& timerHandling);
        void ClearAllTimers();

    private:

        friend class Application;

        void ValidTimerHandling(TimerHandling& timerHandling) const;
        void Update() const;
        
        std::unordered_map<TimerHandling*, std::unique_ptr<Timer>> m_timers{};
        int m_counter{0};
    };

} // namespace ClassicLauncher

#endif