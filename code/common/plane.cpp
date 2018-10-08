#include "plane.h"

Plane::Plane() :
	normal_(0.0f, 1.0f, 0.0f),
	d_(0.0f)
{
}

Plane::Plane(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2)
{
	Vector3f v1 = p1 - p0;
	Vector3f v2 = p2 - p0;

	normal_ = v2.cross(v1).normalize();

	d_ = -normal_.dot(p0);
}

float Plane::computeDistance(const Vector3f& point) const
{
	return normal_.dot(point) + d_;
}
