#include "Timer.h"

#include <utility>
#include "Entity/Entity.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{
    Timer::Timer()
        : mDelay(0)
        , mDuration(0.0)
        , mCurrentTime(0.0)
        , mIsFunctionCalled(false)
        , mIsLoop(false)
        , mIsActive(false)
        , mTargetEntity(nullptr)
        , mCallback(nullptr)
    {
    }

    void Timer::SetTimer(std::function<void()> callbackFunction, Entity* targetEntity, const float delay, const bool bIsLoop)
    {
        mCallback = std::move(callbackFunction);
        mTargetEntity = targetEntity;
        mDelay = delay;
        mIsActive = true;
        mIsLoop = bIsLoop;
        Reset();
    }

    void Timer::Update()
    {
        if (!mIsFunctionCalled && mIsActive)
        {
            if (mCurrentTime <= mDelay)
            {
                mCurrentTime += rlw::GetFrameTime();
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

    void Timer::Reset()
    {
        mCurrentTime = 0.0;                                // Reset current time
        mDuration = mDelay / (double)rlw::GetFrameTime();  // Set duration based on frame time
        mIsFunctionCalled = false;                         // Reset the function called state
    }

}  // namespace ClassicLauncher
