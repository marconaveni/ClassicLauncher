#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include "Entity/Entity.h"

namespace ClassicLauncher
{
    class Entity;


    class TimerBase
    {
    public:

        TimerBase() = default;
        virtual ~TimerBase() = default;
        virtual void Update() = 0;
    };

    template <typename T>
    class Timer : public TimerBase
    {
    private:

        float mDelay;                    // Delay time before the timer triggers
        float mDuration;                 // Duration of the timer
        float mCurrentTime;              // Current elapsed time
        bool bFunctionCalled;            // Flag to check if the function has been called
        bool bLoop;                      // Flag to determine if the timer is looping
        bool bActive;                    // Flag to check if the timer is active
        T* mTargetEntity;                // Pointer to the target entity
        std::function<void()> mCallback;  // Pointer to the callback function to be called

    public:

        Timer()
            : mDelay(0), mDuration(0), mCurrentTime(0), bFunctionCalled(false), bLoop(false), bActive(false), mCallback(nullptr) {};

        void SetTimer(std::function<void()> callbackFunction, T* targetEntity, float delay, bool bLooped = false)
        {
            mCallback = callbackFunction;
            mTargetEntity = targetEntity;
            mDelay = delay;
            bActive = true;
            Reset();
        }

    private:

        void Update()
        {
            if (!bFunctionCalled && bActive)
            {
                if (mCurrentTime <= mDuration)
                {
                    mCurrentTime++;
                    return;
                }
                mCallback();

                if (bLoop)
                {
                    Reset();
                }
                else
                {
                    bFunctionCalled = true;  // Mark the function as already called
                }
            }
        }

    public:

        void Reset()
        {
            mCurrentTime = 0.0f;                  // Reset current time
            mDuration = mDelay / GetFrameTime();  // Set duration based on frame time
            bFunctionCalled = false;              // Reset the function called state
        }

        void Stop() { bFunctionCalled = true; }
    };

}  // namespace ClassicLauncher

#define CALLFUNCTION(functionName, object) [object]() { object->functionName(); }


#endif  // TIMER_H