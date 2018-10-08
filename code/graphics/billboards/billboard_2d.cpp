#include "billboard_2d.h"

Billboard2D::Billboard2D(const char* imageFilename, const GLfloat* vertices, const GLfloat* texCoords) :
	Billboard(imageFilename, vertices, texCoords)
{}

Billboard2D::Billboard2D(const char* imageFilename, float billboardWidth) :
	Billboard(imageFilename, billboardWidth)
{}

Billboard2D::Billboard2D(const char* imageFilename, float billboardWidth, float billboardHeight) :
	Billboard(imageFilename, billboardWidth, billboardHeight)
{}

Billboard2D::Billboard2D(GLsizei textureWidth, GLsizei textureHeight, float billboardWidth, float billboardHeight,
	bool flipVertically, bool flipHorizontally) :
	Billboard(textureWidth, textureHeight, billboardWidth, billboardHeight, flipVertically,
		flipHorizontally)
{}

void* Billboard2D::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<Billboard2D>();
}

void Billboard2D::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<Billboard2D>(ptr);
}
