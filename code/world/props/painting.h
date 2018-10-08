#pragma once

#include "../prop.h"
#include "../../physics/constraints/point_constraint.h"
#include "../../physics/physics_engine.h"

class Painting :public Prop
{
public:
	Painting(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle, float pinRelativeHeight);
	~Painting();

private:
	PointConstraint* pPointConstraint_;
};
