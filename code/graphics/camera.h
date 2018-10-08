#pragma once

#include "../common/vector3.h"

class Camera
{
public:
	Vector3f position_;
	Vector3f direction_;

	Camera();
};

inline Camera::Camera() :
	position_(0.0f, 0.0f, 0.0f),
	direction_(0.0f, 0.0f, 1.0f)
{
}
