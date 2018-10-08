#pragma once

#include "../prop.h"
#include "../../interface/diary.h"
#include "../../physics/physics_engine.h"

class Clock :public Prop
{
public:
	Clock(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle, Prop* pClockKey);
	~Clock();

	bool use(Prop* pOtherProp) override;

private:
	bool key_flag_;
	bool used_flag_;

	Diary::Entry* pDiaryEntry_;

	FixedConstraint* pFixedConstraint_;

	void onVideoEnd();
};
