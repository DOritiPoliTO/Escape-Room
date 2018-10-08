#include "safe_lid.h"
#include "../../game.h"

SafeLid::SafeLid(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
	const Vector3f& rotationAxis, float rotationAngle, Prop* pSafe) :
	pHingeConstraint_(nullptr),
	pSageImageBillboard_(nullptr),
	elapsedTime_(0.0f),
	timeInterval_(2.0f),
	state_(SafeLid::State::LOCKED),
	Prop(bodyFilename, modelFilename, false, true, position, rotationAxis, rotationAngle)
{
	try
	{
		pHingeConstraint_ = new HingeConstraint(
			pBody_,
			pSafe->getBody(),
			Vector3f(0.14f, 0.2f, 0.0f),
			Vector3f(0.14f, 0.2f, 0.185f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 1.0f, 0.0f)
		);
		PhysicsEngine::getInstance().addConstraint(pHingeConstraint_);
		pHingeConstraint_->setLimit(0.0f, 0.0f);

		pSageImageBillboard_ = new Billboard2D("../EscapeRoom/data/textures/safe.png", 1.0f);
	}
	catch (...)
	{
		if (pSageImageBillboard_ != nullptr)
		{
			delete pSageImageBillboard_;
			pSageImageBillboard_ = nullptr;
		}

		if (pHingeConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pHingeConstraint_);

			delete pHingeConstraint_;
			pHingeConstraint_ = nullptr;
		}

		throw;
	}
}

SafeLid::~SafeLid()
{
	if (pSageImageBillboard_ != nullptr)
	{
		delete pSageImageBillboard_;
		pSageImageBillboard_ = nullptr;
	}

	if (pHingeConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pHingeConstraint_);

		delete pHingeConstraint_;
		pHingeConstraint_ = nullptr;
	}
}

bool SafeLid::use(Prop* pOtherProp)
{
	switch (state_)
	{
	case SafeLid::State::LOCKED:
	{
		Renderer::getInstance().scenes_[SceneId::KEYPAD_SCENE].add(pSageImageBillboard_);

		Game::getInstance().useKeypad(
			3,
			std::bind(
				&SafeLid::keypadCallback,
				this,
				std::placeholders::_1
			)
		);
		return false;
	}
	case SafeLid::State::UNLOCKED:
	{
		Game::getInstance().playVideo(
			"../EscapeRoom/data/videos/safe_animation.mkv",
			std::bind(&SafeLid::videoCallback, this)
		);
		return false;
	}
	case SafeLid::State::OPEN:
	{
		pBody_->applyImpulse(Vector3f(0.0f, 0.0f, -0.4f), Vector3f(0.3f, 0.0f, 0.0f));
		return false;
	}
	default:
		return false;
	}
}

void SafeLid::update(float deltaTime)
{
	pShadedModel3D_->position_ = pBody_->getPosition();
	pShadedModel3D_->rotationAxis_ = pBody_->getRotationAxis();
	pShadedModel3D_->angle_ = pBody_->getRotationAngle();
}

bool SafeLid::keypadCallback(const int* numbers)
{
	if (numbers == nullptr)
	{
		Renderer::getInstance().scenes_[SceneId::KEYPAD_SCENE].remove(pSageImageBillboard_);

		return false;
	}

	if (numbers[0] == 8
		&& numbers[1] == 3
		&& numbers[2] == 4)
	{
		Renderer::getInstance().scenes_[SceneId::KEYPAD_SCENE].remove(pSageImageBillboard_);

		state_ = SafeLid::State::UNLOCKED;

		return true;
	}

	return false;
}

void SafeLid::videoCallback()
{
	pHingeConstraint_->setLimit(0.0f, 90.0f);

	state_ = SafeLid::State::OPEN;
}
