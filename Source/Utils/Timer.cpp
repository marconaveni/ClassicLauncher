#include "Timer.h"

#include <utility>

#include "Entity/Entity.h"
#include "Window/RayWindow.h"

namespace ClassicLauncher
{
    Timer::Timer()
        : m_delay(0)
        , m_duration(0.0)
        , m_currentTime(0.0)
        , m_isFunctionCalled(false)
        , m_isLoop(false)
        , m_isActive(false)
        , m_targetEntity(nullptr)
        , m_callback(nullptr)
    {
    }

    void Timer::SetTimer(std::function<void()> callbackFunction,
                         Entity* targetEntity,
                         const float delay,
                         const bool bIsLoop)
    {
        m_callback = std::move(callbackFunction);
        m_targetEntity = targetEntity;
        m_delay = delay;
        m_isActive = true;
        m_isLoop = bIsLoop;
        Reset();
    }

    void Timer::Update()
    {
        if (!m_isFunctionCalled && m_isActive)
        {
            if (m_currentTime <= m_delay)
            {
                m_currentTime += RayWindow::GetFrameTime();
                return;
            }
            m_callback();

            if (m_isLoop)
            {
                Reset();
            }
            else
            {
                m_isFunctionCalled = true; // Mark the function as already called
                m_isActive = false;
            }
        }
    }

    void Timer::Reset()
    {
        m_currentTime = 0.0;                                                  // Reset current time
        m_duration = m_delay / static_cast<double>(RayWindow::GetFrameTime()); // Set duration based on frame time
        m_isFunctionCalled = false;                                           // Reset the function called state
    }

} // namespace ClassicLauncher
