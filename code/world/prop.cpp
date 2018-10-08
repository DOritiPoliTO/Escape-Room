#include "prop.h"
#include "../game.h"

Prop::Prop(const char* bodyFilename, const char* modelFilename, bool isPickable,
	bool isUsable, const Vector3f& position, const Vector3f& rotationAxis,
	float rotationAngle, const char* entityTag) :
	Entity(entityTag),
	pBody_(nullptr),
	pShadedModel3D_(nullptr),
	isPickable_(isPickable),
	isUsable_(isUsable)
{
	try
	{
		pBody_ = new Body(bodyFilename, this, position, rotationAxis, rotationAngle);
		PhysicsEngine::getInstance().addBody(pBody_);

		pShadedModel3D_ = new ShadedModel3D(modelFilename);
		Renderer::getInstance().scenes_[SceneId::WORLD_SCENE].add(pShadedModel3D_);
	}
	catch (...)
	{
		if (pShadedModel3D_ != nullptr)
		{
			delete pShadedModel3D_;
			pShadedModel3D_ = nullptr;
		}

		if (pBody_ != nullptr)
		{
			PhysicsEngine::getInstance().removeBody(pBody_);

			delete pBody_;
			pBody_ = nullptr;
		}

		throw;
	}
}

Prop::~Prop()
{
	if (pShadedModel3D_ != nullptr)
	{
		delete pShadedModel3D_;
		pShadedModel3D_ = nullptr;
	}

	if (pBody_ != nullptr)
	{
		PhysicsEngine::getInstance().removeBody(pBody_);

		delete pBody_;
		pBody_ = nullptr;
	}
}

bool Prop::use(Prop* pOtherProp)
{
	return false;
}

void Prop::update(float deltaTime)
{
	pShadedModel3D_->position_ = pBody_->getPosition();
	pShadedModel3D_->rotationAxis_ = pBody_->getRotationAxis();
	pShadedModel3D_->angle_ = pBody_->getRotationAngle();
}

bool Prop::isPickable() const
{
	return isPickable_;
}

bool Prop::isUsable() const
{
	return isUsable_;
}

ShadedModel3D* Prop::getG3DModel()
{
	return pShadedModel3D_;
}

Body* Prop::getBody()
{
	return pBody_;
}
