#pragma once

#include "Math.h"
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

struct Colors
{

private:
	Color color;
	int r;
	int g;
	int b;
	int a;

public:

	Colors()
		: color{ WHITE }, r(255), g(255), b(255), a(255)
	{
	}

	Colors(int r, int g, int b, int a)
		: color{
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b),
			static_cast<unsigned char>(a)
			},
		r(r), g(g), b(b), a(a)
	{
	}

	explicit Colors(const Color& color): color(color), r(color.r), g(color.g), b(color.b), a(color.a)
	{
	}


	void SetColor(const Color& newColor)
	{
		r = newColor.r;
		g = newColor.g;
		b = newColor.b;
		a = newColor.a;
		color = newColor;
	}
	void SetColor(const int red, const int green, const int blue)
	{
		r = red;
		g = green;
		b = blue;
		UpdateColor();
	}
	void SetColor(const int red, const int green, const int blue, const int alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
		UpdateColor();
	}
	void SetColorAlpha(const int alpha)
	{
		a = alpha;
		UpdateColor();
	}

	int GetColorRed() const { return r; }
	int GetColorGreen() const { return g; }
	int GetColorBlue() const { return b; }
	int GetColorAlpha() const { return a; }

	Color GetColor() const { return color;}

private:
	void UpdateColor()
	{
		color.r = Math::Clamp(r, 0, 255);
		color.g = Math::Clamp(g, 0, 255);
		color.b = Math::Clamp(b, 0, 255);
		color.a = Math::Clamp(a, 0, 255);
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
