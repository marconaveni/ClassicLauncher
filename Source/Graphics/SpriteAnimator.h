#ifndef VECTOR_2EX_H
#define VECTOR_2EX_H

#include <string>
#include <vector>

namespace ClassicLauncher
{

    class SpriteAnimator
    {
    public:

        SpriteAnimator(float timeAnimation, const std::vector<int>& spriteIndices)
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
                    mCurrentSpriteIndex = 0;  // Reinicia a animação (loop)
                }
            }
        }

        // Retorna o índice atual do sprite
        int GetCurrentSprite() const { return mSpriteIndices[mCurrentSpriteIndex]; }

    private:

        float mTimeAnimation;             // Duração total para avançar para o próximo sprite
        float mCurrentTime;               // Tempo decorrido total
        float mAlpha;                     // Fator interpolante entre 0 e 1
        std::vector<int> mSpriteIndices;  // Vetor com os índices ou IDs dos sprites
        size_t mCurrentSpriteIndex;       // Índice atual do sprite
    };

}  // namespace ClassicLauncher

#endif