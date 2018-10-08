#include "fps_counter.h"

FpsCounter::FpsCounter(float timeInterval) :
	frameCount_(0),
	elapsedTime_(0.0f),
	timeInterval_(timeInterval)
{
}

void FpsCounter::update(float deltaTime)
{
	frameCount_++;

	elapsedTime_ += deltaTime;

	if (elapsedTime_ >= timeInterval_)
	{
		fps_ = static_cast<float>(frameCount_) / elapsedTime_;
		elapsedTime_ = 0.0f;
		frameCount_ = 0;
	}
}

float FpsCounter::getFps() const
{
	return fps_;
}
