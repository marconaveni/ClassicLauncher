#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include "Entity/Entity.h"

namespace ClassicLauncher
{
    class Entity;

    struct TimerHandling
    {
        friend class EntityManager;

    private:

        int id = -1;
    };

    class Timer
    {
    private:

        float mDelay;                     // Delay time before the timer triggers
        float mDuration;                  // Duration of the timer
        float mCurrentTime;               // Current elapsed time
        bool mIsFunctionCalled;           // Flag to check if the function has been called
        bool mIsLoop;                     // Flag to determine if the timer is looping
        bool mIsActive;                   // Flag to check if the timer is active
        Entity* mTargetEntity;            // Pointer to the target entity
        std::function<void()> mCallback;  // Pointer to the callback function to be called

    public:

        Timer()
            : mDelay(0), mDuration(0), mCurrentTime(0), mIsFunctionCalled(false), mIsLoop(false), mIsActive(false), mCallback(nullptr) {};

        ~Timer() = default;

        void SetTimer(std::function<void()> callbackFunction, Entity* targetEntity, float delay, bool bIsLoop = false)
        {
            mCallback = callbackFunction;
            mTargetEntity = targetEntity;
            mDelay = delay;
            mIsActive = true;
            mIsLoop = bIsLoop;
            Reset();
        }

        void Update()
        {
            if (!mIsFunctionCalled && mIsActive)
            {
                if (mCurrentTime <= mDuration)
                {
                    mCurrentTime++;
                    return;
                }
                mCallback();

                if (mIsLoop)
                {
                    Reset();
                }
                else
                {
                    mIsFunctionCalled = true;  // Mark the function as already called
                }
            }
        }

        void Reset()
        {
            mCurrentTime = 0.0f;                  // Reset current time
            mDuration = mDelay / GetFrameTime();  // Set duration based on frame time
            mIsFunctionCalled = false;            // Reset the function called state
        }

        void Stop() { mIsFunctionCalled = true; }
    };

}  // namespace ClassicLauncher

#define CALLFUNCTION(functionName, object) \
    [object]()                             \
    {                                      \
        object->functionName();            \
    }

#endif  // TIMER_H