#pragma once

#include "../constraint.h"

class FixedConstraint :public Constraint
{
	friend class PhysicsEngine;

public:
	FixedConstraint(Body* pBodyA, Body* pBodyB, const Vector3f& localPositionA,
		const btQuaternion& rotationA, const Vector3f& localPositionB, const btQuaternion& rotationB);

	void setAngularLowerLimit(const Vector3f& angularLowerLimit);
	void setAngularUpperLimit(const Vector3f& angularUpperLimit);
	void setLinearLowerLimit(const Vector3f& linearLowerLimit);
	void setLinearUpperLimit(const Vector3f& linearUpperLimit);

private:
	Body* pBodyB_;
};
