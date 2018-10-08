#pragma once

#include <functional>
#include <math.h>
#include "entity.h"
#include "../graphics/scene.h"
#include "../graphics/shaded_model_3d.h"
#include "../physics/body.h"

class Prop :public Entity
{
public:
	Prop(const char* bodyFilename, const char* modelFilename, bool isPickable, bool isUsable,
		const Vector3f& position, const Vector3f& rotationAxis, float rotationAngle,
		const char* entityTag = "");
	virtual ~Prop();

	virtual bool use(Prop* pOtherProp);

	virtual void update(float deltaTime);

	bool isPickable() const;
	bool isUsable() const;
	ShadedModel3D* getG3DModel();
	Body* getBody();

protected:
	ShadedModel3D* pShadedModel3D_;
	Body* pBody_;
	bool isPickable_;
	bool isUsable_;
};
