#include "Transformation.h"

namespace ClassicLauncher
{

    void Transformation::SetTransforms(const float m)
    {
        // float _rootX = rootX * m;
        // float _rootY = rootY * m;
        float _x = ((position.x + rootScaleX * offset.x) * m) + (rootX * m);
        float _y = ((position.y + rootScaleY * offset.y) * m) + (rootY * m);
        float _width = position.width * m;
        float _height = position.height * m;

        float _sourceX = source.x * m;
        float _sourceY = source.y * m;
        float _scaleWidth = scaleWidth * m;
        float _scaleHeight = scaleHeight * m;

        // _x = _rootX + _x;
        // _y = _rootY + _y;
        // _width = _width;
        // _height = _height;
        // _sourceX = _sourceX;
        // _sourceY = _sourceY;
        _scaleWidth = _scaleWidth > 0.0f ? _scaleWidth : _width;
        _scaleHeight = _scaleHeight > 0.0f ? _scaleHeight : _height;

        mScale = {scale.x * rootScaleX, scale.y * rootScaleY};
        mSource = {{_sourceX, _sourceY}, {_width, _height}};
        mTransform = {_x, _y, mScale.x * _scaleWidth, mScale.y * _scaleHeight};
    }

    void Transformation::UpdateTransform(Transformation& otherTransform)
    {
        otherTransform.rootX = position.x + rootX;
        otherTransform.rootY = position.y + rootY;
        otherTransform.rootScaleX = scale.x * rootScaleX;
        otherTransform.rootScaleY = scale.y * rootScaleY;
        otherTransform.color.a = color.a <= otherTransform.color.a ? color.a : otherTransform.color.a;
    }

}  // namespace ClassicLauncher
