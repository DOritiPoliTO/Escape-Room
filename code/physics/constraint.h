#pragma once

#include "body.h"

class Constraint
{
	friend class PhysicsEngine;

public:
	void enable(bool flag);
	void setBreakingImpulseThreshold(float breakingImpulseThreshold);

protected:
	btTypedConstraint* pBtTypedConstraint_;
	bool collisionsBetweenLinkedBodiesEnabled_;
	Body* pBody_;

	Constraint();
	virtual ~Constraint() = 0;
};
