#pragma once

class FpsCounter
{
public:
	FpsCounter(float timeInterval);

	void update(float elapsedTime);

	float getFps() const;

private:
	unsigned int frameCount_;
	float elapsedTime_;
	float timeInterval_;
	float fps_;
};
