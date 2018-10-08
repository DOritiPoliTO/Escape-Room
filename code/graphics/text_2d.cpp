#include "text_2d.h"
#include "font.h"

Text2D::Text2D(const char* fontFilename) :
	position_(0.0f, 0.0f),
	color_(1.0f, 1.0f, 1.0f, 1.0f),
	scale_(1.0f),
	effectType_(EffectType::NONE),
	effectScale_(1.0f),
	effectColor_(1.0f, 1.0f, 1.0f, 1.0f),
	vertexBuffer_(GL_ARRAY_BUFFER),
	pFont_(nullptr)
{
	try
	{
		AssetManager& assetManager = AssetManager::getInstance();

		pFont_ = assetManager.acquireAsset<Font>(fontFilename);
		if (pFont_ == nullptr)
		{
			pFont_ = new Font(fontFilename);

			assetManager.insertAsset<Font>(fontFilename, pFont_);
		}
	}
	catch (...)
	{
		if (pFont_ != nullptr)
		{
			if (AssetManager::getInstance().releaseAsset<Font>(pFont_))
			{
				delete pFont_;
			}
			pFont_ = nullptr;
		}

		throw;
	}
}

Text2D::Text2D(const char* string, Text2D::Alignment alignment, const char* fontFilename) :
	Text2D(fontFilename)
{
	CharacterQuad* characterQuads = nullptr;

	try
	{
		size_t size = std::fmax(std::strlen(string), vertexBuffer_.getSize() / sizeof(CharacterQuad));

		characterQuads = new CharacterQuad[size];

		computeVertices(string, pFont_, alignment, characterQuads);

		vertexBuffer_.write(GL_ARRAY_BUFFER, characterQuads, size * sizeof(CharacterQuad), GL_STATIC_DRAW);

		delete[] characterQuads;
	}
	catch (...)
	{
		if (characterQuads != nullptr)
		{
			delete[] characterQuads;
		}

		throw;
	}
}

Text2D::~Text2D()
{
	if (pFont_ != nullptr)
	{
		if (AssetManager::getInstance().releaseAsset<Font>(pFont_))
		{
			delete pFont_;
		}
		pFont_ = nullptr;
	}
}

void Text2D::setString(const char* string, Text2D::Alignment alignment)
{
	CharacterQuad* characterQuads = nullptr;
	CharacterQuad buffer[64];

	try
	{
		size_t size = std::fmax(std::strlen(string), vertexBuffer_.getSize() / sizeof(CharacterQuad));
		if (size > 64)
		{
			characterQuads = new CharacterQuad[size];

			computeVertices(string, pFont_, alignment, characterQuads);

			vertexBuffer_.write(GL_ARRAY_BUFFER, characterQuads, size * sizeof(CharacterQuad), GL_DYNAMIC_DRAW);

			delete[] characterQuads;
		}
		else
		{
			computeVertices(string, pFont_, alignment, buffer);

			vertexBuffer_.write(GL_ARRAY_BUFFER, buffer, size * sizeof(CharacterQuad), GL_DYNAMIC_DRAW);
		}
	}
	catch (...)
	{
		if (characterQuads != nullptr)
		{
			delete[] characterQuads;
		}

		throw;
	}
}

void Text2D::setString(float number, const char* format, Text2D::Alignment alignment)
{
	char buffer[64];
	sprintf_s(buffer, format, number);

	setString(buffer, alignment);
}

const Buffer& Text2D::getVertexBuffer() const
{
	return vertexBuffer_;
}

const Font* Text2D::getFont() const
{
	return pFont_;
}

void* Text2D::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<Text2D>();
}

void Text2D::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<Text2D>(ptr);
}

void Text2D::computeVertices(const char* string, const Font* pFont, Alignment alignment,
	CharacterQuad* characterQuads)
{
	float alignmentOffset = -computeAlignmentOffset(string, pFont, alignment);

	const Font::CharacterInfo* charsInfo = pFont->getCharacterInfo();
	Vector2f cursorPosition = Vector2f(alignmentOffset, 0.0f);
	int i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == ' ')
		{
			cursorPosition.x += pFont->getMeanCharacterWidth();

			i++;

			continue;
		}

		if (string[i] == '\n')
		{
			cursorPosition.x = alignmentOffset;
			cursorPosition.y -= pFont->getLineHeight();

			i++;

			continue;
		}

		// Top left vertex texCoord...
		characterQuads[i].topLeftVertex.texCoord = charsInfo[string[i]].texCoord;
		// ...and position
		characterQuads[i].topLeftVertex.position = cursorPosition + charsInfo[string[i]].offset;

		// Top right vertex texCoord...
		characterQuads[i].topRightVertex.texCoord = characterQuads[i].topLeftVertex.texCoord;
		characterQuads[i].topRightVertex.texCoord.x += charsInfo[string[i]].deltaU;
		// ...and position
		characterQuads[i].topRightVertex.position = characterQuads[i].topLeftVertex.position;
		characterQuads[i].topRightVertex.position.x += charsInfo[string[i]].width;

		// Bottom right vertex texCoord...
		characterQuads[i].bottomRightVertex.texCoord = characterQuads[i].topRightVertex.texCoord;
		characterQuads[i].bottomRightVertex.texCoord.y -= charsInfo[string[i]].deltaV;
		// ...and position
		characterQuads[i].bottomRightVertex.position = characterQuads[i].topRightVertex.position;
		characterQuads[i].bottomRightVertex.position.y -= charsInfo[string[i]].height;

		// Bottom left vertex texCoord...
		characterQuads[i].bottomLeftVertex.texCoord = characterQuads[i].bottomRightVertex.texCoord;
		characterQuads[i].bottomLeftVertex.texCoord.x -= charsInfo[string[i]].deltaU;
		// ...and position
		characterQuads[i].bottomLeftVertex.position = characterQuads[i].bottomRightVertex.position;
		characterQuads[i].bottomLeftVertex.position.x -= charsInfo[string[i]].width;

		cursorPosition.x += charsInfo[string[i]].advance;

		i++;
	}
}

float Text2D::computeMaxLineLength(const char* string, const Font* pFont)
{
	const Font::CharacterInfo* charsInfo = pFont->getCharacterInfo();
	float lineLength = 0.0f;
	int i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == ' ')
		{
			lineLength += pFont->getMeanCharacterWidth();

			i++;

			continue;
		}

		if (string[i] == '\n')
		{
			return std::fmax(lineLength, computeMaxLineLength(&string[i + 1], pFont));
		}

		lineLength += charsInfo[string[i]].advance;

		i++;
	}

	return lineLength;
}

float Text2D::computeAlignmentOffset(const char* string, const Font* pFont,
	Alignment alignment)
{
	if (alignment == Text2D::Alignment::LEFT)
	{
		return 0.0f;
	}

	float maxLineLength = computeMaxLineLength(string, pFont);

	if (alignment == Text2D::Alignment::CENTER)
	{
		return maxLineLength / 2.0f;
	}

	if (alignment == Text2D::Alignment::RIGHT)
	{
		return maxLineLength;
	}

	return 0.0f;
}
