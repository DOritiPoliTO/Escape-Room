#include "generic_6_dof_constraint.h"

Generic6DofConstraint::Generic6DofConstraint(Body* pBody, const Vector3f& position,
	const btQuaternion& rotation)
	:
	Constraint()
{
	btGeneric6DofConstraint* pGeneric6DofConstraint(nullptr);

	try
	{
		collisionsBetweenLinkedBodiesEnabled_ = true;

		pBody_ = pBody;

		btRigidBody* pbtRigidBody = pBody->pRigidBody_;

		btVector3 worldPosition(position.x, position.y, position.z);
		btVector3 localPosition(pbtRigidBody->getCenterOfMassTransform().inverse() * worldPosition);
		btTransform frameInB(rotation, localPosition);
		pGeneric6DofConstraint = new btGeneric6DofConstraint(*pbtRigidBody, frameInB, true);
		pGeneric6DofConstraint->setAngularLowerLimit({ 0.0f,0.0f,0.0f });
		pGeneric6DofConstraint->setAngularUpperLimit({ 0.0f,0.0f,0.0f });
		pGeneric6DofConstraint->setLinearLowerLimit({ 0.0f,0.0f,0.0f });
		pGeneric6DofConstraint->setLinearUpperLimit({ 0.0f,0.0f,0.0f });
		btTransform frameInA(btQuaternion({ 0.0f,1.0f,0.0f }, 0.0f), worldPosition);
		pGeneric6DofConstraint->setFrames(frameInA, frameInB);

		pBtTypedConstraint_ = reinterpret_cast<btTypedConstraint*>(pGeneric6DofConstraint);
	}
	catch (...)
	{
		if (pGeneric6DofConstraint != nullptr)
		{
			delete pGeneric6DofConstraint;
		}

		throw;
	}
}

void Generic6DofConstraint::setAngularLowerLimit(const Vector3f& angularLowerLimit)
{
	reinterpret_cast<btGeneric6DofConstraint*>(pBtTypedConstraint_)->setAngularLowerLimit({ angularLowerLimit.x,angularLowerLimit.y,angularLowerLimit.z });

	pBody_->pRigidBody_->activate();
}

void Generic6DofConstraint::setAngularUpperLimit(const Vector3f& angularUpperLimit)
{
	reinterpret_cast<btGeneric6DofConstraint*>(pBtTypedConstraint_)->setAngularLowerLimit({ angularUpperLimit.x,angularUpperLimit.y,angularUpperLimit.z });

	pBody_->pRigidBody_->activate();
}

void Generic6DofConstraint::setLinearLowerLimit(const Vector3f& linearLowerLimit)
{
	reinterpret_cast<btGeneric6DofConstraint*>(pBtTypedConstraint_)->setLinearLowerLimit({ linearLowerLimit.x,linearLowerLimit.y,linearLowerLimit.z });

	pBody_->pRigidBody_->activate();
}

void Generic6DofConstraint::setLinearUpperLimit(const Vector3f& linearUpperLimit)
{
	reinterpret_cast<btGeneric6DofConstraint*>(pBtTypedConstraint_)->setLinearUpperLimit({ linearUpperLimit.x,linearUpperLimit.y,linearUpperLimit.z });

	pBody_->pRigidBody_->activate();
}
