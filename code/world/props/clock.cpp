#include "clock.h"
#include "../../game.h"

Clock::Clock(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
	const Vector3f& rotationAxis, float rotationAngle, Prop* pClockKey) :
	Prop(bodyFilename, modelFilename, false, true, position, rotationAxis, rotationAngle),
	key_flag_(false),
	used_flag_(false),
	pDiaryEntry_(nullptr),
	pFixedConstraint_(nullptr)
{
	try
	{
		pDiaryEntry_ = new Diary::Entry("../EscapeRoom/data/models/clock.mdl",
			"After having been charged, the clock started working.\nThe clock hand went to number 4,\nthen got back to 12"
		);
		pDiaryEntry_->getShadedModel3D()->position_.y = -0.057f;
		pDiaryEntry_->getShadedModel3D()->scale_ = 0.17f;

		pFixedConstraint_ = new FixedConstraint(
			pBody_,
			pClockKey->getBody(),
			Vector3f(0.25f, 0.8f, 0.04f),
			btQuaternion({ 0.0f, 1.0f, 0.0f }, 0.0f),
			Vector3f(0.0f, 0.0f, 0.0f),
			btQuaternion({ 0.0f, 0.0f, 1.0f }, -1.57f)
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

Clock::~Clock()
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

bool Clock::use(Prop* pOtherProp)
{
	if (pOtherProp != nullptr
		&& pOtherProp->getTag() == "ClockKey")
	{
		pFixedConstraint_->enable(true);
		key_flag_ = true;
		pOtherProp->getBody()->enableCollisionWithBody(pBody_, false);
		return true;
	}

	if (key_flag_ && !used_flag_)
	{
		Game::getInstance().playVideo(
			"../EscapeRoom/data/videos/clock_animation.mkv",
			std::bind(&Clock::onVideoEnd, this)
		);

		used_flag_ = true;
	}

	return false;
}

void Clock::onVideoEnd()
{
	Game::getInstance().addEntryInDiary(pDiaryEntry_);
}
