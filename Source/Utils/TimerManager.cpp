#include "TimerManager.h"

#include <algorithm>
#include <utility>

#include "Helper.h"
#include "Window/Window.h"

namespace ClassicLauncher
{
    
    TimerManager::TimerManager(Window* window)
        : m_windowRef(window)
    {
    }

    void TimerManager::ValidTimerHandling(TimerHandling& timerHandling) const
    {
        const int size = static_cast<int>(m_timers.size() - 1);
        if (timerHandling.id < 0 || timerHandling.id > size)
        {
            timerHandling.id = -1;
        }
    }

    void TimerManager::SetTimer(TimerHandling& timerHandling, std::function<void()> callbackFunction, Entity* targetEntity, float delay, bool isLooped)
    {
        ValidTimerHandling(timerHandling);

        if (timerHandling.id < 0)
        {
            std::unique_ptr<Timer> newTimer = std::make_unique<Timer>(m_windowRef);
            timerHandling.id = static_cast<int>(m_timers.size());
            m_timers.insert(std::make_pair(&timerHandling, std::move(newTimer)));
        }
        m_timers[&timerHandling]->SetTimer(std::move(callbackFunction), targetEntity, delay, isLooped);
    }

    void TimerManager::ClearTimer(TimerHandling& timerHandling)
    {
        ValidTimerHandling(timerHandling);

        if (timerHandling.id < 0)
        {
            std::unique_ptr<Timer> newTimer = std::make_unique<Timer>(m_windowRef);
            timerHandling.id = static_cast<int>(m_timers.size());
            m_timers.insert(std::make_pair(&timerHandling, std::move(newTimer)));
        }
        m_timers[&timerHandling]->Stop();
    }

    void TimerManager::Update() const
    {
        for (auto& timer : m_timers)
        {
            timer.second->Update();
        }
        LOG(LogTrace, "num m_timers in TimerManager %d", m_timers.size());
    }

    void TimerManager::ClearAllTimers()
    {
        for (auto it = m_timers.begin(); it != m_timers.end();)
        {
            auto& timer = it->second;
            if (!timer->IsActive())
            {
                LOG(LogTrace, "m_timers[%d] deleted", it->first->id);
                it->first->id = -1;
                it = m_timers.erase(it); // erase return the next iterator
            }
            else
            {
                ++it;
            }
        }
    }

} // namespace ClassicLauncher
