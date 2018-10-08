#include "constraint.h"
#include "physics_engine.h"

Constraint::Constraint() :
	pBody_(nullptr),
	pBtTypedConstraint_(nullptr)
{}

Constraint::~Constraint()
{
	if (pBtTypedConstraint_ != nullptr)
	{
		delete pBtTypedConstraint_;
		pBtTypedConstraint_ = nullptr;
	}
}

void Constraint::enable(bool flag)
{
	pBtTypedConstraint_->setEnabled(flag);

	pBody_->pRigidBody_->activate();
}

void Constraint::setBreakingImpulseThreshold(float breakingImpulseThreshold)
{
	pBtTypedConstraint_->setBreakingImpulseThreshold(breakingImpulseThreshold);
}
