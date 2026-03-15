#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Data/Color.h"
#include "Data/Rectangle.h"

namespace ClassicLauncher
{
    // Represents the local transformation of an entity.
    // The final world transform is computed by combining this transform
    // with the transform of its parent entity.
    struct Transform
    {
        RectFloat position{};            // Local position and size relative to the parent entity.
        RectFloat source{};              // Texture region used for rendering (UV/source rectangle).
        
        Vector2f offset{};               // Additional positional offset applied after the base position.
        Vector2f origin{};               // Pivot point used for rotation and scaling.
        
        Vector2f scale{1.0f, 1.0f};      // Local scaling factor relative to the parent transform.
        float rotation{};                // Local rotation angle relative to the parent.
        
        Color color{255, 255, 255, 255}; // Tint color and opacity applied to the entity.
    };


} // namespace ClassicLauncher

#endif // TRANSFORM_H