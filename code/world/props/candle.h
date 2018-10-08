#pragma once

#include <random>
#include "../prop.h"
#include "../../graphics/point_light.h"
#include "../fire_effect.h"

class Candle : public Prop
{
public:
	Candle(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle, const char* entityTag = "");
	~Candle() override;

	bool use(Prop* pOtherProp) override;

	void update(float deltaTime) override;

	bool isOn() const { return isOn_; }

private:
	bool isOn_;
	PointLight* pPointLight_;
	FireEffect* pFireEffect_;
	Vector3f pointLightDeltaPosition_;
	float elapsedTime_;
	float random0_;
	float random1_;
	float random2_;
};
