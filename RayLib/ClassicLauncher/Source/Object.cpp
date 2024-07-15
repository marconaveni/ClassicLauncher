#include "Object.h"
#include <cmath>


Object::Object(Texture2D* textureReference, const Vector2 position, const Rectangle rectangleTexture)
	: bRegisterObject(false)
	, id(-1)
	, spriteResource(textureReference, rectangleTexture)
{
}

Object::Object()
	: bRegisterObject(false)
	, id(-1)
	, spriteResource(Sprite{})
{
}

Object::~Object() = default;


void Object::Draw()
{
	const Vector2 positionRender = { round(position.position.x), round(position.position.y) };
	Rectangle destination = { 0,0,0,0 };
	destination.x = positionRender.x + (spriteResource.source.width / 2 - spriteResource.source.width * position.scale.x / 2) ;
	destination.y = positionRender.y + (spriteResource.source.height / 2 - spriteResource.source.height * position.scale.y / 2);
	destination.width = spriteResource.source.width * position.scale.x;
	destination.height = spriteResource.source.height * position.scale.y;
	DrawTexturePro(*spriteResource.texture, spriteResource.source, destination, Vector2{}, 0, position.color); //draw texture
	//DrawTextureRec(*spriteResource.texture, spriteResource.source, positionRender, color); //draw texture
}

void Object::SetColor(const unsigned char r, const unsigned char g, const unsigned char b)
{
	position.color.r = r;
	position.color.g = g;
	position.color.b = b;
}

void Object::SetOpacity(const unsigned char opacity)
{
	position.color.a = opacity;
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

void Object::BeginPlay()
{
}

void Object::EndDraw()
{
}

void Object::EndPlay()
{
}

void Object::Collision()
{
}

void Object::Tick()
{
}
