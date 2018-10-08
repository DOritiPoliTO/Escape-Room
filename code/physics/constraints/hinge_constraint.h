#pragma once

#include "../constraint.h"

class HingeConstraint :public Constraint
{
	friend class PhysicsEngine;

public:
	HingeConstraint(Body* pBodyA, Body* pBodyB, const Vector3f& pivotInA, const Vector3f& pivotInB,
		const Vector3f& axisInA, const Vector3f& axisInB);

	void setLimit(float minAngle, float maxAngle);

	float getAngle();

private:
	Body* pBodyB_;
};
