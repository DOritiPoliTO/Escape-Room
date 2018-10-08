#include "gramophone.h"
#include "../../game.h"

Gramophone::Gramophone(const char* bodyFilename, const char* modelFilename,
	const Vector3f& position, const Vector3f& rotationAxis, float rotationAngle,
	Prop* pDisk) :
	Prop(bodyFilename, modelFilename, false, true, position, rotationAxis, rotationAngle),
	key_flag_(false),
	used_flag_(false),
	pDiaryEntry_(nullptr),
	pFixedConstraint_(nullptr)
{
	try
	{
		pDiaryEntry_ = new Diary::Entry(
			"../EscapeRoom/data/models/gramophone.mdl",
			"The disk played on the gramophone\nreproduced the following notes:\nSi Re Sol Do Mi Do Fa Re La Do Fa"
		);
		pDiaryEntry_->getShadedModel3D()->position_.y = -0.05f;
		pDiaryEntry_->getShadedModel3D()->scale_ = 0.2f;

		pFixedConstraint_ = new FixedConstraint(
			pBody_,
			pDisk->getBody(),
			Vector3f(0.0f, 0.18f, 0.0f),
			btQuaternion({ 0.0f, 1.0f, 0.0f }, 0.0f),
			Vector3f(0.0f, 0.0f, 0.0f),
			btQuaternion({ 0.0f, 1.0f, 0.0f }, 0.0f)
		);
		PhysicsEngine::getInstance().addConstraint(pFixedConstraint_);
		pFixedConstraint_->enable(false);
	}
	catch (...)
	{
		if (pFixedConstraint_ != nullptr)
		{
			PhysicsEngine::getInstance().removeConstraint(pFixedConstraint_);

			delete pFixedConstraint_;
			pFixedConstraint_ = nullptr;
		}

		if (pDiaryEntry_ != nullptr)
		{
			delete pDiaryEntry_;
			pDiaryEntry_ = nullptr;
		}

		throw;
	}
}

Gramophone::~Gramophone()
{
	if (pFixedConstraint_ != nullptr)
	{
		PhysicsEngine::getInstance().removeConstraint(pFixedConstraint_);

		delete pFixedConstraint_;
		pFixedConstraint_ = nullptr;
	}

	if (pDiaryEntry_ != nullptr)
	{
		delete pDiaryEntry_;
		pDiaryEntry_ = nullptr;
	}
}

bool Gramophone::use(Prop* pOtherProp)
{
	if (pOtherProp != nullptr
		&& pOtherProp->getTag() == "Disk")
	{
		pFixedConstraint_->enable(true);
		key_flag_ = true;
		pOtherProp->getBody()->enableCollisionWithBody(pBody_, false);
		return true;
	}

	if (key_flag_ && !used_flag_)
	{
		Game::getInstance().playVideo(
			"../EscapeRoom/data/videos/gramophone_animation.mkv",
			std::bind(&Gramophone::onVideoEnd, this)
		);

		used_flag_ = true;
	}

	return false;
}

void Gramophone::onVideoEnd()
{
	Game::getInstance().addEntryInDiary(pDiaryEntry_);
}
