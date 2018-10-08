#include "frustum.h"

Frustum::Frustum() :
	Frustum(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), 45.0f, 0.1f, 1000.0f, 1.0f)
{
}

Frustum::Frustum(const Vector3f& position, const Vector3f& direction, float fovy, float zNear,
	float zFar, float aspectRatio)
{
	Vector3f right = Vector3f(0.0f, 1.0f, 0.0f).cross(direction).normalize();
	Vector3f up = direction.cross(right);

	float nHalfWidth = std::tanf(fovy / 2.0f) * zNear;
	float nHalfHeight = nHalfWidth * aspectRatio;
	float nWidth = nHalfWidth * 2.0f;
	float nHeight = nHalfHeight * 2.0f;

	float fHalfWidth = std::tanf(fovy / 2.0f) * zFar;
	float fHalfHeight = fHalfWidth * aspectRatio;
	float fWidth = fHalfWidth * 2.0f;
	float fHeight = fHalfHeight * 2.0f;

	Vector3f nc = position + direction * zNear;
	Vector3f fc = position + direction * zFar;

	Vector3f ntl = nc + up * nHalfHeight - right * nHalfWidth;
	Vector3f ntr = ntl + right * nWidth;
	Vector3f nbr = ntr - up * nHeight;
	Vector3f nbl = nbr - right * nWidth;

	Vector3f ftl = fc + up * fHalfHeight - right * fHalfWidth;
	Vector3f ftr = ftl + right * fWidth;
	Vector3f fbr = ftr - up * fHeight;
	Vector3f fbl = fbr - right * fWidth;

	new (&planes_[0]) Plane(ntr, ftr, fbr);
	new (&planes_[1]) Plane(ntl, ftl, ftr);
	new (&planes_[2]) Plane(nbl, fbl, ftl);
	new (&planes_[3]) Plane(nbr, fbr, fbl);
	new (&planes_[4]) Plane(ntl, ntr, nbr);
	new (&planes_[5]) Plane(ftr, ftl, fbl);
}

bool Frustum::checkSphere(float radius, const Vector3f& position) const
{
	for (size_t i = 0; i < 6; i++)
	{
		float distance = planes_[i].computeDistance(position);
		if (distance < -radius)
		{
			return false;
		}
	}

	return true;
}
