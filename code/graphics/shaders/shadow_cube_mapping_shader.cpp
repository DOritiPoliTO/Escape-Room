#include "shadow_cube_mapping_shader.h"

const float ShadowCubeMappingShader::zNear_ = 0.01f;
const float ShadowCubeMappingShader::zFar_ = 8.01f;

ShadowCubeMappingShader::ShadowCubeMappingShader(GLsizei shadowMapSize) :
	Shader("../EscapeRoom/data/shaders/shadow_cube_mapping_shader.vert",
		"../EscapeRoom/data/shaders/shadow_cube_mapping_shader.frag"),
	shadowMapSize_(shadowMapSize),
	frameBufferId_(-1),
	shadowMapId_(-1)
{
	try
	{
		depthModelViewProjectionMatrixUniformLocation_ = glGetUniformLocation(id_,
			"u_DepthModelViewProjectionMatrix");
		modelMatrixUniformLocation_ = glGetUniformLocation(id_, "u_ModelMatrix");
		lightPosition_WS_UniformLocation_ = glGetUniformLocation(id_, "u_LightPosition_WS");

		// Generate frame buffer and shadow map
		{
			glGenFramebuffers(1, &frameBufferId_);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

			glGenTextures(1, &shadowMapId_);
			glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapId_);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT24, shadowMapSize_,
				shadowMapSize_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT24, shadowMapSize_,
				shadowMapSize_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT24, shadowMapSize_,
				shadowMapSize_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT24, shadowMapSize_,
				shadowMapSize_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT24, shadowMapSize_,
				shadowMapSize_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT24, shadowMapSize_,
				shadowMapSize_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				shadowMapId_, 0);

			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				throw std::exception();
			}
		}
	}
	catch (...)
	{
		if (frameBufferId_ != -1)
		{
			glDeleteFramebuffers(1, &frameBufferId_);
			frameBufferId_ = -1;
		}

		if (shadowMapId_ != -1)
		{
			glDeleteTextures(1, &shadowMapId_);
			shadowMapId_ = -1;
		}

		if (id_ != 0)
		{
			glDetachShader(id_, vertexShaderId_);
			glDetachShader(id_, fragmentShaderId_);

			glDeleteProgram(id_);
			id_ = 0;
		}

		if (vertexShaderId_ != 0)
		{
			glDeleteShader(vertexShaderId_);
			vertexShaderId_ = 0;
		}

		if (fragmentShaderId_ != 0)
		{
			glDeleteShader(fragmentShaderId_);
			fragmentShaderId_ = 0;
		}

		throw;
	}
}

ShadowCubeMappingShader::~ShadowCubeMappingShader()
{
	if (frameBufferId_ != -1)
	{
		glDeleteFramebuffers(1, &frameBufferId_);
		frameBufferId_ = -1;
	}

	if (shadowMapId_ != -1)
	{
		glDeleteTextures(1, &shadowMapId_);
		shadowMapId_ = -1;
	}

	if (id_ != 0)
	{
		glDetachShader(id_, vertexShaderId_);
		glDetachShader(id_, fragmentShaderId_);

		glDeleteProgram(id_);
		id_ = 0;
	}

	if (vertexShaderId_ != 0)
	{
		glDeleteShader(vertexShaderId_);
		vertexShaderId_ = 0;
	}

	if (fragmentShaderId_ != 0)
	{
		glDeleteShader(fragmentShaderId_);
		fragmentShaderId_ = 0;
	}
}

void ShadowCubeMappingShader::execute(const Vector3f& cameraPosition, const Vector3f& cameraDirection,
	const std::vector<ShadedModel3D*>& pShadedModel3Ds, const PointLight* pPointLight)
{
	glUseProgram(id_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);
	glViewport(0, 0, shadowMapSize_, shadowMapSize_);

	glUniform3fv(lightPosition_WS_UniformLocation_, 1, pPointLight->position_);

	Matrix44f projectionMatrix;
	projectionMatrix.setPerspective(90.0f, 1.0f, zNear_, zFar_);

	const Vector3f layerDirections[]{
		{ 1.0f,0.0f,0.0f },
		{ -1.0f,0.0f,0.0f },
		{ 0.0f,1.0f,0.0f },
		{ 0.0f,-1.0f,0.0f },
		{ 0.0f,0.0f,1.0f },
		{ 0.0f,0.0f,-1.0f }
	};

	const Vector3d upDirections[]{
		{ 0.0,-1.0,0.0 },
		{ 0.0,-1.0,0.0 },
		{ 0.0,0.0,1.0 },
		{ 0.0,0.0,-1.0 },
		{ 0.0,-1.0,0.0 },
		{ 0.0,-1.0,0.0 }
	};

	Vector3f eye(pPointLight->position_[0], pPointLight->position_[1], pPointLight->position_[2]);
	for (size_t i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			shadowMapId_, 0);

		glClear(GL_DEPTH_BUFFER_BIT);

		Vector3f center = eye + layerDirections[i];
		Matrix44f viewMatrix;
		viewMatrix.setLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z,
			upDirections[i].x, upDirections[i].y, upDirections[i].z);

		for (size_t j = 0; j < pShadedModel3Ds.size(); j++)
		{
			const Vector3f& position = pShadedModel3Ds[j]->position_;
			const Vector3f& rotationAxis = pShadedModel3Ds[j]->rotationAxis_;
			GLfloat angle = pShadedModel3Ds[j]->angle_;

			Matrix44f modelMatrix = Matrix44f().setTranslation(position.x, position.y, position.z)
				* Matrix44f().setRotation(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z)
				* Matrix44f().setScale(pShadedModel3Ds[j]->scale_);
			glUniformMatrix4fv(modelMatrixUniformLocation_, 1, GL_FALSE, &modelMatrix.m00);

			Matrix44f modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
			glUniformMatrix4fv(depthModelViewProjectionMatrixUniformLocation_, 1, GL_FALSE,
				&modelViewProjectionMatrix.m00);

			draw(pShadedModel3Ds[j]);
		}
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

GLuint ShadowCubeMappingShader::getShadowMapId() const
{
	return shadowMapId_;
}

void ShadowCubeMappingShader::draw(const ShadedModel3D* pShadedModel3D)
{
	auto* pShadedModel3DData = pShadedModel3D->getData();

	// Bind buffer with vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, pShadedModel3DData->vertexBuffer.getId());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ShadedModel3D::VertexAttribs),
		(void*)offsetof(struct ShadedModel3D::VertexAttribs, position)
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pShadedModel3DData->indexBuffer.getId());

	glDrawElements(GL_TRIANGLES, pShadedModel3DData->indexBuffer.getSize() / 4, GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(0);
}
