#pragma once

#include <math.h>

template<typename T>
struct Matrix44
{
	T m00;
	T m01;
	T m02;
	T m03;
	T m10;
	T m11;
	T m12;
	T m13;
	T m20;
	T m21;
	T m22;
	T m23;
	T m30;
	T m31;
	T m32;
	T m33;

	Matrix44();
	Matrix44(T s00, T s01, T s02, T s03, T s10, T s11, T s12, T s13,
		T s20, T s21, T s22, T s23, T s30, T s31, T s32, T s33);
	Matrix44(const Matrix44& source);

	Matrix44 operator*(const Matrix44& rhs) const;

	Matrix44& setIdentity();
	Matrix44& setTranslation(T x, T y, T z);
	Matrix44& setRotation(T angle, T x, T y, T z);
	Matrix44& setScale(T scale);
	Matrix44& setPerspective(T fovy, T aspectRatio, T zNear, T zFar);
	Matrix44& setOrthographic(T right, T top, T zNear, T zFar);
	Matrix44& setLookAt(T eyeX, T eyeY, T eyeZ, T centerX, T centerY, T centerZ,
		T upX, T upY, T upZ);
};

template<typename T>
Matrix44<T>::Matrix44() {}

template<typename T>
Matrix44<T>::Matrix44(T s00, T s01, T s02, T s03, T s10, T s11, T s12, T s13,
	T s20, T s21, T s22, T s23, T s30, T s31, T s32, T s33)
{
	m00 = s00;
	m01 = s01;
	m02 = s02;
	m03 = s03;
	m10 = s10;
	m11 = s11;
	m12 = s12;
	m13 = s13;
	m20 = s20;
	m21 = s21;
	m22 = s22;
	m23 = s23;
	m30 = s30;
	m31 = s31;
	m32 = s32;
	m33 = s33;
}

template<typename T>
Matrix44<T>::Matrix44(const Matrix44<T>& source)
{
	m00 = source.m00;
	m01 = source.m01;
	m02 = source.m02;
	m03 = source.m03;
	m10 = source.m10;
	m11 = source.m11;
	m12 = source.m12;
	m13 = source.m13;
	m20 = source.m20;
	m21 = source.m21;
	m22 = source.m22;
	m23 = source.m23;
	m30 = source.m30;
	m31 = source.m31;
	m32 = source.m32;
	m33 = source.m33;
}

typedef Matrix44<float> Matrix44f;
