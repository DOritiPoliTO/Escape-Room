#pragma once

#include "../common/vector2.h"

struct Mouse
{
public:
	Mouse();

	void updateDelta(const Vector2i& delta);
	void updateLb(bool isDown);
	void updateRb(bool isDown);
	bool getLbDown() const;
	bool getRbDown() const;
	const Vector2i& getDelta() const;

private:
	bool isLbDown;
	bool isRbDown;
	Vector2i delta;
};
