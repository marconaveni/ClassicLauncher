#pragma once

#include "Math.h"
#include "RaylibCpp.h"


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

struct Position
{
	Vector2 position;
	Vector2 scale;
	Colors color;

	Position()
		: position(Vector2{})
		, scale(Vector2{ 1,1 })
		, color(WHITE)
	{
	}

	explicit Position(const Vector2& position)
		: position(position)
		, scale(Vector2{ 1,1 })
		, color(WHITE)
	{
	}

	Position(const Vector2 position, const Vector2 scale)
		: position(position)
		, scale(scale)
		, color(WHITE)
	{
	}

	Position(const Vector2 position, const Vector2 scale, const Color color)
		: position(position)
		, scale(scale)
		, color(color)
	{
	}

};


class Object
{
	bool bRegisterObject;
	bool bToFrontRender;

public:

	Position position;

	friend class ObjectManager;

	Object(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture);
	Object();
	virtual ~Object();

	//int id;
	//Vector2 position;
	Sprite spriteResource;
	//float scale;

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
