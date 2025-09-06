#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Data/Color.h"
#include "Data/Rectangle.h"

namespace ClassicLauncher
{

    class Transformation
    {
    public:

        Vector2f offset;
        float x;
        float y;
        float width;
        float height;
        float scaleX;
        float scaleY;
        float rotation;
        Color color;

        float sourceX;
        float sourceY;
        float scaleWidth;
        float scaleHeight;

        Transformation()
            : rootX(0)
            , rootY(0)
            , rootScaleX(1)
            , rootScaleY(1)
            , offset()
            , x(0)
            , y(0)
            , width(0)
            , height(0)
            , scaleX(1)
            , scaleY(1)
            , rotation(0)
            , color({ 255, 255, 255, 255 })
            , sourceX(0)
            , sourceY(0)
            , scaleWidth(0)
            , scaleHeight(0)
            , mScale{ 1.0f, 1.0f }
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

        RectFloat mTransform;
        RectFloat mSource;
        Vector2f mScale;

        float rootX;
        float rootY;
        float rootScaleX;
        float rootScaleY;
    };

    // class Transformation;

}  // namespace ClassicLauncher

#endif  // TRANSFORM_H