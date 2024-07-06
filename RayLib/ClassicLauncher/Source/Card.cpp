#include "Card.h"

Card::Card(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture)
	:Object(textureReference, position,  rectangleTexture)
{
}

Card::~Card()
{
}

void Card::BeginPlay()
{
    Object::BeginPlay();
}

void Card::Tick()
{
    Object::Tick();
}

void Card::Draw()
{
    Object::Draw();
}

void Card::EndDraw()
{
    Object::EndDraw();
}

void Card::EndPlay()
{
    Object::EndPlay();
}

void Card::Collision()
{
    Object::Collision();
}
