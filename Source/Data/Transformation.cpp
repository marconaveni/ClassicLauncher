#include "Transformation.h"

namespace ClassicLauncher
{

    void Transformation::SetTransforms(const float m)
    {
        // float _rootX = rootX * m;
        // float _rootY = rootY * m;
        float _x = ((position.x + rootScaleX * offset.x) * m) + (rootX * m);
        float _y = ((position.y + rootScaleY * offset.y) * m) + (rootY * m);
        float _width = width * m;
        float _height = height * m;

        float _sourceX = sourceX * m;
        float _sourceY = sourceY * m;
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

        mScale = {scaleX * rootScaleX, scaleY * rootScaleY};
        mSource = {{_sourceX, _sourceY}, {_width, _height}};
        mTransform = {_x, _y, mScale.x * _scaleWidth, mScale.y * _scaleHeight};
    }

    void Transformation::UpdateTransform(Transformation& otherTransform)
    {
        otherTransform.rootX = position.x + rootX;
        otherTransform.rootY = position.y + rootY;
        otherTransform.rootScaleX = scaleX * rootScaleX;
        otherTransform.rootScaleY = scaleY * rootScaleY;
        otherTransform.color.a = color.a <= otherTransform.color.a ? color.a : otherTransform.color.a;
    }

}  // namespace ClassicLauncher
