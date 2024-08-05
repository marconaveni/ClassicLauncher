#pragma once

#include "raylib.h"

#include <chrono>
#include <map>
#include <vector>


struct DelayAnimation
{
    float duration;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    bool bStart;

    void Reset()
    {
        bStart = false;
    }

    void SetStart()
    {
        if (!bStart)
        {
            bStart = true;
            start = std::chrono::steady_clock::now();
        }
    }

    void SetEnd()
    {
        end = std::chrono::steady_clock::now();
    }

    bool IsTimeElapsed()
    {
        SetEnd();
        const std::chrono::duration<float> elapsedTime = end - start;
        const float time = elapsedTime.count();
        return time < duration;
    }
};


class AnimationFrame
{
public:

    DelayAnimation delay;

    std::string currentAnimation;

    int frame = 0;

    std::map<std::string, std::vector<Vector2>> animations;

    void AddAnimation(const std::string& name, std::vector<Vector2> position);

    std::vector<Vector2> GetAnimations(const std::string& name);

    void UpdateAnimation();

    void AnimationChangeFrame();

    Vector2 GetCurrentFrame();
};