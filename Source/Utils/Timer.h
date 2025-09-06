#ifndef TIMER_H
#define TIMER_H

#include <functional>

namespace ClassicLauncher
{
    class Entity;

    class Timer
    {
    public:

        Timer();
        ~Timer() = default;
        void SetTimer(std::function<void()> callbackFunction, Entity* targetEntity, float delay, bool bIsLoop = false);
        void Update();
        void Reset();
        void Stop() { mIsActive = false; }

    private:

        double mDelay;                    // Delay time before the timer triggers
        double mDuration;                 // Duration of the timer
        double mCurrentTime;              // Current elapsed time
        bool mIsFunctionCalled;           // Flag to check if the function has been called
        bool mIsLoop;                     // Flag to determine if the timer is looping
        bool mIsActive;                   // Flag to check if the timer is active
        Entity* mTargetEntity;            // Pointer to the target entity
        std::function<void()> mCallback;  // Pointer to the callback function to be called
    };

}  // namespace ClassicLauncher

#define CALLFUNCTION(functionName, object) \
    [object]()                             \
    {                                      \
        object->functionName();            \
    }

#endif  // TIMER_H