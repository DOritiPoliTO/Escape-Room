#pragma once

#include "texture.h"
#include "buffer.h"
#include "color.h"

class Billboard
{
public:
	Color colorScale_;

	Texture* getTexture();
	Buffer& getVertexBuffer();
	Buffer& getTexCoordBuffer();

protected:
	Texture* pTexture_;
	Buffer vertexBuffer_;
	Buffer texCoordBuffer_;

	Billboard(const char* imageFilename, const GLfloat* vertices, const GLfloat* texCoords);
	Billboard(const char* imageFilename, float billboardWidth);
	Billboard(const char* imageFilename, float billboardWidth, float billboardHeight);
	Billboard(GLsizei textureWidth, GLsizei textureHeight, float billboardWidth, float billboardHeight,
		bool flipVertically = false, bool flipHorizontally = false);
	~Billboard();
};
