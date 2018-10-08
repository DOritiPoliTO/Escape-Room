#include "matrix44.h"

template<>
Matrix44<float> Matrix44<float>::operator*(const Matrix44<float>& rhs) const
{
	Matrix44 result;

	result.m00 = this->m00 * rhs.m00 + this->m10 * rhs.m01 + this->m20 * rhs.m02 + this->m30 * rhs.m03;
	result.m01 = this->m01 * rhs.m00 + this->m11 * rhs.m01 + this->m21 * rhs.m02 + this->m31 * rhs.m03;
	result.m02 = this->m02 * rhs.m00 + this->m12 * rhs.m01 + this->m22 * rhs.m02 + this->m32 * rhs.m03;
	result.m03 = this->m03 * rhs.m00 + this->m13 * rhs.m01 + this->m23 * rhs.m02 + this->m33 * rhs.m03;

	result.m10 = this->m00 * rhs.m10 + this->m10 * rhs.m11 + this->m20 * rhs.m12 + this->m30 * rhs.m13;
	result.m11 = this->m01 * rhs.m10 + this->m11 * rhs.m11 + this->m21 * rhs.m12 + this->m31 * rhs.m13;
	result.m12 = this->m02 * rhs.m10 + this->m12 * rhs.m11 + this->m22 * rhs.m12 + this->m32 * rhs.m13;
	result.m13 = this->m03 * rhs.m10 + this->m13 * rhs.m11 + this->m23 * rhs.m12 + this->m33 * rhs.m13;

	result.m20 = this->m00 * rhs.m20 + this->m10 * rhs.m21 + this->m20 * rhs.m22 + this->m30 * rhs.m23;
	result.m21 = this->m01 * rhs.m20 + this->m11 * rhs.m21 + this->m21 * rhs.m22 + this->m31 * rhs.m23;
	result.m22 = this->m02 * rhs.m20 + this->m12 * rhs.m21 + this->m22 * rhs.m22 + this->m32 * rhs.m23;
	result.m23 = this->m03 * rhs.m20 + this->m13 * rhs.m21 + this->m23 * rhs.m22 + this->m33 * rhs.m23;

	result.m30 = this->m00 * rhs.m30 + this->m10 * rhs.m31 + this->m20 * rhs.m32 + this->m30 * rhs.m33;
	result.m31 = this->m01 * rhs.m30 + this->m11 * rhs.m31 + this->m21 * rhs.m32 + this->m31 * rhs.m33;
	result.m32 = this->m02 * rhs.m30 + this->m12 * rhs.m31 + this->m22 * rhs.m32 + this->m32 * rhs.m33;
	result.m33 = this->m03 * rhs.m30 + this->m13 * rhs.m31 + this->m23 * rhs.m32 + this->m33 * rhs.m33;

	return result;
}

template<>
Matrix44<float>& Matrix44<float>::setIdentity()
{
	this->m00 = 1.0f;
	this->m01 = 0.0f;
	this->m02 = 0.0f;
	this->m03 = 0.0f;

	this->m10 = 0.0f;
	this->m11 = 1.0f;
	this->m12 = 0.0f;
	this->m13 = 0.0f;

	this->m20 = 0.0f;
	this->m21 = 0.0f;
	this->m22 = 1.0f;
	this->m23 = 0.0f;

	this->m30 = 0.0f;
	this->m31 = 0.0f;
	this->m32 = 0.0f;
	this->m33 = 1.0f;

	return *this;
}

template<>
Matrix44<float>& Matrix44<float>::setTranslation(float x, float y, float z)
{
	this->m00 = 1.0f;
	this->m01 = 0.0f;
	this->m02 = 0.0f;
	this->m03 = 0.0f;

	this->m10 = 0.0f;
	this->m11 = 1.0f;
	this->m12 = 0.0f;
	this->m13 = 0.0f;

	this->m20 = 0.0f;
	this->m21 = 0.0f;
	this->m22 = 1.0f;
	this->m23 = 0.0f;

	this->m30 = x;
	this->m31 = y;
	this->m32 = z;
	this->m33 = 1.0f;

	return *this;
}

