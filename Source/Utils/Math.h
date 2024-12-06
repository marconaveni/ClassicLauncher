#ifndef MATH_H
#define MATH_H


#include <random>


namespace ClassicLauncher
{

    namespace Math
    {

        template <typename T>
        static constexpr T Max(T a, T b) { return (b < a) ? a : b; }

        template <typename T>
        static constexpr T Min(T a, T b) { return (a < b) ? a : b; }

        template <typename T>
        static constexpr T Clamp(T value, T min, T max) { return Max(Min<T>(value, max), min); }

        static constexpr float Clamp(const float value, const float min, const float max) { return Clamp<float>(value, min, max); }
        
        static constexpr double Clamp(const double value, const double min, const double max) { return Clamp<double>(value, min, max); }

        class RandomGenerator 
        {
        public:
            RandomGenerator() 
                : eng(rd()) {}

            double Generate(double min, double max) {
                std::uniform_real_distribution distribution(min, max);
                return distribution(eng);
            }

        private:
            std::random_device rd;
            std::default_random_engine eng;
        };

        
        static double Random(double min, double max) {
            static RandomGenerator generator;
            return generator.Generate(min, max);
        }


        static float GetAngle(float v1X, float v1Y, float v2X, float v2Y)
        {
            float deltaY = v2Y - v1Y;
            float deltaX = v2X - v1X;
            float angleInDegrees = atan2(deltaY, deltaX) * 180 / 3.14159265358979323846f;
            return -angleInDegrees; 
            //return angleInDegrees ;
        }

        static float GetAngle360(float v1X, float v1Y, float v2X, float v2Y)
        {
            float angleInDegrees = GetAngle(v1X, v1Y, v2X, v2Y);
            return (angleInDegrees < 0) ? (angleInDegrees + 360) : angleInDegrees;
        }
    } //Math

} // ClassicLauncher


#endif //MATH