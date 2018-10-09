#pragma once

#include "../prop.h"
#include "../../physics/constraints/hinge_constraint.h"
#include "../../physics/constraints/fixed_constraint.h"
#include "../../physics/physics_engine.h"
#include "../../audio/audio_system.h"

class Door :public Prop
{
public:
	Door(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle, Prop* pDoorFrame, Prop* pKey);
	~Door();

	bool use(Prop* pOtherProp) override;

	void update(float deltaTime) override;

private:
	enum State;

	State state_;
	HingeConstraint* pHingeConstraint_;
	FixedConstraint* pFixedConstraint_;
	Sound* pUnlockSound_;
	Sound* pSqueakSound_;
	Sound* pLockedSound_;
	float elapsedTime_;
};

enum Door::State
{
	LOCKED,
	OPEN,
	CLOSE,
	OPENING,
	CLOSING
};
