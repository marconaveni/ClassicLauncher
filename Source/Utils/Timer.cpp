#include "Timer.h"

namespace ClassicLauncher
{

    // // template <typename T>
    // // Timer<T>::Timer()
    // // {
    // // }

    // Timer::Timer()
    //     : mDuration(0), mDelay(0), mCurrentTime(0), bFunctionCalled(false), bLoop(false), callback(nullptr)
    // {
    // }

    // void Timer::SetTimer(void (*callbackFunction)(), float duration, bool bLooped)
    // {
    //     callback = callbackFunction;
    //     //mDelay = delay;
    //     Reset();
    // }


    // void Timer::Update()
    // {
    //     if (!bFunctionCalled)
    //     {
    //         if (mCurrentTime <= mDuration)
    //         {
    //             mCurrentTime++;
    //             return;
    //         }

    //         callback();  // Chama a função membro

    //         if (bLoop)
    //         {
    //             Reset();
    //         }
    //         else
    //         {
    //             bFunctionCalled = true;  // Marca como já chamada
    //         }
    //     }
    // }

    // void Timer::Reset()
    // {
    //     mCurrentTime = 0.0f;                  // Reinicia o tempo atual
    //     mDuration = mDelay / GetFrameTime();  // Reinicia o tempo inicial
    //     bFunctionCalled = false;              // Reseta o estado de chamada
    // }

}  // namespace ClassicLauncher
