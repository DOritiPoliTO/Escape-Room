#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <bullet/btBulletDynamicsCommon.h>
#include "../common/vector3.h"
#include "../common/utility.h"
#include "../common/batch_allocator.h"

class Body
{
	friend class PhysicsEngine;
	friend class Constraint;
	friend class PointConstraint;
	friend class HingeConstraint;
	friend class Generic6DofConstraint;
	friend class FixedConstraint;

public:
	Body(const char* bodyFilename, void* pUserPointer, const Vector3f& position, const Vector3f& rotationAxis,
		float rotationAngle);
	~Body();

	void applyImpulse(const Vector3f& impulse);
	void applyImpulse(const Vector3f& impulse, const Vector3f& position);
	void applyTorqueImpulse(const Vector3f& torqueImpulse);
	void setVelocity(const Vector3f& velocity);
	void setAngularVelocity(const Vector3f& angularVelocity);

	void setLocalScaling(const Vector3f& scale);

	void enableCollisionWithBody(Body* pOtherBody, bool enable);

	void convertFromLocalToWorldSpace(const Vector3f& localPosition, Vector3f& worldPosition);

	const Vector3f& getPosition() const;
	const Vector3f& getVelocity() const;
	Vector3f getRotationAxis() const;
	btQuaternion getRotation() const;
	float getRotationAngle() const;
	float getMass() const;
	void* getUserPointer() const;

	static void* operator new(std::size_t count);
	static void operator delete(void* ptr) noexcept;

private:
	std::vector<btCollisionShape*> pShapes_;
	btDefaultMotionState* pMotionState_;
	btRigidBody* pRigidBody_;
	float mass_;
	void* pUserPointer_;
};
