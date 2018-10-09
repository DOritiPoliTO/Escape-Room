#include "door.h"
#include "../../game.h"

Door::Door(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
	const Vector3f& rotationAxis, float rotationAngle, Prop* pDoorFrame, Prop* pKey) :
	Prop(bodyFilename, modelFilename, false, true, position, rotationAxis, rotationAngle),
	state_(State::LOCKED),
	elapsedTime_(0.0f),
	pHingeConstraint_(nullptr),
	pFixedConstraint_(nullptr),
	pUnlockSound_(nullptr),
	pSqueakSound_(nullptr)
{
	try
	{
		pHingeConstraint_ = new HingeConstraint(
			pBody_,
			pDoorFrame->getBody(),
			Vector3f(-0.55f, 1.145f, 0.0f),
			Vector3f(-0.55f, 1.145f, 0.0f),
			Vector3f(0.0f, -1.0f, 0.0f),
			Vector3f(0.0f, -1.0f, 0.0f)
		);
		PhysicsEngine::getInstance().addConstraint(pHingeConstraint_);
		pHingeConstraint_->setLimit(0.0f, 0.0f);

		pFixedConstraint_ = new FixedConstraint(
			pBody_,
			pKey->getBody(),
			Vector3f(0.47f, 0.98f, 0.05f),
			btQuaternion({ 0.0f, 1.0f, 0.0f }, 0.0f),
			Vector3f(0.0f, 0.0f, 0.0f),
			btQuaternion({ 1.0f, 0.0f, 0.0f }, -1.57f)
		);
		PhysicsEngine::getInstance().addConstraint(pFixedConstraint_);
		pFixedConstraint_->enable(false);

		pUnlockSound_ = AudioSystem::getInstance().loadSound(
			"../EscapeRoom/data/sounds/door_unlock.wav",
			false
		);

		pSqueakSound_ = AudioSystem::getInstance().loadSound(
			"../EscapeRoom/data/sounds/door_squeak.aiff",
			false
		);

		pLockedSound_ = AudioSystem::getInstance().loadSound(
			"../EscapeRoom/data/sounds/door_locked.wav",
			false
		);
	}
	catch (...)
	{
		if (pLockedSound_ != nullptr)
		{
			delete pLockedSound_;
			pLockedSound_ = nullptr;
		}

		if (pSqueakSound_ != nullptr)
		{
			delete pSqueakSound_;
			pSqueakSound_ = nullptr;
		}

		if (pUnlockSound_ != nullptr)
		{
			delete pUnlockSound_;
			pUnlockSound_ = nullptr;
		}

		if (pFixedConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pFixedConstraint_);

			delete pFixedConstraint_;
			pFixedConstraint_ = nullptr;
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

Door::~Door()
{
	if (pLockedSound_ != nullptr)
	{
		delete pLockedSound_;
		pLockedSound_ = nullptr;
	}

	if (pSqueakSound_ != nullptr)
	{
		delete pSqueakSound_;
		pSqueakSound_ = nullptr;
	}

	if (pUnlockSound_ != nullptr)
	{
		delete pUnlockSound_;
		pUnlockSound_ = nullptr;
	}

	if (pFixedConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pFixedConstraint_);

		delete pFixedConstraint_;
		pFixedConstraint_ = nullptr;
	}

	if (pHingeConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pHingeConstraint_);

		delete pHingeConstraint_;
		pHingeConstraint_ = nullptr;
	}
}

bool Door::use(Prop* pOtherProp)
{
	switch (state_)
	{
	case Door::State::LOCKED:
		if (pOtherProp != nullptr
			&& pOtherProp->getTag() == "DoorKey")
		{
			pFixedConstraint_->enable(true);
			pOtherProp->getBody()->enableCollisionWithBody(pBody_, false);

			AudioSystem::getInstance().playSound(pUnlockSound_, 1.0f);

			state_ = Door::State::CLOSE;
			return true;
		}

		AudioSystem::getInstance().playSound(pLockedSound_, 1.0f);
		return false;
	case Door::State::OPEN:
	case Door::State::OPENING:
		AudioSystem::getInstance().playSound(pSqueakSound_, 1.0f);

		state_ = Door::State::CLOSING;
		return false;
	case Door::State::CLOSE:
	case Door::State::CLOSING:
		AudioSystem::getInstance().playSound(pSqueakSound_, 1.0f);

		state_ = Door::State::OPENING;
		return false;
	default:
		return false;
	}
}

void Door::update(float deltaTime)
{
	pShadedModel3D_->position_ = pBody_->getPosition();
	pShadedModel3D_->rotationAxis_ = pBody_->getRotationAxis();
	pShadedModel3D_->angle_ = pBody_->getRotationAngle();

	if (state_ == Door::State::OPENING)
	{
		elapsedTime_ += deltaTime;
		if (elapsedTime_ >= 1.0f)
		{
			elapsedTime_ = 0.0f;
			pHingeConstraint_->setLimit(90.0f, 90.0f);
			state_ = Door::State::OPEN;
		}
		else
		{
			float delta = 90.0f * elapsedTime_;

			pHingeConstraint_->setLimit(delta, delta);
		}

		if (elapsedTime_ >= 0.75f)
		{
			Game::getInstance().end();
		}
	}
	else if (state_ == Door::State::CLOSING)
	{
		elapsedTime_ += deltaTime;
		if (elapsedTime_ >= 1.0f)
		{
			elapsedTime_ = 0.0f;
			pHingeConstraint_->setLimit(0.0f, 0.0f);
			state_ = Door::State::CLOSE;
		}
		else
		{
			float delta = 90.0f * (1.0f - elapsedTime_);

			pHingeConstraint_->setLimit(delta, delta);
		}
	}
}
