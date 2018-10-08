#pragma once

#include "../common/vector2.h"
#include "color.h"
#include "font.h"
#include "../common/batch_allocator.h"

class Text2D
{
public:
	enum EffectType;
	enum Alignment;
	struct VertexAttribs;
	struct CharacterQuad;

	Vector2f position_;
	Color color_;
	GLfloat scale_;
	EffectType effectType_;
	GLfloat effectScale_;
	Color effectColor_;

	Text2D(const char* fontFilename);
	Text2D(const char* string, Text2D::Alignment alignment, const char* fontFilename);
	~Text2D();

	void setString(const char* string, Text2D::Alignment alignment);
	void setString(float number, const char* format, Text2D::Alignment alignment);

	const Buffer& getVertexBuffer() const;
	const Font* getFont() const;

	static void* operator new(std::size_t count);
	static void operator delete(void* ptr) noexcept;

private:
	Buffer vertexBuffer_;
	Font* pFont_;

	static void computeVertices(const char* string, const Font* pFont, Alignment alignment,
		CharacterQuad* characterQuads);
	static float computeMaxLineLength(const char* string, const Font* pFont);
	static float computeAlignmentOffset(const char* string, const Font* pFont, Alignment alignment);
};

enum Text2D::EffectType
{
	NONE = 0,
	OUTLINE = 1,
	GLOW = 2,
	DROP_SHADOWS = 3
};

enum Text2D::Alignment
{
	LEFT,
	CENTER,
	RIGHT
};

struct Text2D::VertexAttribs
{
	Vector2f position;
	Vector2f texCoord;
};

struct Text2D::CharacterQuad
{
	VertexAttribs topLeftVertex;
	VertexAttribs topRightVertex;
	VertexAttribs bottomRightVertex;
	VertexAttribs bottomLeftVertex;
};
