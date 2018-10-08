#include "vector3.h"

template<>
Vector3<int>::Vector3()
{
	this->x = this->y = this->z = 0;
}

template<>
Vector3<float>::Vector3()
{
	this->x = this->y = this->z = 0.0f;
}

template<>
Vector3<double>::Vector3()
{
	this->x = this->y = this->z = 0.0;
}

template<>
int Vector3<int>::length() const
{
	return static_cast<int>(sqrt(this->x * this->x + this->y * this->y + this->z * this->z));
}

template<>
float Vector3<float>::length() const
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

template<>
double Vector3<double>::length() const
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

template<>
Vector3<int>& Vector3<int>::normalize()
{
	int l = this->length();
	if (l != 0)
	{
		this->x /= l;
		this->y /= l;
		this->z /= l;
	}

	return *this;
}

template<>
Vector3<float>& Vector3<float>::normalize()
{
	float l = this->length();
	if (l != 0.0f)
	{
		this->x /= l;
		this->y /= l;
		this->z /= l;
	}

	return *this;
}

template<>
Vector3<double>& Vector3<double>::normalize()
{
	double l = this->length();
	if (l != 0.0)
	{
		this->x /= l;
		this->y /= l;
		this->z /= l;
	}

	return *this;
}
