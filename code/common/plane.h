#pragma once

#include "vector3.h"

class Plane
{
public:
	Plane();
	Plane(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2);

	float computeDistance(const Vector3f& point) const;

private:
	Vector3f normal_;
	float d_;
};
