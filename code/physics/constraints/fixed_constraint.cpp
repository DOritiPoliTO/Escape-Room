#include "fixed_constraint.h"

FixedConstraint::FixedConstraint(Body* pBodyA, Body* pBodyB, const Vector3f& localPositionA,
	const btQuaternion& rotationA, const Vector3f& localPositionB, const btQuaternion& rotationB) :
	Constraint()
{
	btFixedConstraint* pFixedConstraint(nullptr);

	try
	{
		collisionsBetweenLinkedBodiesEnabled_ = true;

		pBody_ = pBodyA;
		pBodyB_ = pBodyB;

		btTransform frameInA;
		frameInA.setRotation(rotationA);
		frameInA.setOrigin(btVector3(localPositionA.x, localPositionA.y, localPositionA.z));

		btTransform frameInB;
		frameInB.setRotation(rotationB);
		frameInB.setOrigin(btVector3(localPositionB.x, localPositionB.y, localPositionB.z));

		pFixedConstraint = new btFixedConstraint(*pBodyA->pRigidBody_, *pBodyB->pRigidBody_, frameInA, frameInB);
		pFixedConstraint->setAngularLowerLimit({ 0.0f,0.0f,0.0f });
		pFixedConstraint->setAngularUpperLimit({ 0.0f,0.0f,0.0f });
		pFixedConstraint->setLinearLowerLimit({ 0.0f,0.0f,0.0f });
		pFixedConstraint->setLinearUpperLimit({ 0.0f,0.0f,0.0f });

		pBtTypedConstraint_ = reinterpret_cast<btTypedConstraint*>(pFixedConstraint);
	}
	catch (...)
	{
		if (pFixedConstraint != nullptr)
		{
			delete pFixedConstraint;
		}

		throw;
	}
}

void FixedConstraint::setAngularLowerLimit(const Vector3f& angularLowerLimit)
{
	reinterpret_cast<btFixedConstraint*>(pBtTypedConstraint_)->setAngularLowerLimit({ angularLowerLimit.x,angularLowerLimit.y,angularLowerLimit.z });

	pBody_->pRigidBody_->activate();
	pBodyB_->pRigidBody_->activate();
}

void FixedConstraint::setAngularUpperLimit(const Vector3f& angularUpperLimit)
{
	reinterpret_cast<btFixedConstraint*>(pBtTypedConstraint_)->setAngularLowerLimit({ angularUpperLimit.x,angularUpperLimit.y,angularUpperLimit.z });

	pBody_->pRigidBody_->activate();
	pBodyB_->pRigidBody_->activate();
}

void FixedConstraint::setLinearLowerLimit(const Vector3f& linearLowerLimit)
{
	reinterpret_cast<btFixedConstraint*>(pBtTypedConstraint_)->setLinearLowerLimit({ linearLowerLimit.x,linearLowerLimit.y,linearLowerLimit.z });

	pBody_->pRigidBody_->activate();
	pBodyB_->pRigidBody_->activate();
}

void FixedConstraint::setLinearUpperLimit(const Vector3f& linearUpperLimit)
{
	reinterpret_cast<btFixedConstraint*>(pBtTypedConstraint_)->setLinearUpperLimit({ linearUpperLimit.x,linearUpperLimit.y,linearUpperLimit.z });

	pBody_->pRigidBody_->activate();
	pBodyB_->pRigidBody_->activate();
}
