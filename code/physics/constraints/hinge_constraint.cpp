#include "hinge_constraint.h"

HingeConstraint::HingeConstraint(Body* pBodyA, Body* pBodyB, const Vector3f& pivotInA,
	const Vector3f& pivotInB, const Vector3f& axisInA, const Vector3f& axisInB)
	:
	Constraint(),
	pBodyB_(nullptr)
{
	btHingeConstraint* pHingeConstraint(nullptr);

	try
	{
		pBody_ = pBodyA;
		pBodyB_ = pBodyB;

		btRigidBody* pbtRigidBodyA = pBodyA->pRigidBody_;
		btRigidBody* pbtRigidBodyB = pBodyB->pRigidBody_;

		btVector3 _axisInA(axisInA.x, axisInA.y, axisInA.z);
		btVector3 _axisInB(axisInB.x, axisInB.y, axisInB.z);
		btVector3 _pivotInA(pivotInA.x, pivotInA.y, pivotInA.z);
		btVector3 _pivotInB(pivotInB.x, pivotInB.y, pivotInB.z);
		pHingeConstraint = new btHingeConstraint(*pbtRigidBodyA, *pbtRigidBodyB, _pivotInA, _pivotInB, _axisInA, _axisInB);
		pHingeConstraint->setLimit(0.0f, 0.0f);

		pBtTypedConstraint_ = reinterpret_cast<btTypedConstraint*>(pHingeConstraint);
	}
	catch (...)
	{
		if (pHingeConstraint != nullptr)
		{
			delete pHingeConstraint;
		}

		throw;
	}
}

void HingeConstraint::setLimit(float minAngle, float maxAngle)
{
	reinterpret_cast<btHingeConstraint*>(pBtTypedConstraint_)->setLimit(minAngle * 0.017444f, maxAngle * 0.017444f, 0.9f, 0.1f, 1.0f);

	pBody_->pRigidBody_->activate();
	pBodyB_->pRigidBody_->activate();
}

float HingeConstraint::getAngle()
{
	return static_cast<float>(reinterpret_cast<btHingeConstraint*>(pBtTypedConstraint_)->getHingeAngle());
}
