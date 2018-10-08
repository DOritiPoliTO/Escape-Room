#include "actor.h"
#include "../game.h"

Actor::Actor(const char* bodyFilename) :
	Entity("Actor"),
	pBody_(nullptr),
	pGeneric6DofConstraint_(nullptr),
	pCamera_(nullptr),
	pCandle_(nullptr),
	pPickedProp_(nullptr),
	pPickingConstraint_(nullptr),
	pCandleConstraint_(nullptr),
	pMousePointerBillboard_(nullptr),
	pJumpCmdText_(nullptr),
	pCrouchCmdText_(nullptr),
	pMoveCmdText_(nullptr),
	pOpenDiaryCmdText_(nullptr),
	pUsePropCmdText_(nullptr),
	pPickPropCmdText_(nullptr),
	pCheckPropCmdText_(nullptr),
	pThrowPropCmdText_(nullptr),
	pRotatePropCmdText_(nullptr),
	pGetCloserPropCmdText_(nullptr),
	pFootstepSound_(nullptr),
	state_(Actor::State::IDLE),
	cameraWobblingDeltaY_(0.0f),
	s1_(sinf(-30.0f * 0.0174444f)),
	c1_(cosf(-30.0f * 0.0174444f)),
	s2_(sinf(30.0f * 0.0174444f)),
	c2_(cosf(30.0f * 0.0174444f)),
	elapsedTime_(0.0f)
{
	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];

	try
	{
		// Initialise components and create resources
		{
			pBody_ = new Body(
				bodyFilename,
				this,
				Vector3f(0.0f, 0.0f, 0.0f),
				Vector3f(0.0f, 1.0f, 0.0f),
				0.0f
			);
			PhysicsEngine::getInstance().addBody(pBody_);

			pGeneric6DofConstraint_ = new Generic6DofConstraint(
				pBody_,
				Vector3f(0.0f, 0.0f, 0.0f),
				pBody_->getRotation()
			);
			PhysicsEngine::getInstance().addConstraint(pGeneric6DofConstraint_);

			pCamera_ = &worldScene.camera_;

			pOpenDiaryCmdText_ = new Text2D(
				"Q: Open Diary",
				Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pMoveCmdText_ = new Text2D(
				"W, A, S, D: Move",
				Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pCrouchCmdText_ = new Text2D(
				"C: Crouch",
				Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pJumpCmdText_ = new Text2D(
				"Spacebar: Jump",
				Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pUsePropCmdText_ = new Text2D(
				"U: Use Object",
				Text2D::Alignment::CENTER,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pPickPropCmdText_ = new Text2D(
				"G: Pick Object",
				Text2D::Alignment::CENTER,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pCheckPropCmdText_ = new Text2D(
				"V: Examine Object",
				Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pThrowPropCmdText_ = new Text2D(
				"T: Throw object",
				Text2D::Alignment::LEFT,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pRotatePropCmdText_ = new Text2D(
				"LMB: Rotate Object",
				Text2D::Alignment::CENTER,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pGetCloserPropCmdText_ = new Text2D(
				"RMB: Move Object Closer",
				Text2D::Alignment::CENTER,
				"../EscapeRoom/data/fonts/candara.fnt"
			);

			pMousePointerBillboard_ = new Billboard2D("../EscapeRoom/data/textures/pointer.png", 0.01f);

			pFootstepSound_ = AudioSystem::getInstance().loadSound("../EscapeRoom/data/sounds/footstep.wav", false);
		}

		// Configure components and resources
		{
			pGeneric6DofConstraint_->setLinearLowerLimit({ -100.0f,0.05f,-100.0f });
			pGeneric6DofConstraint_->setLinearUpperLimit({ 100.0f,20.0f,100.0f });

			pOpenDiaryCmdText_->position_ = Vector2f(-0.98f, 0.96f);
			pOpenDiaryCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pOpenDiaryCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			worldScene.add(pOpenDiaryCmdText_);

			pMoveCmdText_->position_ = Vector2f(-0.98f, 0.88f);
			pMoveCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pMoveCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			worldScene.add(pMoveCmdText_);

			pCrouchCmdText_->position_ = Vector2f(-0.98f, 0.80f);
			pCrouchCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pCrouchCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			worldScene.add(pCrouchCmdText_);

			pJumpCmdText_->position_ = Vector2f(-0.98f, 0.72f);
			pJumpCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pJumpCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			worldScene.add(pJumpCmdText_);

			pUsePropCmdText_->position_ = Vector2f(-0.3f, -0.2f);
			pUsePropCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pUsePropCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			pUsePropCmdText_->scale_ = 0.8f;

			pPickPropCmdText_->position_ = Vector2f(0.3f, -0.2f);
			pPickPropCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pPickPropCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			pPickPropCmdText_->scale_ = 0.8f;

			pCheckPropCmdText_->position_ = Vector2f(0.6f, 0.26f);
			pCheckPropCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pCheckPropCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			pCheckPropCmdText_->scale_ = 0.8f;

			pThrowPropCmdText_->position_ = Vector2f(0.6f, 0.20f);
			pThrowPropCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pThrowPropCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			pThrowPropCmdText_->scale_ = 0.8f;

			pRotatePropCmdText_->position_ = Vector2f(-0.3f, 0.0f);
			pRotatePropCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pRotatePropCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			pRotatePropCmdText_->scale_ = 0.8f;

			pGetCloserPropCmdText_->position_ = Vector2f(0.3f, 0.0f);
			pGetCloserPropCmdText_->effectType_ = Text2D::EffectType::DROP_SHADOWS;
			pGetCloserPropCmdText_->effectColor_ = Color(0.0f, 0.0f, 0.0f);
			pGetCloserPropCmdText_->scale_ = 0.8f;
		}

		// Get actor basic configuration
		{
			Configurator& configurator = Configurator::getInstance();

			minHeadFeetDistance_ = std::atof(configurator.getParameterValue("min_head_feet_distance"));
			maxHeadFeetDistance_ = std::atof(configurator.getParameterValue("max_head_feet_distance"));

			maxStandingSpeed_ = std::atof(configurator.getParameterValue("max_standing_speed"));
			maxCrouchedSpeed_ = std::atof(configurator.getParameterValue("max_crouch_speed"));

			maxSpeed_ = maxStandingSpeed_;
			headFeetDistance_ = maxHeadFeetDistance_;
		}
	}
	catch (...)
	{
		if (pMousePointerBillboard_ != nullptr)
		{
			delete pMousePointerBillboard_;
			pMousePointerBillboard_ = nullptr;
		}

		if (pGetCloserPropCmdText_ != nullptr)
		{
			delete pGetCloserPropCmdText_;
			pGetCloserPropCmdText_ = nullptr;
		}

		if (pRotatePropCmdText_ != nullptr)
		{
			delete pRotatePropCmdText_;
			pRotatePropCmdText_ = nullptr;
		}

		if (pThrowPropCmdText_ != nullptr)
		{
			delete pThrowPropCmdText_;
			pThrowPropCmdText_ = nullptr;
		}

		if (pCheckPropCmdText_ != nullptr)
		{
			delete pCheckPropCmdText_;
			pCheckPropCmdText_ = nullptr;
		}

		if (pPickPropCmdText_ != nullptr)
		{
			delete pPickPropCmdText_;
			pPickPropCmdText_ = nullptr;
		}

		if (pUsePropCmdText_ != nullptr)
		{
			delete pUsePropCmdText_;
			pUsePropCmdText_ = nullptr;
		}

		if (pJumpCmdText_ != nullptr)
		{
			delete pJumpCmdText_;
			pJumpCmdText_ = nullptr;
		}

		if (pCrouchCmdText_ != nullptr)
		{
			delete pCrouchCmdText_;
			pCrouchCmdText_ = nullptr;
		}

		if (pMoveCmdText_ != nullptr)
		{
			delete pMoveCmdText_;
			pMoveCmdText_ = nullptr;
		}

		if (pOpenDiaryCmdText_ != nullptr)
		{
			delete pOpenDiaryCmdText_;
			pOpenDiaryCmdText_ = nullptr;
		}

		if (pGeneric6DofConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pGeneric6DofConstraint_);

			delete pGeneric6DofConstraint_;
			pGeneric6DofConstraint_ = nullptr;
		}

		if (pBody_ != nullptr)
		{
			PhysicsEngine::getInstance().removeBody(pBody_);

			delete pBody_;
			pBody_ = nullptr;
		}

		if (pPickingConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pPickingConstraint_);

			delete pPickingConstraint_;
			pPickingConstraint_ = nullptr;

			pPickedProp_ = nullptr;
		}

		if (pCandleConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pCandleConstraint_);

			delete pCandleConstraint_;
			pCandleConstraint_ = nullptr;

			pCandle_ = nullptr;
		}

		throw;
	}
}

Actor::~Actor()
{
	if (pMousePointerBillboard_ != nullptr)
	{
		delete pMousePointerBillboard_;
		pMousePointerBillboard_ = nullptr;
	}

	if (pGetCloserPropCmdText_ != nullptr)
	{
		delete pGetCloserPropCmdText_;
		pGetCloserPropCmdText_ = nullptr;
	}

	if (pRotatePropCmdText_ != nullptr)
	{
		delete pRotatePropCmdText_;
		pRotatePropCmdText_ = nullptr;
	}

	if (pThrowPropCmdText_ != nullptr)
	{
		delete pThrowPropCmdText_;
		pThrowPropCmdText_ = nullptr;
	}

	if (pCheckPropCmdText_ != nullptr)
	{
		delete pCheckPropCmdText_;
		pCheckPropCmdText_ = nullptr;
	}

	if (pPickPropCmdText_ != nullptr)
	{
		delete pPickPropCmdText_;
		pPickPropCmdText_ = nullptr;
	}

	if (pUsePropCmdText_ != nullptr)
	{
		delete pUsePropCmdText_;
		pUsePropCmdText_ = nullptr;
	}

	if (pJumpCmdText_ != nullptr)
	{
		delete pJumpCmdText_;
		pJumpCmdText_ = nullptr;
	}

	if (pCrouchCmdText_ != nullptr)
	{
		delete pCrouchCmdText_;
		pCrouchCmdText_ = nullptr;
	}

	if (pMoveCmdText_ != nullptr)
	{
		delete pMoveCmdText_;
		pMoveCmdText_ = nullptr;
	}

	if (pOpenDiaryCmdText_ != nullptr)
	{
		delete pOpenDiaryCmdText_;
		pOpenDiaryCmdText_ = nullptr;
	}

	if (pGeneric6DofConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pGeneric6DofConstraint_);

		delete pGeneric6DofConstraint_;
		pGeneric6DofConstraint_ = nullptr;
	}

	if (pBody_ != nullptr)
	{
		PhysicsEngine::getInstance().removeBody(pBody_);

		delete pBody_;
		pBody_ = nullptr;
	}

	if (pPickingConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pPickingConstraint_);

		delete pPickingConstraint_;
		pPickingConstraint_ = nullptr;

		pPickedProp_ = nullptr;
	}

	if (pCandleConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pCandleConstraint_);

		delete pCandleConstraint_;
		pCandleConstraint_ = nullptr;

		pCandle_ = nullptr;
	}
}

void Actor::update(float deltaTime)
{
	pCamera_->position_ = pBody_->getPosition();
	pCamera_->position_.y += headFeetDistance_ + cameraWobblingDeltaY_;

	InputManager& inputManager = InputManager::getInstance();
	switch (state_)
	{
	case Actor::State::IDLE:
	{
		Prop* pProp = lookForProp();

		if (inputManager.keyboard_.getKeyDown('w')
			|| inputManager.keyboard_.getKeyDown('s')
			|| inputManager.keyboard_.getKeyDown('a')
			|| inputManager.keyboard_.getKeyDown('d'))
		{
			enterWalkingState();

			walk(deltaTime);
		}
		else if (inputManager.keyboard_.getLastKeyPressed() == 32
			&& !isCrouched_)
		{
			jump();
		}
		else if (inputManager.keyboard_.getLastKeyPressed() == 'g')
		{
			pickDropProp(pProp);
		}
		else if (inputManager.keyboard_.getKeyDown('c'))
		{
			state_ = Actor::State::CROUCHING;

			crouchStand(deltaTime);
		}
		else if (inputManager.keyboard_.getKeyDown('t'))
		{
			throwProp(deltaTime);
		}
		else if (inputManager.keyboard_.getLastKeyPressed() == 'u')
		{
			useProp(pProp);
		}
		else if (pPickedProp_ != nullptr
			&& inputManager.keyboard_.getLastKeyPressed() == 'v')
		{
			enterCheckingPropState();

			checkProp(deltaTime);
		}

		lookAround(deltaTime);
		carryProp(deltaTime);
		carryCandle(deltaTime);
		break;
	}
	case Actor::State::WALKING:
	{
		Prop* pProp = lookForProp();

		if (inputManager.keyboard_.getLastKeyPressed() == 'g')
		{
			pickDropProp(pProp);
		}
		else if (inputManager.keyboard_.getKeyDown('t'))
		{
			throwProp(deltaTime);
		}
		else if (inputManager.keyboard_.getLastKeyPressed() == 'u')
		{
			useProp(pProp);
		}
		else if (inputManager.keyboard_.getLastKeyPressed() == 'c')
		{
			state_ = Actor::State::CROUCHING;

			crouchStand(deltaTime);
		}
		else if (inputManager.keyboard_.getLastKeyPressed() == 32
			&& !isCrouched_)
		{
			jump();
		}

		walk(deltaTime);
		lookAround(deltaTime);
		carryProp(deltaTime);
		carryCandle(deltaTime);
		break;
	}
	case Actor::State::CROUCHING:
	{
		Prop* pProp = lookForProp();

		if (inputManager.keyboard_.getLastKeyPressed() == 'g')
		{
			pickDropProp(pProp);
		}
		else if (inputManager.keyboard_.getKeyDown('t'))
		{
			throwProp(deltaTime);
		}
		else if (inputManager.keyboard_.getLastKeyPressed() == 'u')
		{
			useProp(pProp);
		}

		crouchStand(deltaTime);
		walk(deltaTime);
		lookAround(deltaTime);
		carryProp(deltaTime);
		carryCandle(deltaTime);
		break;
	}
	case Actor::State::CHECKING_PROP:
	{
		if (inputManager.keyboard_.getLastKeyPressed() == 'v')
		{
			exitCheckingPropState();
		}

		checkProp(deltaTime);
		break;
	}
	case Actor::State::JUMPING:
	{
		land();
		lookAround(deltaTime);
		carryProp(deltaTime);
		carryCandle(deltaTime);
		break;
	}
	default:
		break;
	}
}

void Actor::lookAround(float deltaTime)
{
	Vector2f delta = InputManager::getInstance().mouse_.getDelta();

	angleX_ -= delta.y * deltaTime * 0.3f;
	if (angleX_ > 1.55f)
	{
		angleX_ = 1.55f;
	}
	else if (angleX_ < -1.55f)
	{
		angleX_ = -1.55f;
	}

	pCamera_->direction_.y = sin(angleX_);

	rotationY_ = delta.x * deltaTime * 0.3f;
	angleY_ += rotationY_;
	if (angleY_ > 6.28f)
	{
		angleY_ -= 6.26f;
	}
	else if (angleY_ < 0.0f)
	{
		angleY_ += 6.28f;
	}

	forwardDirection_.x = sin(angleY_);
	forwardDirection_.z = -cos(angleY_);

	float c = cos(angleX_);
	pCamera_->direction_.x = forwardDirection_.x * c;
	pCamera_->direction_.z = forwardDirection_.z * c;
}

void Actor::crouchStand(float deltaTime)
{
	if (isCrouched_)
	{
		float deltaY = headFeetDistance_ + deltaTime * 2.0f;
		if (deltaY >= (maxHeadFeetDistance_ - 0.001f))
		{
			headFeetDistance_ = maxHeadFeetDistance_;

			maxSpeed_ = maxStandingSpeed_;

			state_ = Actor::State::WALKING;

			isCrouched_ = false;

			pCrouchCmdText_->setString("C: Crouch", Text2D::Alignment::LEFT);

			pBody_->setLocalScaling(Vector3f(1.0f, 1.0f, 1.0f));

			Renderer::getInstance().scenes_[SceneId::WORLD_SCENE].add(pJumpCmdText_);

			return;
		}

		headFeetDistance_ = deltaY;
	}
	else
	{
		float deltaY = headFeetDistance_ - deltaTime * 2.0f;
		if (deltaY <= (minHeadFeetDistance_ - 0.001f))
		{
			headFeetDistance_ = minHeadFeetDistance_;

			maxSpeed_ = maxCrouchedSpeed_;

			state_ = Actor::State::WALKING;

			isCrouched_ = true;

			pCrouchCmdText_->setString("C: Stand", Text2D::Alignment::LEFT);

			pBody_->setLocalScaling(Vector3f(1.0f, 0.55f, 1.0f));

			Renderer::getInstance().scenes_[SceneId::WORLD_SCENE].remove(pJumpCmdText_);

			return;
		}

		headFeetDistance_ = deltaY;
	}
}

Prop* Actor::lookForProp()
{
	Body* pBody = PhysicsEngine::getInstance().raycast(
		pCamera_->position_ + pCamera_->direction_ * 0.25f,
		pCamera_->direction_,
		2.0f
	);

	if (pBody != nullptr)
	{
		Entity* pEntity = reinterpret_cast<Entity*>(pBody->getUserPointer());
		if (pEntity->getTag() != "Actor")
		{
			Prop* pProp = reinterpret_cast<Prop*>(pEntity);
			if (pProp != pPickedProp_
				&& pProp != pCandle_
				&& (pProp->isPickable() || pProp->isUsable()))
			{
				Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
				worldScene.add(pMousePointerBillboard_);

				if (state_ != Actor::State::CHECKING_PROP)
				{
					if (pProp->isPickable())
					{
						worldScene.add(pPickPropCmdText_);
					}

					if (pProp->isUsable())
					{
						worldScene.add(pUsePropCmdText_);
					}
				}

				return pProp;
			}
		}
	}

	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
	worldScene.remove(pMousePointerBillboard_);

	worldScene.remove(pPickPropCmdText_);
	worldScene.remove(pUsePropCmdText_);

	return nullptr;
}

void Actor::walk(float deltaTime)
{
	bool goForward = InputManager::getInstance().keyboard_.getKeyDown('w');
	bool goBackward = InputManager::getInstance().keyboard_.getKeyDown('s');
	bool goRight = InputManager::getInstance().keyboard_.getKeyDown('d');
	bool goLeft = InputManager::getInstance().keyboard_.getKeyDown('a');

	const Vector3f& velocity = pBody_->getVelocity();
	float speed = velocity.length();

	Vector3f impulse(0.0f, 0.0f, 0.0f);
	Vector3f lateralDirection(forwardDirection_.z, forwardDirection_.y, -forwardDirection_.x);
	float a = pBody_->getMass() * deltaTime * 5.0f;
	float b = a * 0.75f;

	if (!(goForward || goBackward || goRight || goLeft))
	{
		if (speed < 0.03f)
		{
			state_ = state_ == Actor::State::WALKING ? Actor::State::IDLE : state_;

			pBody_->setVelocity(Vector3f(0.0f, 0.0f, 0.0f));

			return;
		}

		impulse -= velocity * b;
	}
	else
	{
		elapsedTime_ += deltaTime;
		if (elapsedTime_ >= 0.5f)
		{
			AudioSystem::getInstance().playSound(pFootstepSound_, 0.5f);

			elapsedTime_ -= 0.5f;
		}

		cameraWobblingDeltaY_ = sinf(elapsedTime_ * 6.28f) * 0.025f;

		if (speed < maxSpeed_)
		{
			if (goForward)
			{
				impulse += forwardDirection_ * a;
			}

			if (goBackward)
			{
				impulse -= forwardDirection_ * a;
			}

			if (goLeft)
			{
				impulse += lateralDirection * a;
			}

			if (goRight)
			{
				impulse -= lateralDirection * a;
			}
		}

		impulse -= velocity * b;
	}

	pBody_->applyImpulse(impulse);
}

void Actor::pickDropProp(Prop* pProp)
{
	if (pProp != nullptr)
	{
		Candle* pCandle = dynamic_cast<Candle*>(pProp);
		if (pCandle != nullptr
			&& pCandle->isOn())
		{
			pickCandle(pCandle);

			return;
		}

		if (pPickedProp_ != nullptr)
		{
			dropProp();
		}

		if (pProp->isPickable())
		{
			pickProp(pProp);
		}

		return;
	}

	if (pPickedProp_ != nullptr)
	{
		dropProp();
	}
}

void Actor::throwProp(float deltaTime)
{
	if (pPickedProp_ != nullptr)
	{
		Body* pPropBody = pPickedProp_->getBody();

		Vector3f impulse = pCamera_->direction_;
		impulse.y += 0.3f;
		impulse.normalize();
		impulse *= pPropBody->getMass() * 5.0f;

		pPropBody->applyImpulse(impulse);

		dropProp();
	}
}

void Actor::useProp(Prop* pProp)
{
	if (pProp != nullptr)
	{
		if (pProp->isUsable())
		{
			Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
			worldScene.remove(pMousePointerBillboard_);

			worldScene.remove(pPickPropCmdText_);
			worldScene.remove(pUsePropCmdText_);

			if (pPickedProp_ != nullptr)
			{
				if (pProp->use(pPickedProp_))
				{
					dropProp();
				}
			}
			else
			{
				pProp->use(nullptr);
			}
		}
	}
}

void Actor::pickProp(Prop* pProp)
{
	pPickedProp_ = pProp;

	Body* pPropBody = pProp->getBody();

	Vector3f direction = pCamera_->direction_;
	direction.x = pCamera_->direction_.x * c1_ + pCamera_->direction_.z * s1_;
	direction.z = pCamera_->direction_.x * -s1_ + pCamera_->direction_.z * c1_;
	Vector3f targetPropPosition = pCamera_->position_ + direction;

	pPickingConstraint_ = new PointConstraint(
		pPropBody,
		targetPropPosition,
		Vector3f(0.0f, 0.0f, 0.0f),
		0.3f
	);
	PhysicsEngine::getInstance().addConstraint(pPickingConstraint_);

	btQuaternion actorRotation(-angleY_, 0.0f, 0.0f);
	propDeltaRotation_ = pPropBody->getRotation() * actorRotation.inverse();

	pBody_->enableCollisionWithBody(pPropBody, false);

	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
	worldScene.add(pCheckPropCmdText_);
	worldScene.add(pThrowPropCmdText_);
}

void Actor::dropProp()
{
	pBody_->enableCollisionWithBody(pPickedProp_->getBody(), true);

	PhysicsEngine::getInstance().removeConstraint(pPickingConstraint_);
	delete pPickingConstraint_;
	pPickingConstraint_ = nullptr;

	pPickedProp_ = nullptr;

	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
	worldScene.remove(pCheckPropCmdText_);
	worldScene.remove(pThrowPropCmdText_);
}

void Actor::checkProp(float deltaTime)
{
	Body* pPropBody = pPickedProp_->getBody();

	Vector2f delta;
	if (InputManager::getInstance().mouse_.getLbDown())
	{
		delta = InputManager::getInstance().mouse_.getDelta();
		delta.x = clamp(-0.05f, 0.05f, delta.x * 0.003f);
		delta.y = clamp(-0.05f, 0.05f, delta.y * 0.003f);
		propDeltaRotation_ *= btQuaternion(delta.x, delta.y, 0.0f);
	}
	else if (InputManager::getInstance().mouse_.getRbDown())
	{
		Vector2f delta = InputManager::getInstance().mouse_.getDelta();

		propDistance_ -= delta.y * deltaTime * 0.1f;
		if (propDistance_ < 0.5f)
		{
			propDistance_ = 0.5f;
		}
		else if (propDistance_ > 1.5f)
		{
			propDistance_ = 1.5f;
		}
	}
	else if (InputManager::getInstance().keyboard_.getLastKeyPressed() == 'u')
	{
		pPickedProp_->use(nullptr);
	}
	else
	{
		lookAround(deltaTime);
		carryCandle(deltaTime);
	}

	Vector3f targetPropPosition = pCamera_->position_ + pCamera_->direction_ * propDistance_;
	pPickingConstraint_->updatePosition(targetPropPosition);

	btQuaternion actorRotation(-angleY_, 0.0f, 0.0f);
	btQuaternion torque = (actorRotation * propDeltaRotation_) * pPropBody->getRotation().inverse();
	Vector3f torqueImpulse;
	QuaternionToEulerXYZ(torque, torqueImpulse);
	pPropBody->setAngularVelocity({ 0.0f,0.0f,0.0f });
	pPropBody->applyTorqueImpulse(torqueImpulse * 0.02f);
}

void Actor::carryProp(float deltaTime)
{
	if (pPickedProp_ != nullptr)
	{
		Body* pPropBody = pPickedProp_->getBody();

		Vector3f direction = pCamera_->direction_;
		direction.x = pCamera_->direction_.x * c1_ + pCamera_->direction_.z * s1_;
		direction.z = pCamera_->direction_.x * -s1_ + pCamera_->direction_.z * c1_;
		Vector3f targetPropPosition = pCamera_->position_ + direction;

		const Vector3f& propPosition = pPropBody->getPosition();
		if ((propPosition - targetPropPosition).length2() > 0.8f)
		{
			dropProp();

			return;
		}

		pPickingConstraint_->updatePosition(targetPropPosition);

		btQuaternion actorRotation(-angleY_, 0.0f, 0.0f);
		btQuaternion torque = (actorRotation * propDeltaRotation_) * pPropBody->getRotation().inverse();
		Vector3f euler;
		QuaternionToEulerXYZ(torque, euler);
		pPropBody->setAngularVelocity({ 0.0f,0.0f,0.0f });
		pPropBody->applyTorqueImpulse(euler * 0.02f);
	}
}

void Actor::pickCandle(Candle* pCandle)
{
	pCandle_ = pCandle;

	Body* pPropBody = pCandle->getBody();

	Vector3f direction = pCamera_->direction_;
	direction.x = pCamera_->direction_.x * c2_ + pCamera_->direction_.z * s2_;
	direction.z = pCamera_->direction_.x * -s2_ + pCamera_->direction_.z * c2_;
	Vector3f targetPropPosition = pCamera_->position_ + direction * 0.5f;

	pCandleConstraint_ = new PointConstraint(
		pPropBody,
		targetPropPosition,
		{ 0.0f,0.0f,0.0f }, 1.0f
	);
	PhysicsEngine::getInstance().addConstraint(pCandleConstraint_);

	pBody_->enableCollisionWithBody(pPropBody, false);
}

void Actor::carryCandle(float deltaTime)
{
	if (pCandle_ != nullptr)
	{
		Body* pPropBody = pCandle_->getBody();

		Vector3f direction = pCamera_->direction_;
		direction.x = pCamera_->direction_.x * c2_ + pCamera_->direction_.z * s2_;
		direction.z = pCamera_->direction_.x * -s2_ + pCamera_->direction_.z * c2_;
		Vector3f targetPropPosition = pCamera_->position_ + direction * 0.5f;
		targetPropPosition.y -= 0.1f;
		const Vector3f& propPosition = pPropBody->getPosition();

		pCandleConstraint_->updatePosition(targetPropPosition);

		const btQuaternion& currentPropRotation(pPropBody->getRotation());
		btQuaternion targetPropRotation(-angleY_, 0.0f, 0.0f);
		btQuaternion deltaRotation(targetPropRotation * currentPropRotation.inverse());
		Vector3f euler;
		QuaternionToEulerXYZ(deltaRotation, euler);
		pPropBody->setAngularVelocity(Vector3f(0.0f, 0.0f, 0.0f));
		pPropBody->applyTorqueImpulse(euler * 0.02f);
	}
}

void Actor::jump()
{
	pBody_->applyImpulse(Vector3f(0.0f, 1.0f, 0.0f) * pBody_->getMass() * 4.0f);

	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
	worldScene.remove(pMousePointerBillboard_);

	worldScene.remove(pPickPropCmdText_);
	worldScene.remove(pUsePropCmdText_);

	state_ = Actor::State::JUMPING;
}

void Actor::land()
{
	if (pBody_->getVelocity().y <= 0.0f)
	{
		Body* pBody = PhysicsEngine::getInstance().raycast(
			pBody_->getPosition(),
			Vector3f(0.0f, -1.0f, 0.0f),
			0.1f
		);

		if (pBody != nullptr)
		{
			state_ = Actor::State::WALKING;
		}
	}
}

void Actor::enterWalkingState()
{
	state_ = Actor::State::WALKING;

	elapsedTime_ = 0.0f;
}

void Actor::enterCheckingPropState()
{
	state_ = Actor::State::CHECKING_PROP;

	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
	worldScene.remove(pMousePointerBillboard_);

	worldScene.add(pRotatePropCmdText_);
	worldScene.add(pGetCloserPropCmdText_);
	worldScene.add(pUsePropCmdText_);
	worldScene.remove(pThrowPropCmdText_);
	worldScene.remove(pPickPropCmdText_);
	worldScene.remove(pMoveCmdText_);
	worldScene.remove(pCrouchCmdText_);
	worldScene.remove(pCheckPropCmdText_);
	worldScene.remove(pJumpCmdText_);

	pUsePropCmdText_->position_ = Vector2f(0.0f, -0.2f);

	c2_ = cosf(0.6f);
	s2_ = sinf(0.6f);

	propDistance_ = 1.0f;
}

void Actor::exitCheckingPropState()
{
	state_ = Actor::State::IDLE;

	btQuaternion actorRotation(-angleY_, 0.0f, 0.0f);
	propDeltaRotation_ = pPickedProp_->getBody()->getRotation() * actorRotation.inverse();

	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
	worldScene.remove(pRotatePropCmdText_);
	worldScene.remove(pGetCloserPropCmdText_);
	worldScene.remove(pUsePropCmdText_);
	worldScene.add(pThrowPropCmdText_);
	worldScene.add(pMoveCmdText_);
	worldScene.add(pCrouchCmdText_);
	worldScene.add(pCheckPropCmdText_);
	worldScene.add(pJumpCmdText_);

	pUsePropCmdText_->position_ = Vector2f(0.6f, 0.4f);

	c2_ = cosf(0.4f);
	s2_ = sinf(0.4f);
}
