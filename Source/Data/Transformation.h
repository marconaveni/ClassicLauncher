#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Data/Color.h"
#include "Data/Rectangle.h"

namespace ClassicLauncher
{

    class Transformation
    {
    public:

        Vector2f offset{};
        RectFloat position{};
        Vector2f scale{ 1.0f, 1.0f };
        float rotation{};
        Color color{ 255, 255, 255, 255 };
        RectFloat source{};

        Transformation()
            : rootX(0)
            , rootY(0)
            , rootScaleX(1)
            , rootScaleY(1)
        {
        }

        RectFloat GetTransform() { return mTransform; }
        RectFloat GetSource() { return mSource; }
        Vector2f GetScale() { return mScale; }
        Vector2f GetRootPosition() { return Vector2f(rootX, rootY); }
        Vector2f GetRootScale() { return Vector2f(rootScaleX, rootScaleY); }
        void UpdateTransform(Transformation& otherTransform);

    private:

        friend class RenderSystem;
        
        void SetTransforms(const float m);

        RectFloat mTransform{};
        RectFloat mSource{};
        Vector2f mScale{ 1.0f, 1.0f };

        float rootX;
        float rootY;
        float rootScaleX;
        float rootScaleY;
    };

    // class Transformation;

}  // namespace ClassicLauncher

#endif  // TRANSFORM_H