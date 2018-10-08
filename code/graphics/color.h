#pragma once

struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color();
	Color(float r, float g, float b);
	Color(float r, float g, float b, float a);
};

inline Color::Color() :
	r(1.0f),
	g(1.0f),
	b(1.0f),
	a(1.0f)
{
}

inline Color::Color(float r, float g, float b) :
	r(r),
	g(g),
	b(b),
	a(1.0f)
{
}

inline Color::Color(float r, float g, float b, float a) :
	r(r),
	g(g),
	b(b),
	a(a)
{
}
