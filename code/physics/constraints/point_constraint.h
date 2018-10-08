#pragma once

#include "../constraint.h"

class PointConstraint :public Constraint
{
	friend class PhysicsEngine;

public:
	PointConstraint(Body* pBody, const Vector3f& position, const Vector3f& pivot, float impulseClamp);

	void updatePosition(const Vector3f & position);
};
