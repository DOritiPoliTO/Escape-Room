#include "sound.h"
#include "audio_system.h"

Sound::~Sound()
{
	if (pFmodSound_ != nullptr)
	{
		FMOD_Sound_Release(pFmodSound_);
		pFmodSound_ = nullptr;
	}
}

void Sound::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<Sound>(ptr);
}

Sound::Sound() :
	pFmodSound_(nullptr)
{}

void* Sound::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<Sound>();
}
