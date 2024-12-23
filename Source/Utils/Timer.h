#ifndef TIMER_H
#define TIMER_H

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

        float mDelay;           // Tempo de atraso
        float mDuration;        // Tempo de duracao
        float mCurrentTime;     // Tempo atual
        bool bFunctionCalled;   // Controle de chamada da função
        bool bLoop;             // Indica se o timer é cíclico
        bool bActive;           // Indica se o timer está ativo
        T* mTargetEntity;       // Entidade alvo
        void (T::*callback)();  // Ponteiro para a função a ser chamada

    public:

        Timer()
            : mDelay(0), mDuration(0), mCurrentTime(0), bFunctionCalled(false), bLoop(false), bActive(false), callback(nullptr) {};

        void SetTimer(void (T::*callbackFunction)(), T* targetEntity, float delay, bool bLooped = false)
        {
            callback = callbackFunction;
            mTargetEntity = targetEntity;
            mDelay = delay;
            bActive = true;
            Reset();
        }

        void Update()
        {
            if (!bFunctionCalled && bActive)
            {
                if (mCurrentTime <= mDuration)
                {
                    mCurrentTime++;
                    return;
                }
                (mTargetEntity->*callback)();

                if (bLoop)
                {
                    Reset();
                }
                else
                {
                    bFunctionCalled = true;  // Marca como já chamada
                }
            }
        }

        void Reset()
        {
            mCurrentTime = 0.0f;                  // Reinicia o tempo atual
            mDuration = mDelay / GetFrameTime();  // Reinicia o tempo inicial
            bFunctionCalled = false;              // Reseta o estado de chamada
        }

        void Stop() { bFunctionCalled = true; }
    };

}  // namespace ClassicLauncher

#endif  // TIMER_H