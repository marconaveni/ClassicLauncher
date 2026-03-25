#ifndef MATH_H
#define MATH_H

#include <algorithm> // std::clamp
#include <cmath>
#include <random>

#include "ClassicAssert.h"
#include "Data/Rectangle.h"
#include "Data/Vector2.h"

namespace ClassicLauncher::Math
{

    template <typename T>
    inline constexpr T Max(T a, T b)
    {
        return (b < a) ? a : b;
    }

    template <typename T>
    inline constexpr T Min(T a, T b)
    {
        return (a < b) ? a : b;
    }

    template <typename T, typename U, typename V>
    inline constexpr T Clamp(T value, U min, V max)
    {
        CLASSIC_ASSERT(std::is_signed_v<T> == std::is_signed_v<U> && std::is_signed_v<T> == std::is_signed_v<V>,
                       "Clamp arguments must all be of the same signedness to avoid errors.");

        return (value < min) ? min : (value > max) ? max : value;
    }


    template <typename T>
    inline constexpr bool CheckCollisionPointRec(Vector2<T> point, Rectangle<T> rec)
    {
        return ((point.x >= rec.x) && (point.x < (rec.x + rec.width)) && (point.y >= rec.y) && (point.y < (rec.y + rec.height)));
    }

    template <typename T>
    inline constexpr bool CheckCollisionRecs(Rectangle<T> rec1, Rectangle<T> rec2)
    {
        return ((rec1.x < (rec2.x + rec2.width) && (rec1.x + rec1.width) > rec2.x) && (rec1.y < (rec2.y + rec2.height) && (rec1.y + rec1.height) > rec2.y));
    }

    inline constexpr int ToInt(float value)
    {
        return static_cast<int>(value);
    }

    inline constexpr int ToIntRound(float value)
    {
        return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f);
    }

    inline constexpr float BytesToTerabytes(int value)
    {
        return value / (1024.0f * 1024.0f * 1024.0f * 1024.0f);
    }

    inline constexpr float BytesToGigabytes(int value)
    {
        return value / (1024.0f * 1024.0f * 1024.0f);
    }

    inline constexpr float BytesToMegabytes(int value)
    {
        return value / (1024.0f * 1024.0f);
    }

    template <typename T>
    static Vector2<T> VecClamp(Vector2<T> value, Vector2<T> min, Vector2<T> max)
    {
        Vector2<T> result{};
        result.x = std::clamp(value.x, min.x, max.x);
        result.y = std::clamp(value.y, min.y, max.y);
        return result;
    }

    static float FloatAbs(float value)
    {
        return std::fabs(value);
    }

    static int IntAbs(int value)
    {
        return std::abs(value);
    }

    class RandomGenerator
    {
    public:

        RandomGenerator()
            : eng(rd())
        {
        }

        double Generate(double min, double max)
        {
            std::uniform_real_distribution distribution(min, max);
            return distribution(eng);
        }

    private:

        std::random_device rd;
        std::default_random_engine eng;
    };

    static double Random(const double min, const double max)
    {
        static RandomGenerator generator;
        return generator.Generate(min, max);
    }

    static float GetAngle(float v1X, float v1Y, float v2X, float v2Y)
    {
        float deltaY = v2Y - v1Y;
        float deltaX = v2X - v1X;
        float angleInDegrees = atan2(deltaY, deltaX) * 180 / 3.14159265358979323846f;
        return -angleInDegrees;
        // return angleInDegrees ;
    }

    static float GetAngle360(float v1X, float v1Y, float v2X, float v2Y)
    {
        float angleInDegrees = GetAngle(v1X, v1Y, v2X, v2Y);
        return (angleInDegrees < 0) ? (angleInDegrees + 360) : angleInDegrees;
    }

} // namespace ClassicLauncher::Math

#endif // MATH