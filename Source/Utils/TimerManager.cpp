#include "TimerManager.h"

namespace ClassicLauncher
{
    void TimerManager::ValidTimerHandling(TimerHandling& timerHandling)
    {
        const int size = static_cast<int>(mTimers.size() - 1);
        if (timerHandling.id < 0 || timerHandling.id > size)
        {
            timerHandling.id = -1;
            return;
        }
    }

    void TimerManager::SetTimer(TimerHandling& timerHandling, std::function<void()> callbackFunction, Entity* targetEntity, float delay, bool bLooped)
    {
        ValidTimerHandling(timerHandling);

        if (timerHandling.id < 0)
        {
            std::unique_ptr<Timer> newTimer = std::make_unique<Timer>();
            timerHandling.id = mTimers.size();
            mTimers.insert(std::make_pair(timerHandling.id, std::move(newTimer)));
        }
        mTimers[timerHandling.id]->SetTimer(callbackFunction, targetEntity, delay, bLooped);
    }

    void TimerManager::Update()
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

}  // namespace ClassicLauncher
