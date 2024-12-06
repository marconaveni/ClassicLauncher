#pragma once

#include "Math.h"
#include "RaylibCpp.h"
#include <memory>

namespace ClassicLauncher
{

struct Sprite
{
    Texture2D* texture;
    Rectangle source;
    bool bInvert;
    Sprite() = default;

    Sprite(Texture2D* texture, const Rectangle source)
        : texture(texture), source(source), bInvert(true)
    {
    }
};

struct Position
{
    Vector2 position;
    Vector2 scale;
    Colors color;

    Position()
        : position(Vector2{}), scale(Vector2{1, 1}), color(WHITE)
    {
    }

    explicit Position(const Vector2& position)
        : position(position), scale(Vector2{1, 1}), color(WHITE)
    {
    }

    Position(const Vector2 position, const Vector2 scale)
        : position(position), scale(scale), color(WHITE)
    {
    }

    Position(const Vector2 position, const Vector2 scale, const Color color)
        : position(position), scale(scale), color(color)
    {
    }
};

class ObjectManager;

class Object
{
    bool bRegisterObject;
    bool bToFrontRender;

  protected:
  public:
    Position position;

    // friend class ObjectManager;

    Object(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture);

    Object();

    /* 	Object(const Object&) = default;  	// Copy constructor

        Object(Object&& other) noexcept       // Move constructor
            : bRegisterObject(other.bRegisterObject),
            bToFrontRender(other.bToFrontRender),
            position(other.position),
            spriteResource(other.spriteResource)
        {
        }

        Object& operator=(const Object&) = default;  // Copy assignment operator

        Object& operator=(Object&& other) noexcept  // Move assignment operator
        {
            if (this != &other)
            {
                bRegisterObject = other.bRegisterObject;  // Move data from 'other' to 'this' object
                bToFrontRender = other.bToFrontRender;
                position = other.position;
                spriteResource = other.spriteResource;
            }
            return *this;
        } */

    virtual ~Object() = default;

    Sprite spriteResource;

    void SetTexture(Texture2D* textureReference);

    void SetTexture(Texture2D* textureReference, Rectangle rectangleTexture);

    void BringToFrontRender();

    virtual void BeginPlay();

    virtual void Tick();

    virtual void Draw();

    virtual void EndDraw();

    virtual void EndPlay();

    virtual void Collision();
};

} // namespace ClassicLauncher