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
        bool mIsFunctionCalled;            // Flag to check if the function has been called
        bool mIsLoop;                      // Flag to determine if the timer is looping
        bool mIsActive;                    // Flag to check if the timer is active
        T* mTargetEntity;                // Pointer to the target entity
        std::function<void()> mCallback;  // Pointer to the callback function to be called

    public:

        Timer()
            : mDelay(0), mDuration(0), mCurrentTime(0), mIsFunctionCalled(false), mIsLoop(false), mIsActive(false), mCallback(nullptr) {};

        void SetTimer(std::function<void()> callbackFunction, T* targetEntity, float delay, bool bLooped = false)
        {
            mCallback = callbackFunction;
            mTargetEntity = targetEntity;
            mDelay = delay;
            mIsActive = true;
            Reset();
        }

    private:

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

    public:

        void Reset()
        {
            mCurrentTime = 0.0f;                  // Reset current time
            mDuration = mDelay / GetFrameTime();  // Set duration based on frame time
            mIsFunctionCalled = false;              // Reset the function called state
        }

        void Stop() { mIsFunctionCalled = true; }
    };

}  // namespace ClassicLauncher

#define CALLFUNCTION(functionName, object) [object]() { object->functionName(); }


#endif  // TIMER_H