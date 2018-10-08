#pragma once

#include "../prop.h"
#include "../../physics/constraints/hinge_constraint.h"
#include "../../graphics/billboards/billboard_2d.h"

class SafeLid :public Prop
{
public:
	SafeLid(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle, Prop* pSafe);
	~SafeLid();

	bool use(Prop* pOtherProp) override;

	void update(float deltaTime) override;

private:
	enum State;

	State state_;
	HingeConstraint* pHingeConstraint_;
	float elapsedTime_;
	float timeInterval_;
	Billboard2D* pSageImageBillboard_;

	bool keypadCallback(const int* numbers);
	void videoCallback();
};

enum SafeLid::State
{
	LOCKED,
	UNLOCKED,
	OPEN
};
