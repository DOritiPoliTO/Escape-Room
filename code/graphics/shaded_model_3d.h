#pragma once

#include <vector>
#include "texture.h"
#include "buffer.h"
#include "../common/vector2.h"
#include "../common/vector3.h"
#include "../common/utility.h"
#include "../common/batch_allocator.h"

class ShadedModel3D
{
public:
	struct VertexAttribs;
	struct Material;
	struct TriangleGroup;;
	struct Data;

	Vector3f position_;
	Vector3f rotationAxis_;
	float angle_;
	float scale_;

	ShadedModel3D(const char* filename);
	~ShadedModel3D();

	const Data* getData() const;

	static void* operator new(std::size_t count);
	static void operator delete(void* ptr) noexcept;

protected:
	Data* pData_;
};

struct ShadedModel3D::Data
{
	std::vector<TriangleGroup> triangleGroups;
	Buffer vertexBuffer;
	Buffer indexBuffer;
	float boundingSphereRadius;

	Data(const char* filename);

	static void* operator new(std::size_t count);
	static void operator delete(void* ptr) noexcept;

private:
	void readMdlFile(const char* filename);
	void readFbxFile(const char* filename);
};

struct ShadedModel3D::VertexAttribs
{
	Vector3f position;
	Vector2f texCoords;
	Vector3f normal;
	Vector3f tangent;
	Vector3f bitangent;
};

struct ShadedModel3D::Material
{
	Texture* pNormalMap;
	Texture* pDiffuseColorMap;
	Texture* pSpecularMap;
	GLfloat reflectivity;

	Material();
	Material(Material&& other);
	Material(const char* normalMapFilename, const char* diffuseColorMapFilename,
		const char* specularMapFilename, GLfloat reflectivity);
	~Material();
	Material& operator=(Material&& other);
};

struct ShadedModel3D::TriangleGroup
{
	Material material;
	GLsizei indexBufferOffset;
	GLsizei numberOfTriangles;

	TriangleGroup();
	TriangleGroup(TriangleGroup&& other);
	TriangleGroup(const char* normalMapFilename, const char* diffuseColorMapFilename,
		const char* specularMapFilename, GLfloat materialReflectivity, GLsizei indexBufferOffset,
		GLsizei numberOfTriangles);
	TriangleGroup& operator=(TriangleGroup&& other);
};
