#pragma once

#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

template<typename T>
struct Vector2
{
	T x;
	T y;

	Vector2();
	Vector2(T x, T y);
	Vector2(const Vector2<int>& src);

	Vector2& operator=(const aiVector3D& src);
	Vector2& operator=(const Vector2& src);
	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	Vector2 operator*(T rhs) const;
	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	Vector2& operator*=(T rhs);
	Vector2& operator/=(T rhs);
	bool operator==(const Vector2& rhs) const;
	bool operator!=(const Vector2& rhs) const;

	T dot(const Vector2& rhs) const;
	T length() const;
	T length2() const;
	Vector2& normalize();
};

template<typename T>
Vector2<T>::Vector2(T x, T y)
{
	this->x = x;
	this->y = y;
}

template<typename T>
Vector2<T>::Vector2(const Vector2<int>& src)
{
	this->x = (T)src.x;
	this->y = (T)src.y;
}

template<typename T>
Vector2<T>& Vector2<T>::operator=(const aiVector3D& src)
{
	this->x = src.x;
	this->y = src.y;

	return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator=(const Vector2& src)
{
	this->x = src.x;
	this->y = src.y;

	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2& rhs) const
{
	Vector2 result;

	result.x = this->x + rhs.x;
	result.y = this->y + rhs.y;

	return result;
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2& rhs) const
{
	Vector2 result;

	result.x = this->x - rhs.x;
	result.y = this->y - rhs.y;

	return result;
}

template<typename T>
Vector2<T> Vector2<T>::operator*(T rhs) const
{
	Vector2 result;

	result.x = this->x * rhs;
	result.y = this->y * rhs;

	return result;
}

template<typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;

	return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;

	return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator*=(T rhs)
{
	this->x *= rhs;
	this->y *= rhs;

	return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator/=(T rhs)
{
	this->x /= rhs;
	this->y /= rhs;

	return *this;
}

template<typename T>
bool Vector2<T>::operator==(const Vector2& rhs) const
{
	return ((this->x == rhs.x) && (this->y == rhs.y));
}

template<typename T>
bool Vector2<T>::operator!=(const Vector2& rhs) const
{
	return ((this->x != rhs.x) || (this->y != rhs.y));
}

template<typename T>
T Vector2<T>::dot(const Vector2& rhs) const
{
	return (this->x * rhs.x + this->y * rhs.y);
}

template<typename T>
T Vector2<T>::length2() const
{
	return (this->x * this->x + this->y * this->y);
}

template<typename T>
std::ifstream& operator >> (std::ifstream& in, Vector2<T>& vector2)
{
	in >> vector2.x >> vector2.y;

	return in;
}

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
