#include "font.h"
#include "text_2d.h"

Font::Font(const char* fontFilename) :
	texture_()
{
	for (size_t i = 0; i < 256; i++)
	{
		characterInfo_[i].deltaU = 0.0f;
		characterInfo_[i].deltaV = 0.0f;
		characterInfo_[i].texCoord = Vector2f(0.0f, 0.0f);

		characterInfo_[i].height = 0.0f;
		characterInfo_[i].width = 0.0f;
		characterInfo_[i].advance = 0.0f;
		characterInfo_[i].offset = Vector2f(0.0f, 0.0f);
	}

	readFntFile(fontFilename);

	loadTexture(fontFilename);

	processData();
}

const Texture& Font::getTexture() const
{
	return texture_;
}

const Font::CharacterInfo* Font::getCharacterInfo() const
{
	return characterInfo_;
}

float Font::getMeanCharacterWidth() const
{
	return meanCharacterWidth_;
}

float Font::getLineHeight() const
{
	return lineHeight_;
}

void* Font::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<Font>();
}

void Font::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<Font>(ptr);
}

void Font::readFntFile(const char* fntFilename)
{
	std::ifstream file;
	int charsCount;
	int charCode;

	try
	{
		file.open(fntFilename);
		if (!file.is_open())
		{
			throw std::exception("Could not open font file");
		}

		char buffer[512];
		file.getline(buffer, 512);
		file.getline(buffer, 512);
		file.getline(buffer, 512);

		file.getline(buffer, 512, '=');
		file >> charsCount;
		for (int i = 0; i < charsCount; i++)
		{
			// Read char ASCII code
			file.getline(buffer, 512, '=');
			file >> charCode;

			// Read char u texture coord
			file.getline(buffer, 512, '=');
			file >> characterInfo_[charCode].texCoord.x;

			// Read char v texture coord
			file.getline(buffer, 512, '=');
			file >> characterInfo_[charCode].texCoord.y;

			// Read char delta u
			file.getline(buffer, 512, '=');
			file >> characterInfo_[charCode].deltaU;

			// Read char delta v
			file.getline(buffer, 512, '=');
			file >> characterInfo_[charCode].deltaV;

			// Read char x offset
			file.getline(buffer, 512, '=');
			file >> characterInfo_[charCode].offset.x;

			// Read char y offset
			file.getline(buffer, 512, '=');
			file >> characterInfo_[charCode].offset.y;

			// Read char x advance
			file.getline(buffer, 512, '=');
			file >> characterInfo_[charCode].advance;

			// Read remaining unused fields
			file.getline(buffer, 512);
		}

		file.close();
	}
	catch (...)
	{
		if (file.is_open())
		{
			file.close();
		}

		throw;
	}
}

void Font::loadTexture(const char* fontFilename)
{
	char textureFilename[128];
	int i = 0;
	do
	{
		textureFilename[i] = fontFilename[i];

		i++;

	} while (fontFilename[i] != '\0');

	textureFilename[i - 3] = 'p';
	textureFilename[i - 2] = 'n';
	textureFilename[i - 1] = 'g';
	textureFilename[i] = '\0';

	texture_.write(textureFilename, false);
}

void Font::processData()
{
	meanCharacterWidth_ = 0.0f;
	lineHeight_ = 0.0f;
	float c1 = 0.04f / characterInfo_['a'].deltaU;
	float c2 = 1.0f / static_cast<float>(texture_.getWidth());
	int count = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (characterInfo_[i].deltaV == 0.0f)
		{
			continue;
		}

		characterInfo_[i].height = characterInfo_[i].deltaV * c1;
		characterInfo_[i].width = characterInfo_[i].deltaU * c1;
		characterInfo_[i].advance *= c1;
		characterInfo_[i].offset *= c1;

		characterInfo_[i].deltaU *= c2;
		characterInfo_[i].deltaV *= c2;
		characterInfo_[i].texCoord *= c2;

		characterInfo_[i].offset.y *= -1.0f;
		characterInfo_[i].texCoord.y = 1.0f - characterInfo_[i].texCoord.y;

		meanCharacterWidth_ += characterInfo_[i].width;
		lineHeight_ = std::fmax(lineHeight_, characterInfo_[i].height);

		count++;
	}

	meanCharacterWidth_ /= count;
}
