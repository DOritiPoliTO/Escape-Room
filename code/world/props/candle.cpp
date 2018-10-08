#include "candle.h"
#include "../../game.h"

Candle::Candle(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
	const Vector3f& rotationAxis, float rotationAngle, const char* entityTag) :
	isOn_(false),
	pPointLight_(nullptr),
	pFireEffect_(nullptr),
	elapsedTime_(0.0f),
	Prop(bodyFilename, modelFilename, true, true, position, rotationAxis, rotationAngle,
		entityTag)
{
	try
	{
		GLfloat diffuse[] = { 0.2f, 0.2f, 0.2f };
		GLfloat specular[]{ 0.6f, 0.6f, 0.6f };
		GLfloat lightPosition[]{ 0.0f, 0.0f, 0.0f };
		pPointLight_ = new PointLight(diffuse, specular, lightPosition);

		pFireEffect_ = new FireEffect("../EscapeRoom/data/textures/flame.png");
	}
	catch (...)
	{
		if (pPointLight_ != nullptr)
		{
			delete pPointLight_;
			pPointLight_ = nullptr;
		}

		if (pFireEffect_ != nullptr)
		{
			delete pFireEffect_;
			pFireEffect_ = nullptr;
		}

		throw;
	}
}

Candle::~Candle()
{
	if (pPointLight_ != nullptr)
	{
		delete pPointLight_;
		pPointLight_ = nullptr;
	}

	if (pFireEffect_ != nullptr)
	{
		delete pFireEffect_;
		pFireEffect_ = nullptr;
	}
}

bool Candle::use(Prop* pOtherProp)
{
	if (pOtherProp == nullptr
		|| pOtherProp->getTag() != "Matchsticks")
	{
		return false;
	}

	Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
	worldScene.add(pFireEffect_->getBillboard());

	worldScene.pPointLight_ = pPointLight_;

	isOn_ = true;

	return false;
}

void Candle::update(float deltaTime)
{
	pShadedModel3D_->position_ = pBody_->getPosition();
	pShadedModel3D_->rotationAxis_ = pBody_->getRotationAxis();
	pShadedModel3D_->angle_ = pBody_->getRotationAngle();

	if (isOn_)
	{
		Vector3f position;
		pBody_->convertFromLocalToWorldSpace(Vector3f(0.0f, 0.13f, 0.0f), position);

		pFireEffect_->getBillboard()->position_ = position;
		pFireEffect_->update(deltaTime);

		elapsedTime_ += deltaTime;
		if (elapsedTime_ > 0.4f)
		{
			elapsedTime_ -= 0.4f;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(48, 128);
			random0_ = (float)dis(gen) / 142.0f;
			random1_ = (float)dis(gen) / 162.0f;
			random2_ = (float)dis(gen) / 162.0f;
		}

		position += pointLightDeltaPosition_;
		pPointLight_->position_[0] = position.x;
		pPointLight_->position_[1] = position.y;
		pPointLight_->position_[2] = position.z;

		float c = deltaTime * 0.3f;

		pointLightDeltaPosition_.lerp(Vector3f(random0_, random1_, random2_) * 0.1f, c);

		pPointLight_->diffuse_[0] += (random0_ - pPointLight_->diffuse_[0]) * c;
		pPointLight_->diffuse_[1] += (random0_ - pPointLight_->diffuse_[1]) * c;
		pPointLight_->diffuse_[2] += (random0_ - pPointLight_->diffuse_[2]) * c;

		float specular = 1.2f * random0_;
		pPointLight_->specular_[0] += (specular - pPointLight_->specular_[0]) * c;
		pPointLight_->specular_[1] += (specular - pPointLight_->specular_[1]) * c;
		pPointLight_->specular_[2] += (specular - pPointLight_->specular_[2]) * c;
	}
}
