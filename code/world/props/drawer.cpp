#include "drawer.h"

Drawer::Drawer(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
	const Vector3f& rotationAxis, float rotationAngle, Prop* pDesk) :
	pFixedConstraint_(nullptr),
	isOpen_(false),
	isOpening_(false),
	isClosing_(false),
	elapsedTime_(0.0f),
	timeInterval_(1.0f),
	Prop(bodyFilename, modelFilename, false, true, position, rotationAxis, rotationAngle)
{
	try
	{
		pFixedConstraint_ = new FixedConstraint(
			pBody_,
			pDesk->getBody(),
			Vector3f(0.0f, 0.05f, 0.0f),
			btQuaternion({ 0.0f,1.0f,0.0f }, 0.0f),
			Vector3f(0.66f, 0.78f, 0.15f),
			btQuaternion({ 0.0f,1.0f,0.0f }, 0.0f)
		);
		PhysicsEngine::getInstance().addConstraint(pFixedConstraint_);

		pBody_->enableCollisionWithBody(pDesk->getBody(), false);
	}
	catch (...)
	{
		if (pFixedConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pFixedConstraint_);

			delete pFixedConstraint_;
			pFixedConstraint_ = nullptr;
		}

		throw;
	}
}

Drawer::~Drawer()
{
	if (pFixedConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pFixedConstraint_);

		delete pFixedConstraint_;
		pFixedConstraint_ = nullptr;
	}
}

bool Drawer::use(Prop* pOtherProp)
{
	if (isOpening_ || isClosing_)
	{
		elapsedTime_ = timeInterval_ - elapsedTime_;
		isOpening_ = isClosing_ = false;
	}

	if (!isOpen_)
	{
		isOpening_ = true;
		isOpen_ = true;
	}
	else
	{
		isClosing_ = true;
		isOpen_ = false;
	}
	return false;
}

void Drawer::update(float deltaTime)
{
	pShadedModel3D_->position_ = pBody_->getPosition();
	pShadedModel3D_->rotationAxis_ = pBody_->getRotationAxis();
	pShadedModel3D_->angle_ = pBody_->getRotationAngle();

	if (isOpening_)
	{
		elapsedTime_ += deltaTime;
		if (elapsedTime_ >= timeInterval_)
		{
			elapsedTime_ = 0.0f;
			pFixedConstraint_->setLinearLowerLimit({ 0.0f,0.0f,-0.25f });
			pFixedConstraint_->setLinearUpperLimit({ 0.0f,0.0f,-0.25f });
			isOpening_ = false;
		}
		else
		{
			float delta = -0.25f * (elapsedTime_ / timeInterval_);

			pFixedConstraint_->setLinearLowerLimit({ 0.0f,0.0f,delta });
			pFixedConstraint_->setLinearUpperLimit({ 0.0f,0.0f,delta });
		}
	}

	if (isClosing_)
	{
		elapsedTime_ += deltaTime;
		if (elapsedTime_ >= timeInterval_)
		{
			elapsedTime_ = 0.0f;
			pFixedConstraint_->setLinearLowerLimit({ 0.0f,0.0f,0.0f });
			pFixedConstraint_->setLinearUpperLimit({ 0.0f,0.0f,0.0f });
			isClosing_ = false;
		}
		else
		{
			float delta = -0.25f * (1.0f - (elapsedTime_ / timeInterval_));

			pFixedConstraint_->setLinearLowerLimit({ 0.0f,0.0f,delta });
			pFixedConstraint_->setLinearUpperLimit({ 0.0f,0.0f,delta });
		}
	}
}
