#include "painting.h"

Painting::Painting(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
	const Vector3f& rotationAxis, float rotationAngle, float pinRelativeHeight) :
	pPointConstraint_(nullptr),
	Prop(bodyFilename, modelFilename, true, false, position, rotationAxis, rotationAngle)
{
	try
	{
		Vector3f pinWorldPosition(pBody_->getPosition());
		pinWorldPosition.y += pinRelativeHeight;
		pPointConstraint_ = new PointConstraint(
			pBody_,
			pinWorldPosition,
			{ 0.0f,pinRelativeHeight,0.0f },
			0.25f
		);
		PhysicsEngine::getInstance().addConstraint(pPointConstraint_);
		pPointConstraint_->setBreakingImpulseThreshold(0.25f);
	}
	catch (...)
	{
		if (pPointConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pPointConstraint_);

			delete pPointConstraint_;
			pPointConstraint_ = nullptr;
		}

		throw;
	}
}

Painting::~Painting()
{
	if (pPointConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pPointConstraint_);

		delete pPointConstraint_;
		pPointConstraint_ = nullptr;
	}
}
