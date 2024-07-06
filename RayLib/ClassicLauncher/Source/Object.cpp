#include "Object.h"
#include <cmath>


Object::Object(Texture2D* textureReference, const Vector2 position, const Rectangle rectangleTexture)
	: bRegisterObject(false)
	, color(WHITE)
	, id(-1)
	, position(position)
	, spriteResource(textureReference, rectangleTexture)
	, scale(0)
{
}

Object::Object()
	: bRegisterObject(false)
	, color(WHITE)
	, id(-1)
	, position(Vector2{ 0, 0 })
	, spriteResource(Sprite{})
	, scale(0)
{
}


void Object::Draw()
{
	const Vector2 positionRender = { round(position.x), round(position.y) };
	Rectangle destination = { 0,0,0,0 };
	destination.x = positionRender.x - scale / 2;
	destination.y = positionRender.y - scale / 2;
	destination.width = spriteResource.source.width + scale;
	destination.height = spriteResource.source.height + scale;
	DrawTexturePro(*spriteResource.texture, spriteResource.source, destination, Vector2{}, 0, color); //draw texture
	//DrawTextureRec(*spriteResource.texture, spriteResource.source, positionRender, color); //draw texture
}

void Object::SetColor(const unsigned char r, const unsigned char g, const unsigned char b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void Object::SetOpacity(const unsigned char opacity)
{
	color.a = opacity;
}

void Object::SetTexture(Texture2D* textureReference)
{
	spriteResource.texture = textureReference;
}

void Object::SetTexture(Texture2D* textureReference, const Rectangle rectangleTexture)
{
	spriteResource.source = rectangleTexture;
	spriteResource.texture = textureReference;
}


void Object::Tick()
{
}
