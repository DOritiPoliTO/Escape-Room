#include "shaded_model_3d.h"

ShadedModel3D::ShadedModel3D(const char* filename) :
	position_(0.0f, 0.0f, 0.0f),
	rotationAxis_(0.0f, 1.0f, 0.0f),
	angle_(0.0f),
	scale_(1.0f),
	pData_(nullptr)
{
	try
	{
		AssetManager& assetMananger = AssetManager::getInstance();

		pData_ = assetMananger.acquireAsset<ShadedModel3D::Data>(filename);
		if (pData_ == nullptr)
		{
			pData_ = new ShadedModel3D::Data(filename);
			assetMananger.insertAsset<ShadedModel3D::Data>(filename, pData_);
		}
	}
	catch (...)
	{
		if (pData_ != nullptr)
		{
			delete pData_;
			pData_ = nullptr;
		}

		throw;
	}
}

ShadedModel3D::~ShadedModel3D()
{
	if (pData_ != nullptr)
	{
		if (AssetManager::getInstance().releaseAsset<ShadedModel3D::Data>(pData_))
		{
			delete pData_;
			pData_ = nullptr;
		}
	}
}

const ShadedModel3D::Data* ShadedModel3D::getData() const
{
	return pData_;
}

void* ShadedModel3D::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<ShadedModel3D>();
}

void ShadedModel3D::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<ShadedModel3D>(ptr);
}

ShadedModel3D::Data::Data(const char* filename) :
	vertexBuffer(GL_ARRAY_BUFFER),
	indexBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
	if (strstr(filename, ".mdl") != nullptr)
	{
		readMdlFile(filename);
	}
	else if (strstr(filename, ".fbx") != nullptr)
	{
		readFbxFile(filename);
	}
	else
	{
		throw std::exception("3d model file format not supported!");
	}
}

void* ShadedModel3D::Data::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<ShadedModel3D::Data>();
}

void ShadedModel3D::Data::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<ShadedModel3D::Data>(ptr);
}

