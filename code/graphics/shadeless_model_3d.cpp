#include "shadeless_model_3d.h"

ShadelessModel3D::ShadelessModel3D(const char* smdlFilename) :
	position_(0.0f, 0.0f, 0.0f),
	rotationAxis_(0.0f, 1.0f, 0.0f),
	angle_(0.0f),
	colorScale_(1.0f, 1.0f, 1.0f, 1.0f),
	pTexture_(nullptr),
	vertexBuffer_(GL_ARRAY_BUFFER),
	indexBuffer_(GL_ELEMENT_ARRAY_BUFFER)
{
	std::ifstream file;
	std::string buffer;
	std::string directoryPath(smdlFilename);
	std::vector<VertexAttribs> vertices;
	std::vector<GLuint> indices;

	try
	{
		extractDirectoryPath(directoryPath);
		size_t offset = directoryPath.find("/models/");
		directoryPath.replace(offset, 10, "/textures/");

		file.open(smdlFilename);
		if (!file.is_open())
		{
			throw std::exception();
		}

		file >> buffer;
		if (buffer != "texture")
		{
			throw std::exception();
		}
		file >> buffer;
		pTexture_ = new Texture((directoryPath + buffer).data(), true);

		file >> buffer;
		if (buffer != "number_of_vertices")
		{
			throw std::exception();
		}
		size_t numberOfVertices;
		file >> numberOfVertices;
		vertices.resize(numberOfVertices);
		for (size_t i = 0; i < numberOfVertices; i++)
		{
			file >> vertices[i].position >> vertices[i].texCoords;
		}

		file >> buffer;
		if (buffer != "number_of_indices")
		{
			throw std::exception();
		}
		size_t numberOfIndices;
		file >> numberOfIndices;
		indices.resize(numberOfIndices);
		for (size_t i = 0; i < numberOfIndices; i += 3)
		{
			file >> indices[i] >> indices[i + 1] >> indices[i + 2];
		}

		vertexBuffer_.write(GL_ARRAY_BUFFER, vertices.data(), vertices.size() * sizeof(VertexAttribs),
			GL_STATIC_DRAW);

		indexBuffer_.write(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint),
			GL_STATIC_DRAW);

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

ShadelessModel3D::~ShadelessModel3D()
{
	if (pTexture_ != nullptr)
	{
		if (AssetManager::getInstance().releaseAsset<Texture>(pTexture_))
		{
			delete pTexture_;
		}
		pTexture_ = nullptr;
	}
}

const Texture* ShadelessModel3D::getTexture() const
{
	return pTexture_;
}

const Buffer& ShadelessModel3D::getVertexBuffer() const
{
	return vertexBuffer_;
}

const Buffer& ShadelessModel3D::getIndexBuffer() const
{
	return indexBuffer_;
}

void* ShadelessModel3D::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<ShadelessModel3D>();
}

void ShadelessModel3D::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<ShadelessModel3D>(ptr);
}
