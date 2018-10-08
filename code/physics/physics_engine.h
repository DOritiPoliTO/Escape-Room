#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "body.h"
#include "constraints/fixed_constraint.h"
#include "constraints/generic_6_dof_constraint.h"
#include "constraints/hinge_constraint.h"
#include "constraints/point_constraint.h"

class PhysicsEngine
{
public:
	static PhysicsEngine& getInstance();

	void update(float deltaTime);

	void addBody(Body* pBody);
	void removeBody(Body* pBody);
	void addConstraint(Constraint* pConstraint);
	void removeConstraint(Constraint* pConstraint);

	// @DEBUG
	void setDebugDrawer(btIDebugDraw* pDebugDrawer);
	void debugDrawWorld() const;

	Body* raycast(const Vector3f& position, const Vector3f& direction, float distance);

private:
	btBroadphaseInterface* pBroadphase_;
	btDefaultCollisionConfiguration* pCollisionConfiguration_;
	btCollisionDispatcher* pDispatcher_;
	btSequentialImpulseConstraintSolver* pSolver_;
	btDiscreteDynamicsWorld* pDynamicsWorld_;

	PhysicsEngine();
	~PhysicsEngine();
};
