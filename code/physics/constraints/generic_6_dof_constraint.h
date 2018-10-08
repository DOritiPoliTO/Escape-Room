#pragma once

#include "../constraint.h"

class Generic6DofConstraint :public Constraint
{
	friend class PhysicsEngine;

public:
	Generic6DofConstraint(Body* pBody, const Vector3f& position, const btQuaternion& rotation);

	void setAngularLowerLimit(const Vector3f& angularLowerLimit);
	void setAngularUpperLimit(const Vector3f& angularUpperLimit);
	void setLinearLowerLimit(const Vector3f& linearLowerLimit);
	void setLinearUpperLimit(const Vector3f& linearUpperLimit);
};
