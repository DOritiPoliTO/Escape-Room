#pragma once

#include "../prop.h"
#include "../../physics/constraints/fixed_constraint.h"
#include "../../physics/physics_engine.h"

class Drawer :public Prop
{
public:
	Drawer(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle, Prop* pDesk);
	~Drawer();

	bool use(Prop* pOtherProp) override;

	void update(float deltaTime) override;

private:
	FixedConstraint* pFixedConstraint_;
	bool isOpen_;
	bool isOpening_;
	bool isClosing_;
	float elapsedTime_;
	float timeInterval_;
};
