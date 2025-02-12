#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Core.h"

namespace ClassicLauncher
{
    class Transform
    {

        friend class RenderSystem;

        Rectangle mTransform;
        Rectangle mSource;

    public:

        float rootX;
        float rootY;
        float rootScaleX;
        float rootScaleY;
        Vector2 offset;
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

        Transform()
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
            ,  sourceX(0)
            , sourceY(0)
            , scaleWidth(0)
            , scaleHeight(0)
        {
        }

        Rectangle GetTransform() { return mTransform; }
        Rectangle GetSource() { return mSource; }

        void SetTransforms(const float m);

    };

}  // namespace ClassicLauncher

#endif  // TRANSFORM_H