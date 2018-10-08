#pragma once

#include "../prop.h"
#include "../../interface/diary.h"
#include "../../physics/physics_engine.h"

class Gramophone :public Prop
{
public:
	Gramophone(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle, Prop* pDisk);
	~Gramophone();

	bool use(Prop* pOtherProp) override;

private:
	bool key_flag_;
	bool used_flag_;

	Diary::Entry* pDiaryEntry_;

	FixedConstraint* pFixedConstraint_;

	void onVideoEnd();
};
