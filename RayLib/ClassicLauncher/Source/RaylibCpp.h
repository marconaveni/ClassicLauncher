#pragma once

#include "raylib.h"
#include "Math.h"

template<typename T>
struct Vector2D
{
	T x;
	T y;

	Vector2D(T x, T y)
		:x(x)
		, y(y)
	{}

	Vector2D& operator=(const Vector2 a)
	{
		this->x = a.x;
		this->y = a.y;
		return *this;
	}

	Vector2 ToVector2()
	{
		return Vector2{ this->x,this->y };
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

	explicit Colors(const Color& color) : color(color), r(color.r), g(color.g), b(color.b), a(color.a)
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

	Color GetColor() const { return color; }

private:
	void UpdateColor()
	{
		color.r = static_cast<unsigned char>(Math::Clamp(r, 0, 255));
		color.g = static_cast<unsigned char>(Math::Clamp(g, 0, 255));
		color.b = static_cast<unsigned char>(Math::Clamp(b, 0, 255));
		color.a = static_cast<unsigned char>(Math::Clamp(a, 0, 255));
	}
};