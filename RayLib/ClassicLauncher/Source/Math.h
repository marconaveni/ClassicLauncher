#pragma once
#include <random>


class Math
{
public:
    template <typename T>
    static constexpr T Clamp(T value, T min, T max)
    {
        return Max(Min<T>(value, max), min);
    }

	static constexpr float Clamp(const float value, const float min, const float max) { return Clamp<float>(value, min, max); }
	static constexpr double Clamp(const double value, const double min, const double max) { return Clamp<double>(value, min, max); }

    //template <typename T, typename X>
    //static constexpr X Clamp(T value, T min, T max)
    //{
    //    return Max(Min<T>(value, max), min);
    //}

    //template <typename T, typename X>
    //static constexpr T Clamp(X value, T min, T max)
    //{
    //    return Max(Min<T>(value, max), min);
    //}

    //template <typename T, typename X>
    //static constexpr T Clamp(X value, X min, T max)
    //{
    //    return Max(Min<T>(value, max), min);
    //}

    //template <typename T, typename X>
    //static constexpr T Clamp(X value, X min, X max)
    //{
    //    return Max(Min<T>(value, max), min);
    //}

    //template <typename T, typename X>
    //static constexpr T Clamp(X value, T min, X max)
    //{
    //    return Max(Min<T>(value, max), min);
    //}

    //template <typename T, typename X>
    //static constexpr T Clamp(T value, X min, T max)
    //{
    //    return Max(Min<T>(value, max), min);
    //}

    //static constexpr float Clamp(const int value, const int min, const int max)
    //{
    //    return Max(Min<float>(static_cast<float>(value), static_cast<float>(max)), static_cast<float>(min));
    //}

    //static constexpr float Clamp(const float value, const int min, const int max)
    //{
    //    return Max(Min<float>(value, static_cast<float>(max)), static_cast<float>(min));
    //}

    //static constexpr float Clamp(const float value, const float min, const int max)
    //{
    //    return Max(Min<float>(value, static_cast<float>(max)), min);
    //}

    //static constexpr float Clamp(const float value, const int min, const float max)
    //{
    //    return Max(Min<float>(value, max), static_cast<float>(min));
    //}

    //static constexpr float Clamp(const int value, const float min, const int max)
    //{
    //    return Max(Min<float>(static_cast<float>(value), static_cast<float>(max)), min);
    //}

    //static constexpr float Clamp(const int value, const int min, const float max)
    //{
    //    return Max(Min<float>(static_cast<float>(value), max), static_cast<float>(min));
    //}

    template <typename T>
    static constexpr T Max(T a, T b)
    {
        return (b < a) ? a : b;
    }

    template <typename T>
    static constexpr T Min(T a, T b)
    {
        return (a < b) ? a : b;
    }


    static float Random(float min, float max)
    {
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<float> distr(min, max);
        return distr(eng);
    }


    static float GetAngle(float v1X, float v1Y, float v2X, float v2Y)
    {
        float deltaY = v2Y - v1Y;
        float deltaX = v2X - v1X;
        float angleInDegrees = atan2(deltaY, deltaX) * 180 / 3.14159265358979323846f;
        return angleInDegrees *= -1;
        //return angleInDegrees ;
    }

    static float GetAngle360(float v1X, float v1Y, float v2X, float v2Y)
    {
        float angleInDegrees = GetAngle(v1X, v1Y, v2X, v2Y);
        if (angleInDegrees < 0)
        {
            angleInDegrees = angleInDegrees + 360;
        }
        return angleInDegrees;
    }
};