void ShadedModel3D::Data::readMdlFile(const char* filename)
{
	std::ifstream file;
	std::string field;
	std::string normalMapFilename;
	std::string diffuseColorMapFilename;
	std::string specularMapFilename;
	size_t numberOfMeshes;
	size_t numberOfVertices;
	std::vector<VertexAttribs> vertices;
	size_t numberOfIndices;
	std::vector<GLuint> indices;
	GLuint index;
	GLfloat materialReflectivity;

	try
	{
		std::string directoryPath(filename);
		extractDirectoryPath(directoryPath);
		size_t offset = directoryPath.find("/models/");
		directoryPath.replace(offset, 10, "/textures/");

		file.open(filename);
		if (!file.is_open())
		{
			throw std::exception();
		}

		file >> field >> boundingSphereRadius;
		if (!file.good()
			|| field != "bounding_sphere_radius")
		{
			throw std::exception("An error has occurred while reading a mdl file");
		}

		file >> field >> numberOfMeshes;
		if (!file.good()
			|| field != "number_of_objects")
		{
			throw std::exception("An error has occurred while reading a mdl file");
		}

		triangleGroups.resize(numberOfMeshes);
		triangleGroups.shrink_to_fit();

		vertices.resize(65536);
		indices.resize(65536);

		GLsizei indexOffset = 0;
		GLsizei vertexOffset = 0;
		for (size_t i = 0; i < numberOfMeshes; i++)
		{
			// Get textures
			{
				// Get normal map filename and load it into memory
				file >> field >> normalMapFilename;
				if (!file.good()
					|| field != "normal_map")
				{
					throw std::exception("An error has occurred while reading a mdl file");
				}
				normalMapFilename = directoryPath + normalMapFilename;

				// Get diffuse color map filename and load it into memory
				file >> field >> diffuseColorMapFilename;
				if (!file.good()
					|| field != "diffuse_map")
				{
					throw std::exception("An error has occurred while reading a mdl file");
				}
				diffuseColorMapFilename = directoryPath + diffuseColorMapFilename;

				// Get specular map filename and load it into memory
				file >> field >> specularMapFilename;
				if (!file.good()
					|| field != "specular_map")
				{
					throw std::exception("An error has occurred while reading a mdl file");
				}
				specularMapFilename = directoryPath + specularMapFilename;
			}

			// Get material reflectivity
			file >> field >> materialReflectivity;
			if (!file.good()
				|| field != "reflectivity")
			{
				throw std::exception("An error has occurred while reading a mdl file");
			}

			// Get vertices
			{
				file >> field >> numberOfVertices;
				if (!file.good()
					|| field != "number_of_vertices")
				{
					throw std::exception("An error has occurred while reading a mdl file");
				}

				for (size_t j = vertexOffset; j < numberOfVertices + vertexOffset; j++)
				{
					file >> vertices[j].position
						>> vertices[j].texCoords
						>> vertices[j].normal
						>> vertices[j].tangent
						>> vertices[j].bitangent;
				}
			}

			// Get indices
			{
				file >> field >> numberOfIndices;
				if (!file.good()
					|| field != "number_of_indices")
				{
					throw std::exception("An error has occurred while reading a mdl file");
				}

				for (size_t j = indexOffset; j < numberOfIndices + indexOffset; j += 3)
				{
					file >> index;
					indices[j] = index + vertexOffset;

					file >> index;
					indices[j + 1] = index + vertexOffset;

					file >> index;
					indices[j + 2] = index + vertexOffset;
				}
			}

			TriangleGroup triangleGroup(normalMapFilename.data(), diffuseColorMapFilename.data(),
				specularMapFilename.data(), materialReflectivity, indexOffset, numberOfIndices / 3);

			triangleGroups[i] = std::move(triangleGroup);

			indexOffset += numberOfIndices;
			vertexOffset += numberOfVertices;
		}

		vertexBuffer.write(GL_ARRAY_BUFFER, vertices.data(), vertexOffset * sizeof(VertexAttribs),
			GL_STATIC_DRAW);

		indexBuffer.write(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indexOffset * sizeof(GLuint),
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

void ShadedModel3D::Data::readFbxFile(const char* filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(
		filename,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
	);

	std::string directoryPath(filename);
	extractDirectoryPath(directoryPath);
	size_t offset = directoryPath.find("/models/");
	directoryPath.replace(offset, 10, "/textures/");

	size_t numberOfMeshes = pScene->mNumMeshes;

	boundingSphereRadius = 200.0f;

	triangleGroups.resize(numberOfMeshes);
	triangleGroups.shrink_to_fit();

	std::vector<VertexAttribs> vertices(65536);
	std::vector<GLuint> indices(65536);

	GLsizei indexOffset = 0;
	GLsizei vertexOffset = 0;
	size_t numberOfVertices;
	size_t numberOfFaces;
	aiString diffuseImagePath;
	aiString normalImagePath;
	aiString specularImagePath;
	auto meshes = pScene->mMeshes;
	auto materials = pScene->mMaterials;
	for (size_t i = 0; i < numberOfMeshes; i++)
	{
		auto mesh = meshes[i];

		// Get textures
		{
			int materialIndex = mesh->mMaterialIndex;
			auto material = materials[materialIndex];

			if (material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &diffuseImagePath) == aiReturn_FAILURE)
			{
				diffuseImagePath.Set("default_diffuse.png");
			}

			if (material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &normalImagePath) == aiReturn_FAILURE)
			{
				normalImagePath.Set("default_normal.png");
			}

			if (material->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &specularImagePath) == aiReturn_FAILURE)
			{
				specularImagePath.Set("default_specular.png");
			}

			diffuseImagePath.Set(directoryPath + diffuseImagePath.C_Str());
			normalImagePath.Set(directoryPath + normalImagePath.C_Str());
			specularImagePath.Set(directoryPath + specularImagePath.C_Str());
		}

		// Get material reflectivity
		float materialReflectivity = 0.0f;

		// Get vertices
		{
			if (!mesh->HasTangentsAndBitangents())
			{
				throw std::exception("mesh has no tangent base!");
			}

			numberOfVertices = mesh->mNumVertices;
			for (size_t j = 0; j < numberOfVertices; j++)
			{
				vertices[vertexOffset].position = mesh->mVertices[j];
				vertices[vertexOffset].texCoords = mesh->mTextureCoords[0][j];
				vertices[vertexOffset].normal = mesh->mNormals[j];
				vertices[vertexOffset].tangent = mesh->mTangents[j];
				vertices[vertexOffset].bitangent = mesh->mBitangents[j];

				vertexOffset++;
			}
		}

		// Get indices
		{
			numberOfFaces = mesh->mNumFaces;
			for (size_t j = 0; j < numberOfFaces; j++)
			{
				indices[indexOffset++] = mesh->mFaces[j].mIndices[0];
				indices[indexOffset++] = mesh->mFaces[j].mIndices[1];
				indices[indexOffset++] = mesh->mFaces[j].mIndices[2];
			}
		}

		TriangleGroup triangleGroup(normalImagePath.C_Str(), diffuseImagePath.C_Str(),
			specularImagePath.C_Str(), materialReflectivity, 0, numberOfFaces);

		triangleGroups[i] = std::move(triangleGroup);
	}

	vertexBuffer.write(GL_ARRAY_BUFFER, vertices.data(), vertexOffset * sizeof(VertexAttribs),
		GL_STATIC_DRAW);

	indexBuffer.write(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indexOffset * sizeof(GLuint),
		GL_STATIC_DRAW);
}

