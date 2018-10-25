#pragma once

#include "sound.h"
#include "../common/configurator.h"

class AudioSystem
{
	friend class Sound;

public:
	static AudioSystem& getInstance();

	void update();
	Sound* loadSound(const char* filename, bool looping);
	void playSound(Sound* pSound, float volume);

private:
	FMOD_SYSTEM* pFmodSystem_;

	AudioSystem(int nChannels);
	AudioSystem(const AudioSystem&) = delete;
	~AudioSystem();
};
