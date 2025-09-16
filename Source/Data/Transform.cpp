#include "Transform.h"

namespace ClassicLauncher
{

    void Transform::SetTransforms(const float m)
    {

        float x = ((position.x + root.scale.x * offset.x) * m) + (root.position.x * m);
        float y = ((position.y + root.scale.y * offset.y) * m) + (root.position.y * m);
        float width = position.width * m;
        float height = position.height * m;

        float sourceX = source.x * m;
        float sourceY = source.y * m;
        float sourceWidth = source.width * m;
        float sourceHeight = source.height * m;

        sourceWidth = sourceWidth > 0.0f ? sourceWidth : width;
        sourceHeight = sourceHeight > 0.0f ? sourceHeight : height;

        mScale = Vector2f{scale.x * root.scale.x, scale.y * root.scale.y};
        mSource = RectFloat{Vector2f{sourceX, sourceY}, Vector2f{width, height}};
        mTransform = RectFloat{x, y, mScale.x * sourceWidth, mScale.y * sourceHeight};
    }

    void Transform::UpdateTransform(Transform& otherTransform)
    {
        otherTransform.root.position.x = position.x + root.position.x;
        otherTransform.root.position.y = position.y + root.position.y;
        otherTransform.root.scale.x = scale.x * root.scale.x;
        otherTransform.root.scale.y = scale.y * root.scale.y;
        otherTransform.color.a = color.a <= otherTransform.color.a ? color.a : otherTransform.color.a;
    }

}  // namespace ClassicLauncher
