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
        void Stop() { m_isActive = false; }
        [[nodiscard]] bool IsActive() const { return m_isActive; }

    private:

        double m_delay;                   // Delay time before the timer triggers
        double m_duration;                // Duration of the timer
        double m_currentTime;             // Current elapsed time
        bool m_isFunctionCalled;          // Flag to check if the function has been called
        bool m_isLoop;                    // Flag to determine if the timer is looping
        bool m_isActive;                  // Flag to check if the timer is active
        Entity* m_targetEntity;           // Pointer to the target entity
        std::function<void()> m_callback; // Pointer to the callback function to be called
    };

} // namespace ClassicLauncher

#define CALLFUNCTION(functionName, object) \
    [object]()                             \
    {                                      \
        object->functionName();            \
    }

#endif // TIMER_H