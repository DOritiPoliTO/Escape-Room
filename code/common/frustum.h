#pragma once

#include "plane.h"

class Frustum
{
public:
	Frustum();
	Frustum(const Vector3f& position, const Vector3f& direction, float fovy, float zNear,
		float zFar, float aspectRatio);

	bool checkSphere(float radius, const Vector3f& position) const;

private:
	Plane planes_[6];
};
