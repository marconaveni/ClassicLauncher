#pragma once

#include "raylib.h"


struct Sprite
{
    Texture2D* texture;
    Rectangle source;
    bool bInvert;
    Sprite() = default;

    Sprite(Texture2D* texture, const Rectangle source)
        : texture(texture)
          , source(source)
          , bInvert(true)
    {
    }
};


class Object
{
    bool bRegisterObject;

protected:

    Color color;

public:
    friend class ObjectManager;

    Object(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture);
    Object();
    virtual ~Object() = default;

    int id;
    Vector2 position;
    Sprite spriteResource;
    float scale;

    void SetTexture(Texture2D* textureReference);

    void SetTexture(Texture2D* textureReference, Rectangle rectangleTexture);

    virtual void BeginPlay() {}

    virtual void Tick();

    virtual void Draw();

    virtual void EndDraw() {}

    virtual void EndPlay() {}

    virtual void Collision() {}

    void SetColor(unsigned char r, unsigned char g, unsigned char b);

    void SetOpacity(unsigned char opacity);
};
