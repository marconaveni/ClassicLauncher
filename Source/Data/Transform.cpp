#include "Transform.h"

namespace ClassicLauncher
{
    
    void Transform::SetTransforms(const float m)
    {
        
        
        // float _rootX = rootX * m;
        // float _rootY = rootY * m;
        float _x = ((x + rootScaleX * offset.x) * m) + (rootX * m);
        float _y = ((y + rootScaleY * offset.y) * m) + (rootY * m);
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
        
        const Vector2 _scale = { (_scaleWidth * scaleX * rootScaleX), (_scaleHeight * scaleY * rootScaleY) };
        
        mSource = Rectangle(_sourceX, _sourceY, _width, _height);
        mTransform = Rectangle(_x, _y, _scale.x, _scale.y);
        
        
        
        
        // Transform newTransf = *this;
        // return newTransf;
        
       

        /*
       Transform newTransf = *this;
       newTransf.rootX = rootX * m;
       newTransf.rootY = rootY * m;
       newTransf.x = (x + rootScaleX * offset.x) * m;
       newTransf.y = (y + rootScaleY * offset.y) * m;
       newTransf.width = width * m;
       newTransf.height = height * m;
        
        newTransf.sourceX = sourceX * m;
        newTransf.sourceY = sourceY * m;
        newTransf.scaleWidth = scaleWidth * m;
        newTransf.scaleHeight = scaleHeight * m;
        
        const float _x = newTransf.rootX + newTransf.x;
        const float _y = newTransf.rootY + newTransf.y;
        const float _width = newTransf.width;
        const float _height = newTransf.height;
        const float _sourceX = newTransf.sourceX;
        const float _sourceY = newTransf.sourceY;
        const float _scaleWidth = newTransf.scaleWidth > 0.0f ? newTransf.scaleWidth : _width;
        const float _scaleHeight = newTransf.scaleHeight > 0.0f ? newTransf.scaleHeight : _height;
        
        const Vector2 _scale = { (_scaleWidth * newTransf.scaleX * newTransf.rootScaleX), (_scaleHeight * newTransf.scaleY * newTransf.rootScaleY) };
        
        mSource = Rectangle(_sourceX, _sourceY, _width, _height);
        mTransform = Rectangle(_x, _y, _scale.x, _scale.y);

        
        
        
        return newTransf;
        */
    }


    

}  // namespace ClassicLauncher
