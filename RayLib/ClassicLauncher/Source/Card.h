#pragma once
#include "Object.h"

class Card : public Object
{
public:
    Card(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture);
    ~Card() override;
    void BeginPlay() override;
    void Tick() override;
    void Draw() override;
    void EndDraw() override;
    void EndPlay() override;
    void Collision() override;
};