#pragma once

struct Keyboard
{
public:
	Keyboard();

	void clear();

	void setKeyState(int key, bool isDown);

	bool getKeyDown(int key) const;
	int getLastKeyPressed();

private:
	bool keys[256];
	int lastKeyPressed;
};
