#include "billboard.h"

Texture* Billboard::getTexture()
{
	return pTexture_;
}

Buffer& Billboard::getVertexBuffer()
{
	return vertexBuffer_;
}

Buffer& Billboard::getTexCoordBuffer()
{
	return texCoordBuffer_;
}

Billboard::Billboard(const char* imageFilename, const GLfloat* vertices, const GLfloat* texCoords) :
	colorScale_(1.0f, 1.0f, 1.0f, 1.0f),
	pTexture_(nullptr),
	vertexBuffer_(GL_ARRAY_BUFFER),
	texCoordBuffer_(GL_ARRAY_BUFFER)
{
	try
	{
		pTexture_ = new Texture(imageFilename, false);

		vertexBuffer_.write(GL_ARRAY_BUFFER, vertices, 4 * 3 * sizeof(GLfloat), GL_STATIC_DRAW);

		texCoordBuffer_.write(GL_ARRAY_BUFFER, texCoords, 4 * 2 * sizeof(GLfloat), GL_STATIC_DRAW);
	}
	catch (...)
	{
		if (pTexture_ != nullptr)
		{
			delete pTexture_;
			pTexture_ = nullptr;
		}

		throw;
	}
}

Billboard::Billboard(const char* imageFilename, float billboardWidth) :
	colorScale_(1.0f, 1.0f, 1.0f, 1.0f),
	pTexture_(nullptr),
	vertexBuffer_(GL_ARRAY_BUFFER),
	texCoordBuffer_(GL_ARRAY_BUFFER)
{
	try
	{
		pTexture_ = new Texture(imageFilename, false);

		float billboardHeight = billboardWidth * (float)pTexture_->getHeight() / (float)pTexture_->getWidth();
		GLfloat vertices[] = {
			-billboardWidth,-billboardHeight,0.0f,
			billboardWidth,-billboardHeight,0.0f,
			billboardWidth,billboardHeight,0.0f,
			-billboardWidth,billboardHeight,0.0f
		};
		vertexBuffer_.write(GL_ARRAY_BUFFER, vertices, 4 * 3 * sizeof(GLfloat), GL_STATIC_DRAW);

		GLfloat texCoords[] = {
			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f
		};
		texCoordBuffer_.write(GL_ARRAY_BUFFER, texCoords, 4 * 2 * sizeof(GLfloat), GL_STATIC_DRAW);
	}
	catch (...)
	{
		if (pTexture_ != nullptr)
		{
			delete pTexture_;
		}

		throw;
	}
}

Billboard::Billboard(const char* imageFilename, float billboardWidth, float billboardHeight) :
	colorScale_(1.0f, 1.0f, 1.0f, 1.0f),
	pTexture_(nullptr),
	vertexBuffer_(GL_ARRAY_BUFFER),
	texCoordBuffer_(GL_ARRAY_BUFFER)
{
	try
	{
		pTexture_ = new Texture(imageFilename, false);

		GLfloat vertices[] = {
			-billboardWidth,-billboardHeight,0.0f,
			billboardWidth,-billboardHeight,0.0f,
			billboardWidth,billboardHeight,0.0f,
			-billboardWidth,billboardHeight,0.0f
		};
		vertexBuffer_.write(GL_ARRAY_BUFFER, vertices, 4 * 3 * sizeof(GLfloat), GL_STATIC_DRAW);

		GLfloat texCoords[] = {
			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f
		};
		texCoordBuffer_.write(GL_ARRAY_BUFFER, texCoords, 4 * 2 * sizeof(GLfloat), GL_STATIC_DRAW);
	}
	catch (...)
	{
		if (pTexture_ != nullptr)
		{
			delete pTexture_;
		}

		throw;
	}
}

Billboard::Billboard(GLsizei textureWidth, GLsizei textureHeight, float billboardWidth,
	float billboardHeight, bool flipVertically, bool flipHorizontally) :
	colorScale_(1.0f, 1.0f, 1.0f, 1.0f),
	pTexture_(nullptr),
	vertexBuffer_(GL_ARRAY_BUFFER),
	texCoordBuffer_(GL_ARRAY_BUFFER)
{
	try
	{
		pTexture_ = new Texture(textureWidth, textureHeight, GL_RGB, nullptr, false);

		// Create buffer containing vertex positions
		GLfloat vertices[] = {
			-billboardWidth,-billboardHeight,0.0f,
			billboardWidth,-billboardHeight,0.0f,
			billboardWidth,billboardHeight,0.0f,
			-billboardWidth,billboardHeight,0.0f
		};
		vertexBuffer_.write(GL_ARRAY_BUFFER, vertices, 4 * 3 * sizeof(GLfloat), GL_STATIC_DRAW);

		// Create buffer containing vertex texCoords
		GLfloat texCoords[] = {
			0.0f,0.0f,	// uv of bottom left vertex
			1.0f,0.0f,	// uv of bottom right vertex
			1.0f,1.0f,	// uv of top right vertex
			0.0f,1.0f	// uv of top left vertex
		};

		if (flipVertically)
		{
			// invert uvs of bottom left vertex and top left vertex
			float t0 = texCoords[0];
			float t1 = texCoords[1];
			texCoords[0] = texCoords[6];
			texCoords[1] = texCoords[7];
			texCoords[6] = t0;
			texCoords[7] = t1;

			// invert uvs of bottom right vertex and top right vertex
			t0 = texCoords[2];
			t1 = texCoords[3];
			texCoords[2] = texCoords[4];
			texCoords[3] = texCoords[5];
			texCoords[4] = t0;
			texCoords[5] = t1;
		}

		if (flipHorizontally)
		{
			// invert uvs of bottom left vertex and bottom right vertex
			float t0 = texCoords[0];
			float t1 = texCoords[1];
			texCoords[0] = texCoords[2];
			texCoords[1] = texCoords[3];
			texCoords[2] = t0;
			texCoords[3] = t1;

			// invert uvs of top left vertex and top right vertex
			t0 = texCoords[4];
			t1 = texCoords[5];
			texCoords[4] = texCoords[6];
			texCoords[5] = texCoords[7];
			texCoords[6] = t0;
			texCoords[7] = t1;
		}

		texCoordBuffer_.write(GL_ARRAY_BUFFER, texCoords, 4 * 2 * sizeof(GLfloat), GL_STATIC_DRAW);
	}
	catch (...)
	{
		if (pTexture_ != nullptr)
		{
			delete pTexture_;
		}

		throw;
	}
}

Billboard::~Billboard()
{
	if (pTexture_ != nullptr)
	{
		delete pTexture_;
		pTexture_ = nullptr;
	}
}
