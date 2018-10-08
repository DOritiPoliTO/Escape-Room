#include "audio_system.h"

AudioSystem& AudioSystem::getInstance()
{
	static AudioSystem instance(
		std::atoi(Configurator::getInstance().getParameterValue("number_audio_channels"))
	);

	return instance;
}

AudioSystem::AudioSystem(int nChannels) :
	pFmodSystem_(nullptr)
{
	try
	{
		if (FMOD_System_Create(&pFmodSystem_) != FMOD_OK)
		{
			throw std::exception("could not create audio system!");
		}

		if (FMOD_System_Init(pFmodSystem_, nChannels, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
		{
			throw std::exception("could not initialise audio system!");
		}
	}
	catch (...)
	{
		if (pFmodSystem_ != nullptr)
		{
			FMOD_System_Release(pFmodSystem_);
			pFmodSystem_ = nullptr;
		}

		throw;
	}
}

AudioSystem::~AudioSystem()
{
	if (pFmodSystem_ != nullptr)
	{
		FMOD_System_Release(pFmodSystem_);
		pFmodSystem_ = nullptr;
	}
}

void AudioSystem::update()
{
	FMOD_System_Update(pFmodSystem_);
}

Sound* AudioSystem::loadSound(const char* filename, bool looping)
{
	Sound* pSound(nullptr);

	try
	{
		pSound = new Sound();

		FMOD_MODE mode = FMOD_DEFAULT | FMOD_2D;
		mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

		FMOD_System_CreateSound(
			pFmodSystem_,
			filename,
			mode,
			0,
			&pSound->pFmodSound_
		);

		return pSound;
	}
	catch (...)
	{
		if (pSound != nullptr)
		{
			delete pSound;
			pSound = nullptr;
		}

		throw;
	}
}

void AudioSystem::playSound(Sound* pSound, float volume)
{
	FMOD_CHANNEL* pChannel = nullptr;

	FMOD_System_PlaySound(
		AudioSystem::getInstance().pFmodSystem_,
		pSound->pFmodSound_,
		nullptr,
		false,
		&pChannel
	);

	FMOD_Channel_SetVolume(pChannel, volume);
}