template<>
Matrix44<float>& Matrix44<float>::setRotation(float angle, float x, float y, float z)
{
	float c = cosf(angle * 0.0174444444f);
	float s = sinf(angle * 0.0174444444f);

	float t0 = 1 - c;

	this->m00 = x * x * t0 + c;
	this->m01 = y * x * t0 + z * s;
	this->m02 = x * z * t0 - y * s;
	this->m03 = 0.0f;

	this->m10 = x * y * t0 - z * s;
	this->m11 = y * y * t0 + c;
	this->m12 = y * z * t0 + x * s;
	this->m13 = 0.0f;

	this->m20 = x * z * t0 + y * s;
	this->m21 = y * z * t0 - x * s;
	this->m22 = z * z * t0 + c;
	this->m23 = 0.0f;

	this->m30 = 0.0f;
	this->m31 = 0.0f;
	this->m32 = 0.0f;
	this->m33 = 1.0f;

	return *this;
}

template<>
Matrix44<float>& Matrix44<float>::setScale(float scale)
{
	this->m00 = scale;
	this->m01 = 0.0f;
	this->m02 = 0.0f;
	this->m03 = 0.0f;

	this->m10 = 0.0f;
	this->m11 = scale;
	this->m12 = 0.0f;
	this->m13 = 0.0f;

	this->m20 = 0.0f;
	this->m21 = 0.0f;
	this->m22 = scale;
	this->m23 = 0.0f;

	this->m30 = 0.0f;
	this->m31 = 0.0f;
	this->m32 = 0.0f;
	this->m33 = 1.0f;

	return *this;
}

template<>
Matrix44<float>& Matrix44<float>::setPerspective(float fovy, float aspectRatio, float zNear, float zFar)
{
	float top = zNear * tanf(fovy * 3.14f / 360.0f);
	float right = top * aspectRatio;

	this->m00 = zNear / right;
	this->m01 = 0.0f;
	this->m02 = 0.0f;
	this->m03 = 0.0f;

	this->m10 = 0.0f;
	this->m11 = zNear / top;
	this->m12 = 0.0f;
	this->m13 = 0.0f;

	this->m20 = 0.0f;
	this->m21 = 0.0f;
	this->m22 = -(zFar + zNear) / (zFar - zNear);
	this->m23 = -1.0f;

	this->m30 = 0.0f;
	this->m31 = 0.0f;
	this->m32 = -2.0f * zFar * zNear / (zFar - zNear);
	this->m33 = 0.0f;

	return *this;
}

template<>
Matrix44<float>& Matrix44<float>::setOrthographic(float right, float top, float zNear, float zFar)
{
	this->m00 = 1.0f / right;
	this->m01 = 0.0f;
	this->m02 = 0.0f;
	this->m03 = 0.0f;

	this->m10 = 0.0f;
	this->m11 = 1.0f / top;
	this->m12 = 0.0f;
	this->m13 = 0.0f;

	this->m20 = 0.0f;
	this->m21 = 0.0f;
	this->m22 = -2.0f / (zFar - zNear);
	this->m23 = 0.0f;

	this->m30 = 0.0f;
	this->m31 = 0.0f;
	this->m32 = -1.0f * (zFar + zNear) / (zFar - zNear);
	this->m33 = 1.0f;

	return *this;
}

template<>
Matrix44<float>& Matrix44<float>::setLookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY,
	float centerZ, float upX, float upY, float upZ)
{
	float fX = centerX - eyeX;
	float fY = centerY - eyeY;
	float fZ = centerZ - eyeZ;
	float c = sqrtf(fX * fX + fY * fY + fZ * fZ);
	fX /= c;
	fY /= c;
	fZ /= c;

	float sX = fY * upZ - fZ * upY;
	float sY = -(fX * upZ - fZ * upX);
	float sZ = fX * upY - fY * upX;
	c = sqrtf(sX * sX + sY * sY + sZ * sZ);
	sX /= c;
	sY /= c;
	sZ /= c;

	float uX = sY * fZ - sZ * fY;
	float uY = -(sX * fZ - sZ * fX);
	float uZ = sX * fY - sY * fX;

	this->m00 = sX;
	this->m01 = uX;
	this->m02 = -fX;
	this->m03 = 0.0f;

	this->m10 = sY;
	this->m11 = uY;
	this->m12 = -fY;
	this->m13 = 0.0f;

	this->m20 = sZ;
	this->m21 = uZ;
	this->m22 = -fZ;
	this->m23 = 0.0f;

	this->m30 = -sX * eyeX - sY * eyeY - sZ * eyeZ;
	this->m31 = -uX * eyeX - uY * eyeY - uZ * eyeZ;
	this->m32 = fX * eyeX + fY * eyeY + fZ * eyeZ;
	this->m33 = 1.0f;

	return *this;
}
