#ifndef SPRITE_ANIMATOR_H
#define SPRITE_ANIMATOR_H

#include <string>
#include <vector>

namespace ClassicLauncher
{
    class SpriteAnimator
    {
    public:

        SpriteAnimator()
            : mTimeAnimation(0.0f), mCurrentTime(0.0f), mAlpha(0.0f), mSpriteIndices(), mCurrentSpriteIndex(0)
        {
        }

        SpriteAnimator(float timeAnimation, const std::vector<RectFloat>& spriteIndices)
            : mTimeAnimation(timeAnimation), mCurrentTime(0.0f), mAlpha(0.0f), mSpriteIndices(spriteIndices), mCurrentSpriteIndex(0)
        {
        }

        void Update(float deltaTime)
        {
            if (mSpriteIndices.empty()) return;

            mAlpha += (1.0f / mTimeAnimation) * deltaTime;
            mCurrentTime += deltaTime;

            if (mAlpha >= 1.0f)
            {
                mAlpha = 0.0f;
                ++mCurrentSpriteIndex;

                if (mCurrentSpriteIndex >= mSpriteIndices.size())
                {
                    mCurrentSpriteIndex = 0; // Restart Animation (loop)
                }
            }
        }

        // Retorna o índice atual do sprite
        RectFloat GetCurrentSprite() const { return mSpriteIndices[mCurrentSpriteIndex]; }

    private:

        float mTimeAnimation; // Total duration to advance to the next sprite
        float mCurrentTime; // Total elapsed time
        float mAlpha; // Interpolating factor between 0 and 1
        std::vector<RectFloat> mSpriteIndices; // Vector with the indices or IDs of the sprites
        size_t mCurrentSpriteIndex; // Current sprite index
    };
} // namespace ClassicLauncher

#endif