ShadedModel3D::Material::Material() :
	pNormalMap(nullptr),
	pDiffuseColorMap(nullptr),
	pSpecularMap(nullptr)
{
}

ShadedModel3D::Material::Material(Material&& other) :
	pNormalMap(other.pNormalMap),
	pDiffuseColorMap(other.pDiffuseColorMap),
	pSpecularMap(other.pSpecularMap),
	reflectivity(other.reflectivity)
{
	other.pNormalMap = nullptr;
	other.pDiffuseColorMap = nullptr;
	other.pSpecularMap = nullptr;
}

ShadedModel3D::Material::Material(const char* normalMapFilename, const char* diffuseColorMapFilename, const char* specularMapFilename,
	GLfloat reflectivity) :
	pNormalMap(nullptr),
	pDiffuseColorMap(nullptr),
	pSpecularMap(nullptr),
	reflectivity(reflectivity)
{
	try
	{
		AssetManager& assetManager = AssetManager::getInstance();

		this->pNormalMap = assetManager.acquireAsset<Texture>(normalMapFilename);
		if (this->pNormalMap == nullptr)
		{
			this->pNormalMap = new Texture(normalMapFilename, true);

			assetManager.insertAsset<Texture>(normalMapFilename, this->pNormalMap);
		}

		this->pDiffuseColorMap = assetManager.acquireAsset<Texture>(diffuseColorMapFilename);
		if (this->pDiffuseColorMap == nullptr)
		{
			this->pDiffuseColorMap = new Texture(diffuseColorMapFilename, true);

			assetManager.insertAsset<Texture>(diffuseColorMapFilename, this->pDiffuseColorMap);
		}

		this->pSpecularMap = assetManager.acquireAsset<Texture>(specularMapFilename);
		if (this->pSpecularMap == nullptr)
		{
			this->pSpecularMap = new Texture(specularMapFilename, true);

			assetManager.insertAsset<Texture>(specularMapFilename, this->pSpecularMap);
		}
	}
	catch (...)
	{
		this->~Material();

		throw;
	}
}

ShadedModel3D::Material::~Material()
{
	AssetManager& assetManager = AssetManager::getInstance();

	if (this->pSpecularMap != nullptr)
	{
		if (assetManager.releaseAsset<Texture>(this->pSpecularMap))
		{
			delete this->pSpecularMap;
		}
		this->pSpecularMap = nullptr;
	}

	if (this->pDiffuseColorMap != nullptr)
	{
		if (assetManager.releaseAsset<Texture>(this->pDiffuseColorMap))
		{
			delete this->pDiffuseColorMap;
		}
		this->pDiffuseColorMap = nullptr;
	}

	if (this->pNormalMap != nullptr)
	{
		if (assetManager.releaseAsset<Texture>(this->pNormalMap))
		{
			delete this->pNormalMap;
		}
		this->pNormalMap = nullptr;
	}
}

ShadedModel3D::Material& ShadedModel3D::Material::operator=(Material&& other)
{
	this->pNormalMap = other.pNormalMap;
	this->pDiffuseColorMap = other.pDiffuseColorMap;
	this->pSpecularMap = other.pSpecularMap;
	this->reflectivity = other.reflectivity;

	other.pNormalMap = nullptr;
	other.pDiffuseColorMap = nullptr;
	other.pSpecularMap = nullptr;

	return *this;
}

ShadedModel3D::TriangleGroup::TriangleGroup() :
	material()
{
}

ShadedModel3D::TriangleGroup::TriangleGroup(TriangleGroup&& other) :
	material(std::move(other.material)),
	indexBufferOffset(other.indexBufferOffset),
	numberOfTriangles(other.numberOfTriangles)
{
}

ShadedModel3D::TriangleGroup::TriangleGroup(const char* normalMapFilename,
	const char* diffuseColorMapFilename, const char* specularMapFilename, GLfloat materialReflectivity,
	GLsizei indexBufferOffset, GLsizei numberOfTriangles) :
	material(normalMapFilename, diffuseColorMapFilename, specularMapFilename, materialReflectivity),
	indexBufferOffset(indexBufferOffset),
	numberOfTriangles(numberOfTriangles)
{
}

ShadedModel3D::TriangleGroup& ShadedModel3D::TriangleGroup::operator=(TriangleGroup&& other)
{
	this->material = std::move(other.material);
	this->indexBufferOffset = other.indexBufferOffset;
	this->numberOfTriangles = other.numberOfTriangles;

	return *this;
}
