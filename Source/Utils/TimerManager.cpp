#include "TimerManager.h"

#include <utility>

namespace ClassicLauncher
{
    void TimerManager::ValidTimerHandling(TimerHandling& timerHandling) const
    {
        const int size = static_cast<int>(m_timers.size() - 1);
        if (timerHandling.id < 0 || timerHandling.id > size)
        {
            timerHandling.id = -1;
        }
    }

    void TimerManager::SetTimer(TimerHandling& timerHandling,
                                std::function<void()> callbackFunction,
                                Entity* targetEntity,
                                float delay,
                                bool bLooped)
    {
        ValidTimerHandling(timerHandling);

        if (timerHandling.id < 0)
        {
            std::unique_ptr<Timer> newTimer = std::make_unique<Timer>();
            timerHandling.id = static_cast<int>(m_timers.size());
            m_timers.insert(std::make_pair(timerHandling.id, std::move(newTimer)));
        }
        m_timers[timerHandling.id]->SetTimer(std::move(callbackFunction), targetEntity, delay, bLooped);
    }

    void TimerManager::ClearTimer(TimerHandling& timerHandling)
    {
        ValidTimerHandling(timerHandling);
        
        if (timerHandling.id < 0)
        {
            std::unique_ptr<Timer> newTimer = std::make_unique<Timer>();
            timerHandling.id = static_cast<int>(m_timers.size());
            m_timers.insert(std::make_pair(timerHandling.id, std::move(newTimer)));
        }
        m_timers[timerHandling.id]->Stop();
    }

    void TimerManager::Update() const
    {
        for (auto& timer : m_timers)
        {
            timer.second->Update();
        }
    }

    void TimerManager::ClearAllTimers()
    {
        for (auto& timer : m_timers)
        {
            timer.second.reset();
        }
        m_timers.clear();
    }

} // namespace ClassicLauncher
