#pragma once

#include <new>
#include <exception>
#include "fmod.h"
#include "fmod_studio.h"
#include "fmod_errors.h"
#include "../common/batch_allocator.h"

class Sound
{
	friend class AudioSystem;

public:
	~Sound();

	static void operator delete(void* ptr) noexcept;

private:
	FMOD_SOUND* pFmodSound_;

	Sound();

	static void* operator new(std::size_t count);
};
