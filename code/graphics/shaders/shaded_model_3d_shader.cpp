#include "shaded_model_3d_shader.h"

ShadedModel3DShader::ShadedModel3DShader() :
	Shader("../EscapeRoom/data/shaders/shaded_model_3d_shader.vert",
		"../EscapeRoom/data/shaders/shaded_model_3d_shader.frag")
{
	viewMatrixUniformLocation_ = glGetUniformLocation(id_, "u_ViewMatrix");
	modelViewMatrixUniformLocation_ = glGetUniformLocation(id_, "u_ModelViewMatrix");
	projectionMatrixUniformLocation_ = glGetUniformLocation(id_, "u_ProjectionMatrix");
	modelMatrixUniformLocation_ = glGetUniformLocation(id_, "u_ModelMatrix");
	depthModelViewProjectionMatrixUniformLocations_[0] = glGetUniformLocation(id_, "u_DepthModelViewProjectionMatrix[0]");
	depthModelViewProjectionMatrixUniformLocations_[1] = glGetUniformLocation(id_, "u_DepthModelViewProjectionMatrix[1]");
	depthModelViewProjectionMatrixUniformLocations_[2] = glGetUniformLocation(id_, "u_DepthModelViewProjectionMatrix[2]");
	depthModelViewProjectionMatrixUniformLocations_[3] = glGetUniformLocation(id_, "u_DepthModelViewProjectionMatrix[3]");
	normalTextureSamplerUniformLocation_ = glGetUniformLocation(id_, "u_NormalTextureSampler");
	diffuseColorTextureSamplerUniformLocation_ = glGetUniformLocation(id_, "u_DiffuseColorTextureSampler");
	specularTextureSamplerUniformLocation_ = glGetUniformLocation(id_, "u_SpecularTextureSampler");
	shadowMapCubeSamplerUniformLocation_ = glGetUniformLocation(id_, "u_ShadowMapCubeSampler");
	shadowMapSamplerUniformLocations_[0] = glGetUniformLocation(id_, "u_ShadowMapSampler[0]");
	shadowMapSamplerUniformLocations_[1] = glGetUniformLocation(id_, "u_ShadowMapSampler[1]");
	shadowMapSamplerUniformLocations_[2] = glGetUniformLocation(id_, "u_ShadowMapSampler[2]");
	shadowMapSamplerUniformLocations_[3] = glGetUniformLocation(id_, "u_ShadowMapSampler[3]");
	isDirLightOnUniformLocation_ = glGetUniformLocation(id_, "u_IsDirLightOn");
	dirLightAmbientUniformLocation_ = glGetUniformLocation(id_, "u_DirLightAmbient");
	dirLightDiffuseUniformLocation_ = glGetUniformLocation(id_, "u_DirLightDiffuse");
	dirLightSpecularUniformLocation_ = glGetUniformLocation(id_, "u_DirLightSpecular");
	dirLightDirection_WS_UniformLocation_ = glGetUniformLocation(id_, "u_DirLightDirection_WS");
	isPointLightOnUniformLocation_ = glGetUniformLocation(id_, "u_IsPointLightOn");
	pointLightDiffuseUniformLocation_ = glGetUniformLocation(id_, "u_PointLightDiffuse");
	pointLightSpecularUniformLocation_ = glGetUniformLocation(id_, "u_PointLightSpecular");
	pointLightPosition_WS_UniformLocation_ = glGetUniformLocation(id_, "u_PointLightPosition_WS");
	environmentMapCubeSamplerUniformLocation_ = glGetUniformLocation(id_, "u_EnvironmentMapCubeSampler");
	cameraPosition_WS_UniformLocation_ = glGetUniformLocation(id_, "u_CameraPosition_WS");
	materialReflectivityUniformLocation_ = glGetUniformLocation(id_, "u_MaterialReflectivity");
}

