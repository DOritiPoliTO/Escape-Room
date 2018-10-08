#pragma once

#include <fstream>
#include "texture.h"
#include "buffer.h"
#include "../common/batch_allocator.h"
#include "../common/vector2.h"

class Font
{
public:
	struct CharacterInfo
	{
		Vector2f texCoord;	// uv of top left corner
		float deltaU;
		float deltaV;
		float width;
		float height;
		Vector2f offset;		// position of top left corner in relation to the virtual cursor
		float advance;
	};

	Font(const char* fontFilename);

	const Texture& getTexture() const;
	const CharacterInfo* getCharacterInfo() const;
	float getMeanCharacterWidth() const;
	float getLineHeight() const;

	static void* operator new(std::size_t count);
	static void operator delete(void* ptr) noexcept;

private:
	Texture texture_;
	CharacterInfo characterInfo_[256];
	float meanCharacterWidth_;
	float lineHeight_;

	void readFntFile(const char* fontFilename);
	void loadTexture(const char* fontFilename);
	void processData();
};
