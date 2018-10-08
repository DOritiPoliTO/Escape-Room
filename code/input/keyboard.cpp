#include "keyboard.h"

Keyboard::Keyboard() :
	lastKeyPressed(-1)
{
	for (size_t i = 0; i < 256; i++)
	{
		keys[i] = false;
	}
}

void Keyboard::clear() 
{ 
	lastKeyPressed = -1; 
}

void Keyboard::setKeyState(int key, bool isDown)
{
	keys[key] = isDown;

	if (!isDown)
	{
		lastKeyPressed = key;
	}
}

bool Keyboard::getKeyDown(int key) const 
{ 
	return this->keys[key];
}

int Keyboard::getLastKeyPressed() 
{ 
	return lastKeyPressed; 
}
