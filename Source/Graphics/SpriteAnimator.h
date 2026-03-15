#ifndef SPRITE_ANIMATOR_H
#define SPRITE_ANIMATOR_H

#include <vector>

#include "Data/Rectangle.h"

namespace ClassicLauncher
{
    
    class SpriteAnimator
    {
    public:

        SpriteAnimator() = default;

        SpriteAnimator(const float timeAnimation, const std::vector<RectFloat>& spriteIndices)
            : m_timeAnimation(timeAnimation)
            , m_spriteIndices(spriteIndices)
        {
        }

        void Update(const float deltaTime)
        {
            if (m_spriteIndices.empty())
            {
                return;
            }

            m_alpha += (1.0f / m_timeAnimation) * deltaTime;
            m_currentTime += deltaTime;

            if (m_alpha >= 1.0f)
            {
                m_alpha = 0.0f;
                ++m_currentSpriteIndex;

                if (m_currentSpriteIndex >= m_spriteIndices.size())
                {
                    m_currentSpriteIndex = 0; // Restart Animation (loop)
                }
            }
        }

        // Returns the current sprite index.
        [[nodiscard]] RectFloat GetCurrentSprite() const { return m_spriteIndices[m_currentSpriteIndex]; }

    private:

        float m_timeAnimation{0.0f};              // Total duration to advance to the next sprite
        float m_currentTime{0.0f};                // Total elapsed time
        float m_alpha{0.0f};                      // Interpolating factor between 0 and 1
        std::vector<RectFloat> m_spriteIndices{}; // Vector with the indices or IDs of the sprites
        size_t m_currentSpriteIndex{0};           // Current sprite index
    };
} // namespace ClassicLauncher

#endif