#pragma once

#include <fstream>
#include <bullet/btBulletDynamicsCommon.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

template<typename T>
struct Vector3
{
	T x;
	T y;
	T z;

	Vector3();
	Vector3(T x, T y, T z);
	Vector3(const btVector3& src);

	Vector3& operator=(const Vector3& src);
	Vector3& operator=(const btVector3& src);
	Vector3& operator=(const aiVector3D& src);
	Vector3 operator+(const Vector3& rhs) const;
	Vector3 operator-(const Vector3& rhs) const;
	Vector3 operator*(T rhs) const;
	Vector3& operator+=(const Vector3& rhs);
	Vector3& operator-=(const Vector3& rhs);
	Vector3& operator/=(T rhs);
	Vector3& operator*=(T rhs);
	bool operator==(const Vector3& rhs) const;
	bool operator!=(const Vector3& rhs) const;

	T dot(const Vector3& rhs) const;
	Vector3 cross(const Vector3& rhs) const;
	T length() const;
	T length2() const;
	Vector3& normalize();
	Vector3& lerp(const Vector3& other, T c);
};

template<typename T>
Vector3<T>::Vector3(T x, T y, T z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template<typename T>
Vector3<T>::Vector3(const btVector3& src)
{
	this->x = src.getX();
	this->y = src.getY();
	this->z = src.getZ();
}

template<typename T>
Vector3<T>& Vector3<T>::operator=(const Vector3& src)
{
	this->x = src.x;
	this->y = src.y;
	this->z = src.z;

	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator=(const btVector3& src)
{
	this->x = src.getX();
	this->y = src.getY();
	this->z = src.getZ();

	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator=(const aiVector3D& src)
{
	this->x = src.x;
	this->y = src.y;
	this->z = src.z;

	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3& rhs) const
{
	Vector3 result;

	result.x = this->x + rhs.x;
	result.y = this->y + rhs.y;
	result.z = this->z + rhs.z;

	return result;
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3& rhs) const
{
	Vector3 result;

	result.x = this->x - rhs.x;
	result.y = this->y - rhs.y;
	result.z = this->z - rhs.z;

	return result;
}

template<typename T>
Vector3<T> Vector3<T>::operator*(T rhs) const
{
	Vector3 result;

	result.x = this->x * rhs;
	result.y = this->y * rhs;
	result.z = this->z * rhs;

	return result;
}

template<typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator/=(T rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;

	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator*=(T rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;

	return *this;
}

template<typename T>
bool Vector3<T>::operator==(const Vector3& rhs) const
{
	return ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z));
}

template<typename T>
bool Vector3<T>::operator!=(const Vector3& rhs) const
{
	return ((this->x != rhs.x) || (this->y != rhs.y) || (this->z != rhs.z));
}

template<typename T>
T Vector3<T>::dot(const Vector3<T>& rhs) const
{
	return (this->x * rhs.x + this->y * rhs.y + this->z * rhs.z);
}

template<typename T>
Vector3<T> Vector3<T>::cross(const Vector3<T>& rhs) const
{
	Vector3 result;

	result.x = this->y * rhs.z - this->z * rhs.y;
	result.y = -(this->x * rhs.z - this->z * rhs.x);
	result.z = this->x * rhs.y - this->y * rhs.x;

	return result;
}

template<typename T>
T Vector3<T>::length2() const
{
	return (this->x * this->x + this->y * this->y + this->z * this->z);
}

template<typename T>
Vector3<T>& Vector3<T>::lerp(const Vector3& other, T c)
{
	this->x += (other.x - this->x) * c;
	this->y += (other.y - this->y) * c;
	this->z += (other.z - this->z) * c;

	return *this;
}

template<typename T>
std::ifstream& operator >> (std::ifstream& in, Vector3<T>& vector3)
{
	in >> vector3.x >> vector3.y >> vector3.z;

	return in;
}

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
