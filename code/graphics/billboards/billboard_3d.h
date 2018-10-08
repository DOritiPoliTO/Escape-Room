#pragma once

#include "../billboard.h"
#include "../../common/vector3.h"
#include "../../common/batch_allocator.h"

class Billboard3D :public Billboard
{
public:
	Vector3f position_;

	Billboard3D(const char* imageFilename, const GLfloat* vertices, const GLfloat* texCoords);
	Billboard3D(const char* imageFilename, float billboardWidth);
	Billboard3D(const char* imageFilename, float billboardWidth, float billboardHeight);
	Billboard3D(GLsizei textureWidth, GLsizei textureHeight, float billboardWidth, float billboardHeight,
		bool flipVertically, bool flipHorizontally);

	static void* operator new(std::size_t count);
	static void operator delete(void* ptr) noexcept;
};
