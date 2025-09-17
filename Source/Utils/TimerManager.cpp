#include "TimerManager.h"

#include <utility>

namespace ClassicLauncher
{
    void TimerManager::ValidTimerHandling(TimerHandling& timerHandling) const
    {
        const int size = static_cast<int>(mTimers.size() - 1);
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
            timerHandling.id = static_cast<int>(mTimers.size());
            mTimers.insert(std::make_pair(timerHandling.id, std::move(newTimer)));
        }
        mTimers[timerHandling.id]->SetTimer(std::move(callbackFunction), targetEntity, delay, bLooped);
    }

    void TimerManager::ClearTimer(const TimerHandling& timerHandling)
    {
        if (timerHandling.id < 0)
        {
            return;
        }
        mTimers[timerHandling.id]->Stop();
    }

    void TimerManager::Update() const
    {
        for (auto& timer : mTimers)
        {
            timer.second->Update();
        }
    }

    void TimerManager::ClearAllTimers()
    {
        for (auto& timer : mTimers)
        {
            timer.second.reset();
        }
        mTimers.clear();
    }

} // namespace ClassicLauncher