void ShadedModel3DShader::execute(const Vector3f& cameraPosition, const Vector3f& cameraDirection,
	const std::vector<ShadedModel3D*>& pShadedModel3Ds, const PointLight* pPointLight,
	GLuint pointLightShadowMapId, const DirectionalLight* pDirLight, const GLuint* dirLightShadowMapIds,
	const std::vector<Matrix44f>& depthModelViewProjectionMatrices, GLuint environmentMapId,
	const Matrix44f& projectionMatrix, const Frustum& viewFrustum)
{
	glUseProgram(id_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Create the view matrix and pass it to the shader
	Matrix44f viewMatrix;
	viewMatrix.setLookAt(
		cameraPosition.x, cameraPosition.y, cameraPosition.z,
		cameraPosition.x + cameraDirection.x, cameraPosition.y + cameraDirection.y,
		cameraPosition.z + cameraDirection.z, 0.0f, 1.0f, 0.0f
	);
	glUniformMatrix4fv(viewMatrixUniformLocation_, 1, GL_FALSE, &viewMatrix.m00);

	glUniformMatrix4fv(projectionMatrixUniformLocation_, 1, GL_FALSE, &projectionMatrix.m00);

	// Pass uniforms related to point light to the shader
	{
		if (pPointLight != nullptr)
		{
			glUniform1i(isPointLightOnUniformLocation_, 1);

			glUniform3fv(pointLightPosition_WS_UniformLocation_, 1, pPointLight->position_);
			glUniform3fv(pointLightDiffuseUniformLocation_, 1, pPointLight->diffuse_);
			glUniform3fv(pointLightSpecularUniformLocation_, 1, pPointLight->specular_);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pointLightShadowMapId);
			glUniform1i(shadowMapCubeSamplerUniformLocation_, 3);
		}
		else
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pointLightShadowMapId);
			glUniform1i(shadowMapCubeSamplerUniformLocation_, 3);

			glUniform1i(isPointLightOnUniformLocation_, 0);
		}
	}

	// Pass uniforms related to directional light to the shader
	{
		if (pDirLight != nullptr)
		{
			glUniform1i(isDirLightOnUniformLocation_, 1);

			glUniform3fv(dirLightDirection_WS_UniformLocation_, 1, pDirLight->direction_);
			glUniform3fv(dirLightAmbientUniformLocation_, 1, pDirLight->ambient_);
			glUniform3fv(dirLightDiffuseUniformLocation_, 1, pDirLight->diffuse_);
			glUniform3fv(dirLightSpecularUniformLocation_, 1, pDirLight->specular_);

			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, dirLightShadowMapIds[0]);
			glUniform1i(shadowMapSamplerUniformLocations_[0], 4);

			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, dirLightShadowMapIds[1]);
			glUniform1i(shadowMapSamplerUniformLocations_[1], 5);

			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, dirLightShadowMapIds[2]);
			glUniform1i(shadowMapSamplerUniformLocations_[2], 6);

			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, dirLightShadowMapIds[3]);
			glUniform1i(shadowMapSamplerUniformLocations_[3], 7);
		}
		else
		{
			glUniform1i(isDirLightOnUniformLocation_, 0);
		}
	}

	// Pass uniforms for environment map reflection
	{
		glUniform3fv(cameraPosition_WS_UniformLocation_, 1, &cameraPosition.x);

		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMapId);
		glUniform1i(environmentMapCubeSamplerUniformLocation_, 8);
	}

	// Render the model
	for (size_t i = 0; i < pShadedModel3Ds.size(); i++)
	{
		if (viewFrustum.checkSphere(pShadedModel3Ds[i]->getData()->boundingSphereRadius, pShadedModel3Ds[i]->position_))
		{
			size_t start = i * 4;
			size_t end = start + 4;
			for (size_t index = 0, j = start; j < end; j++, index++)
			{
				glUniformMatrix4fv(depthModelViewProjectionMatrixUniformLocations_[index], 1, GL_FALSE,
					&depthModelViewProjectionMatrices[j].m00);
			}

			// Create the model matrix and pass it to the shader
			const Vector3f& position = pShadedModel3Ds[i]->position_;
			const Vector3f& rotationAxis = pShadedModel3Ds[i]->rotationAxis_;
			GLfloat angle = pShadedModel3Ds[i]->angle_;
			Matrix44f modelMatrix = Matrix44f().setTranslation(position.x, position.y, position.z)
				* Matrix44f().setRotation(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z)
				* Matrix44f().setScale(pShadedModel3Ds[i]->scale_);
			glUniformMatrix4fv(modelMatrixUniformLocation_, 1, GL_FALSE, &modelMatrix.m00);

			// Create the model view matrix and pass it to the shader
			Matrix44f modelViewMatrix = viewMatrix * modelMatrix;
			glUniformMatrix4fv(modelViewMatrixUniformLocation_, 1, GL_FALSE, &modelViewMatrix.m00);

			draw(pShadedModel3Ds[i]);
		}
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void ShadedModel3DShader::draw(const ShadedModel3D* pShadedModel3D)
{
	auto* pShadedModel3DData = pShadedModel3D->getData();

	// Enable vertex attrib arrays and bind index buffer
	{
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

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(ShadedModel3D::VertexAttribs),
			(void*)offsetof(struct ShadedModel3D::VertexAttribs, texCoords)
		);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(ShadedModel3D::VertexAttribs),
			(void*)offsetof(struct ShadedModel3D::VertexAttribs, normal)
		);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(
			3,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(ShadedModel3D::VertexAttribs),
			(void*)offsetof(struct ShadedModel3D::VertexAttribs, tangent)
		);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(
			4,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(ShadedModel3D::VertexAttribs),
			(void*)offsetof(struct ShadedModel3D::VertexAttribs, bitangent)
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pShadedModel3DData->indexBuffer.getId());
	}

	for each (auto& triangleGroup in pShadedModel3DData->triangleGroups)
	{
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, triangleGroup.material.pNormalMap->getId());
			glUniform1i(normalTextureSamplerUniformLocation_, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, triangleGroup.material.pDiffuseColorMap->getId());
			glUniform1i(diffuseColorTextureSamplerUniformLocation_, 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, triangleGroup.material.pSpecularMap->getId());
			glUniform1i(specularTextureSamplerUniformLocation_, 2);
		}

		glUniform1f(materialReflectivityUniformLocation_, triangleGroup.material.reflectivity);

		glDrawElements(
			GL_TRIANGLES,
			triangleGroup.numberOfTriangles * 3,
			GL_UNSIGNED_INT,
			(void*)(triangleGroup.indexBufferOffset * sizeof(GLuint))
		);
	}

	// Disable vertex attrib arrays
	{
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
}
