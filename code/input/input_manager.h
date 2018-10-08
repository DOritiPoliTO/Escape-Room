#pragma once

#include "keyboard.h"
#include "mouse.h"

class InputManager
{
public:
	Keyboard keyboard_;
	Mouse mouse_;

	inline static InputManager& getInstance();

private:
	InputManager() {}
	InputManager(const InputManager&) {}
};

InputManager& InputManager::getInstance()
{
	static InputManager instance;
	return instance;
}
