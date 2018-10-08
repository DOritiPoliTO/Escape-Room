#include "mouse.h"

Mouse::Mouse() :
	isLbDown(false),
	isRbDown(false),
	delta(0, 0)
{}

void Mouse::updateDelta(const Vector2i& delta)
{
	this->delta = delta;
}

void Mouse::updateLb(bool isDown)
{
	isLbDown = isDown;
}

void Mouse::updateRb(bool isDown)
{
	isRbDown = isDown;
}

bool Mouse::getLbDown() const
{
	return isLbDown;
}

bool Mouse::getRbDown() const
{
	return isRbDown;
}

const Vector2i& Mouse::getDelta() const
{
	return delta;
}
