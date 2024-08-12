#include "Object.h"
#include <cmath>


Object::Object(Texture2D* textureReference, const Vector2 position, const Rectangle rectangleTexture)
	: bRegisterObject(false)
	, bToFrontRender(false)
	, position(position)
	, spriteResource(textureReference, rectangleTexture)
{
}

Object::Object()
	: bRegisterObject(false)
	, bToFrontRender(false)
	, spriteResource(Sprite{})
{
}

void Object::Draw()
{

	if (spriteResource.texture == nullptr)
	{
		return;
	}

	if(!IsTextureReady(*spriteResource.texture))
	{
		return;
	}

	const Vector2 positionRender = { static_cast<float>(round(position.position.x)), static_cast<float>(round(position.position.y)) };
	const Rectangle destination{
		 positionRender.x + (spriteResource.source.width / 2 - spriteResource.source.width * position.scale.x / 2),
		 positionRender.y + (spriteResource.source.height / 2 - spriteResource.source.height * position.scale.y / 2),
		spriteResource.source.width * position.scale.x,
		spriteResource.source.height * position.scale.y
	};
	DrawTexturePro(*spriteResource.texture, spriteResource.source, destination, Vector2{}, 0, position.color.GetColor()); //draw texture

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

void Object::BringToFrontRender()
{
	bToFrontRender = true;
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
