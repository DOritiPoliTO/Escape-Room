#include "vector2.h"

template<>
Vector2<int>::Vector2()
{
	this->x = this->y = 0;
}

template<>
Vector2<float>::Vector2()
{
	this->x = this->y = 0.0f;
}

template<>
Vector2<double>::Vector2()
{
	this->x = this->y = 0.0;
}

template<>
int Vector2<int>::length() const
{
	return static_cast<int>(sqrt(this->x * this->x + this->y * this->y));
}

template<>
float Vector2<float>::length() const
{
	return sqrtf(this->x * this->x + this->y * this->y);
}

template<>
double Vector2<double>::length() const
{
	return sqrt(this->x * this->x + this->y * this->y);
}

template<>
Vector2<int>& Vector2<int>::normalize()
{
	int l = this->length();
	if (l != 0)
	{
		this->x /= l;
		this->y /= l;
	}

	return *this;
}

template<>
Vector2<float>& Vector2<float>::normalize()
{
	float l = this->length();
	if (l != 0.0f)
	{
		this->x /= l;
		this->y /= l;
	}

	return *this;
}

template<>
Vector2<double>& Vector2<double>::normalize()
{
	double l = this->length();
	if (l != 0.0)
	{
		this->x /= l;
		this->y /= l;
	}

	return *this;
}
