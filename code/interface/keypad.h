#pragma once

#include <functional>
#include "../graphics/renderer.h"
#include "../audio/audio_system.h"

class Keypad
{
public:
	Keypad();
	~Keypad();

	bool update(float deltaTime);
	void open(int nTot, std::function<bool(const int*)> callback);

private:
	char string_[8];
	int numbers_[4];

	int nInsert_;
	int nTot_;
	float elapsedTime_;
	std::function<bool(const int*)> callback_;

	Text2D* pCombinationText_;
	Text2D* pUndoCmdText_;
	Text2D* pConfirmCmdText_;
	Sound* pFailSound_;
	Sound* pSuccessSound_;
};
