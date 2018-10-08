#include "point_constraint.h"

PointConstraint::PointConstraint(Body* pBody, const Vector3f& position, const Vector3f& pivot,
	float impulseClamp)
	:
	Constraint()
{
	btPoint2PointConstraint* pPoint2PointConstraint(nullptr);

	try
	{
		pBody_ = pBody;

		collisionsBetweenLinkedBodiesEnabled_ = false;

		btRigidBody* pbtRigidBody = pBody->pRigidBody_;

		pPoint2PointConstraint = new btPoint2PointConstraint(*pbtRigidBody, btVector3(pivot.x, pivot.y, pivot.z));
		btVector3 worldPivotInB(position.x, position.y, position.z);
		btVector3 localPivotInB(pPoint2PointConstraint->getFixedBody().getCenterOfMassTransform().inverse() * worldPivotInB);
		pPoint2PointConstraint->setPivotB(localPivotInB);
		pPoint2PointConstraint->m_setting.m_impulseClamp = impulseClamp;
		pPoint2PointConstraint->m_setting.m_tau = 0.001f;

		pBtTypedConstraint_ = reinterpret_cast<btTypedConstraint*>(pPoint2PointConstraint);

		pBody_->pRigidBody_->activate();
	}
	catch (...)
	{
		if (pPoint2PointConstraint != nullptr)
		{
			delete pPoint2PointConstraint;
		}

		throw;
	}
}

void PointConstraint::updatePosition(const Vector3f & position)
{
	btVector3 worldPivotB(position.x, position.y, position.z);
	btVector3 localPivotB(pBtTypedConstraint_->getFixedBody().getCenterOfMassTransform().inverse() * worldPivotB);
	reinterpret_cast<btPoint2PointConstraint*>(pBtTypedConstraint_)->setPivotB(localPivotB);
}
