#include "shadow_mapping_shader.h"

const float ShadowMappingShader::radii[4] = { 4.0f,8.0f,16.0f,32.0f };

ShadowMappingShader::ShadowMappingShader(GLsizei shadowMapSize) :
	Shader("../EscapeRoom/data/shaders/shadow_mapping_shader.vert",
		"../EscapeRoom/data/shaders/shadow_mapping_shader.frag"),
	shadowMapSize_(shadowMapSize)
{
	frameBufferIds_[0] = -1;
	frameBufferIds_[1] = -1;
	frameBufferIds_[2] = -1;
	frameBufferIds_[3] = -1;
	shadowMapIds_[0] = -1;
	shadowMapIds_[1] = -1;
	shadowMapIds_[2] = -1;
	shadowMapIds_[3] = -1;

	try
	{
		depthModelViewProjectionMatrixUniformLocation_ = glGetUniformLocation(id_,
			"u_DepthModelViewProjectionMatrix");

		depthModelViewProjectionMatrices_.resize(128);

		for (GLuint i = 0; i < 4; i++)
		{
			glGenFramebuffers(1, &frameBufferIds_[i]);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferIds_[i]);

			glGenTextures(1, &shadowMapIds_[i]);

			glBindTexture(GL_TEXTURE_2D, shadowMapIds_[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowMapSize_, shadowMapSize_, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapIds_[i], 0);

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
		for (size_t i = 0; i < 4; i++)
		{
			if (frameBufferIds_[i] != -1)
			{
				glDeleteFramebuffers(1, &frameBufferIds_[i]);
				frameBufferIds_[i] = -1;
			}

			if (shadowMapIds_[i] != -1)
			{
				glDeleteTextures(1, &shadowMapIds_[i]);
				shadowMapIds_[i] = -1;
			}
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

ShadowMappingShader::~ShadowMappingShader()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (frameBufferIds_[i] != -1)
		{
			glDeleteFramebuffers(1, &frameBufferIds_[i]);
			frameBufferIds_[i] = -1;
		}

		if (shadowMapIds_[i] != -1)
		{
			glDeleteTextures(1, &shadowMapIds_[i]);
			shadowMapIds_[i] = -1;
		}
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

void ShadowMappingShader::execute(const Vector3f& cameraPosition, const Vector3f& cameraDirection,
	const std::vector<ShadedModel3D*>& pShadedModel3Ds, const DirectionalLight* pDirLight)
{
	glUseProgram(id_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	const Matrix44f biasMatrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	for (GLint i = 0; i < 4; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferIds_[i]);
		glViewport(0, 0, shadowMapSize_, shadowMapSize_);

		glClear(GL_DEPTH_BUFFER_BIT);

		Matrix44f projectionMatrix;
		projectionMatrix.setOrthographic(radii[i], radii[i], 0.0f, 2.0f * radii[i]);

		//Vector3f center = cameraPosition;
		// @TEMP
		Vector3f center(0.0f, 0.0f, 0.0f);
		Vector3f lightDirection(pDirLight->direction_[0], pDirLight->direction_[1],
			pDirLight->direction_[2]);
		Vector3f eye = center + lightDirection * radii[i];
		Matrix44f viewMatrix;
		viewMatrix.setLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z,
			0.0f, 1.0f, 0.0f);

		for (size_t j = 0; j < pShadedModel3Ds.size(); j++)
		{
			Matrix44f modelMatrix;
			const Vector3f& position = pShadedModel3Ds[j]->position_;
			const Vector3f& rotationAxis = pShadedModel3Ds[j]->rotationAxis_;
			GLfloat angle = pShadedModel3Ds[j]->angle_;
			modelMatrix = Matrix44f().setTranslation(position.x, position.y, position.z)
				* Matrix44f().setRotation(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z)
				* Matrix44f().setScale(pShadedModel3Ds[j]->scale_);

			size_t index = j * 4 + i;
			depthModelViewProjectionMatrices_[index] = projectionMatrix * viewMatrix * modelMatrix;
			glUniformMatrix4fv(depthModelViewProjectionMatrixUniformLocation_, 1, GL_FALSE,
				&depthModelViewProjectionMatrices_[index].m00);
			depthModelViewProjectionMatrices_[index] = biasMatrix * depthModelViewProjectionMatrices_[index];

			draw(pShadedModel3Ds[j]);
		}
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

const GLuint* ShadowMappingShader::getShadowMapIds() const
{
	return shadowMapIds_;
}

void ShadowMappingShader::draw(const ShadedModel3D* pShadedModel3D)
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
