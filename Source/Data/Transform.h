#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Data/Color.h"
#include "Data/Rectangle.h"

namespace ClassicLauncher
{

    class Transform
    {
    public:

        Vector2f offset{};
        RectFloat position{};
        Vector2f scale{1.0f, 1.0f};
        float rotation{};
        Color color{255, 255, 255, 255};
        RectFloat source{};

        struct Root
        {
            Vector2f position{};
            Vector2f scale{1.0f, 1.0f};
        };

        Root root;

        RectFloat GetTransform() { return mTransform; }
        RectFloat GetSource() { return mSource; }
        Vector2f GetScale() { return mScale; }

        void UpdateTransform(Transform& otherTransform);

    private:

        friend class RenderEntities;

        void SetTransforms(const float m);

        RectFloat mTransform{};
        RectFloat mSource{};
        Vector2f mScale{1.0f, 1.0f};
    };


} // namespace ClassicLauncher

#endif // TRANSFORM_H