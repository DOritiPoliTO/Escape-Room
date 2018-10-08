#pragma once

#include <math.h>
#include <iostream>
#include "../common/vector2.h"
#include "../physics/constraints/point_constraint.h"
#include "../physics/constraints/generic_6_dof_constraint.h"
#include "prop.h"
#include "props/candle.h"
#include "../graphics/renderer.h"
#include "../audio/audio_system.h"

class Actor :public Entity
{
public:
	Actor(const char* bodyFilename);
	~Actor();

	void update(float deltaTime);

private:
	enum State;

	State state_;
	Body* pBody_;
	Generic6DofConstraint* pGeneric6DofConstraint_;
	Camera* pCamera_;
	float cameraWobblingDeltaY_;
	Vector3f forwardDirection_;
	float angleX_;
	float angleY_;
	float rotationY_;
	float headFeetDistance_;
	float minHeadFeetDistance_;
	float maxHeadFeetDistance_;
	float maxSpeed_;
	float maxStandingSpeed_;
	float maxCrouchedSpeed_;
	float c1_;
	float s1_;
	float propDistance_;
	btQuaternion propDeltaRotation_;
	bool isCrouched_;
	Candle* pCandle_;
	Prop* pPickedProp_;
	PointConstraint* pPickingConstraint_;
	PointConstraint* pCandleConstraint_;
	float c2_;
	float s2_;
	Billboard2D* pMousePointerBillboard_;
	Text2D* pJumpCmdText_;
	Text2D* pCrouchCmdText_;
	Text2D* pMoveCmdText_;
	Text2D* pOpenDiaryCmdText_;
	Text2D* pUsePropCmdText_;
	Text2D* pPickPropCmdText_;
	Text2D* pCheckPropCmdText_;
	Text2D* pThrowPropCmdText_;
	Text2D* pRotatePropCmdText_;
	Text2D* pGetCloserPropCmdText_;
	Sound* pFootstepSound_;
	float elapsedTime_;

	void lookAround(float deltaTime);
	void crouchStand(float deltaTime);
	Prop* lookForProp();
	void walk(float deltaTime);
	void pickDropProp(Prop* pProp);
	void throwProp(float deltaTime);
	void useProp(Prop* pProp);
	void pickProp(Prop* pProp);
	void dropProp();
	void carryProp(float deltaTime);
	void checkProp(float deltaTime);
	void pickCandle(Candle* pCandle);
	void carryCandle(float deltaTime);
	void jump();
	void land();
	void enterWalkingState();
	void enterCheckingPropState();
	void exitCheckingPropState();
};

enum Actor::State
{
	IDLE,
	WALKING,
	CROUCHING,
	CHECKING_PROP,
	JUMPING
};
