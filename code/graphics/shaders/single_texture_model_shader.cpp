#include "single_texture_model_shader.h"

SingleTextureModelShader::SingleTextureModelShader() :
	Shader("../EscapeRoom/data/shaders/single_texture_model_shader.vert",
		"../EscapeRoom/data/shaders/single_texture_model_shader.frag")
{
	modelViewProjectionMatrixUniformLocation_ = glGetUniformLocation(id_, "u_ModelViewProjectionMatrix");
	textureSamplerUniformLocation_ = glGetUniformLocation(id_, "u_TextureSampler");
	colorScaleUniformLocation_ = glGetUniformLocation(id_, "u_ColorScale");
}

void SingleTextureModelShader::execute(const Matrix44f& perspectiveMatrix, const Vector3f& cameraPosition,
	const Vector3f& cameraDirection, const std::vector<Billboard3D*>& pBillboard3Ds)
{
	glUseProgram(id_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Matrix44f viewMatrix;
	viewMatrix.setLookAt(
		cameraPosition.x, cameraPosition.y, cameraPosition.z,
		cameraPosition.x + cameraDirection.x, cameraPosition.y + cameraDirection.y,
		cameraPosition.z + cameraDirection.z, 0.0f, 1.0f, 0.0f
	);

	for (size_t i = 0; i < pBillboard3Ds.size(); i++)
	{
		Matrix44f modelMatrix;
		modelMatrix.setTranslation(pBillboard3Ds[i]->position_.x, pBillboard3Ds[i]->position_.y,
			pBillboard3Ds[i]->position_.z);

		Matrix44f modelViewProjectionMatrix = perspectiveMatrix * viewMatrix * modelMatrix;
		modelViewProjectionMatrix.m00 = 1.0f;
		modelViewProjectionMatrix.m01 = 0.0f;
		modelViewProjectionMatrix.m02 = 0.0f;
		modelViewProjectionMatrix.m03 = 0.0f;
		glUniformMatrix4fv(modelViewProjectionMatrixUniformLocation_, 1, false,
			&modelViewProjectionMatrix.m00);

		draw(pBillboard3Ds[i]);
	}

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void SingleTextureModelShader::execute(const Matrix44f& orthographicMatrix,
	const std::vector<Billboard2D*>& pBillboard2Ds)
{
	glUseProgram(id_);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (size_t i = 0; i < pBillboard2Ds.size(); i++)
	{
		Matrix44f modelMatrix;
		modelMatrix.setTranslation(pBillboard2Ds[i]->position_.x, pBillboard2Ds[i]->position_.y, 0.0f);

		Matrix44f modelViewProjectionMatrix = orthographicMatrix * modelMatrix;
		glUniformMatrix4fv(modelViewProjectionMatrixUniformLocation_, 1, false,
			&modelViewProjectionMatrix.m00);

		draw(pBillboard2Ds[i]);
	}

	glDisable(GL_BLEND);
}

void SingleTextureModelShader::execute(const Matrix44f& perspectiveMatrix, const Matrix44f& viewMatrix,
	const std::vector<ShadelessModel3D*>& pShadelessModel3Ds)
{
	glUseProgram(id_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for each (auto pShadelessModel3D in pShadelessModel3Ds)
	{
		const Vector3f& position = pShadelessModel3D->position_;
		const Vector3f& rotationAxis = pShadelessModel3D->rotationAxis_;
		float angle = pShadelessModel3D->angle_;

		Matrix44f modelViewProjectionMatrix = perspectiveMatrix * viewMatrix
			* Matrix44f().setTranslation(position.x, position.y, position.z)
			* Matrix44f().setRotation(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
		glUniformMatrix4fv(modelViewProjectionMatrixUniformLocation_, 1, false,
			&modelViewProjectionMatrix.m00);

		draw(pShadelessModel3D);
	}

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void SingleTextureModelShader::execute(const Matrix44f& perspectiveMatrix, const Vector3f& cameraPosition,
	const Vector3f& cameraDirection, const std::vector<ShadelessModel3D*>& pShadelessModel3Ds)
{
	glUseProgram(id_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Matrix44f viewMatrix;
	viewMatrix.setLookAt(
		cameraPosition.x, cameraPosition.y, cameraPosition.z,
		cameraPosition.x + cameraDirection.x, cameraPosition.y + cameraDirection.y,
		cameraPosition.z + cameraDirection.z, 0.0f, 1.0f, 0.0f
	);

	for each (auto pShadelessModel3D in pShadelessModel3Ds)
	{
		const Vector3f& position = pShadelessModel3D->position_;
		const Vector3f& rotationAxis = pShadelessModel3D->rotationAxis_;
		float angle = pShadelessModel3D->angle_;

		Matrix44f modelViewProjectionMatrix = perspectiveMatrix * viewMatrix
			* Matrix44f().setTranslation(position.x, position.y, position.z)
			* Matrix44f().setRotation(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
		glUniformMatrix4fv(modelViewProjectionMatrixUniformLocation_, 1, false,
			&modelViewProjectionMatrix.m00);

		draw(pShadelessModel3D);
	}

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void SingleTextureModelShader::execute(const Matrix44f& perspectiveMatrix, const Matrix44f& viewMatrix,
	const std::vector<ShadedModel3D*>& pShadedModel3Ds)
{
	glUseProgram(id_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Color colorScale(1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4fv(colorScaleUniformLocation_, 1, &colorScale.r);

	for each (auto pShadedModel3D in pShadedModel3Ds)
	{
		const Vector3f& position = pShadedModel3D->position_;
		const Vector3f& rotationAxis = pShadedModel3D->rotationAxis_;
		float angle = pShadedModel3D->angle_;

		Matrix44f modelViewProjectionMatrix = perspectiveMatrix * viewMatrix
			* Matrix44f().setTranslation(position.x, position.y, position.z)
			* Matrix44f().setRotation(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
		glUniformMatrix4fv(modelViewProjectionMatrixUniformLocation_, 1, false,
			&modelViewProjectionMatrix.m00);

		draw(pShadedModel3D);
	}

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void SingleTextureModelShader::draw(Billboard* pBillboard)
{
	glUniform4fv(colorScaleUniformLocation_, 1, &pBillboard->colorScale_.r);

	const Texture* pTexture = pBillboard->getTexture();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pBillboard->getTexture()->getId());
	glUniform1i(textureSamplerUniformLocation_, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pBillboard->getVertexBuffer().getId());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, pBillboard->getTexCoordBuffer().getId());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_QUADS, 0, pBillboard->getVertexBuffer().getSize() / 12);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void SingleTextureModelShader::draw(const ShadelessModel3D* pShadelessModel3D)
{
	glUniform4fv(colorScaleUniformLocation_, 1, &pShadelessModel3D->colorScale_.r);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pShadelessModel3D->getTexture()->getId());
	glUniform1i(textureSamplerUniformLocation_, 0);

	glBindBuffer(GL_ARRAY_BUFFER, pShadelessModel3D->getVertexBuffer().getId());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ShadelessModel3D::VertexAttribs),
		(void*)offsetof(struct ShadelessModel3D::VertexAttribs, position)
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ShadelessModel3D::VertexAttribs),
		(void*)offsetof(struct ShadelessModel3D::VertexAttribs, texCoords)
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pShadelessModel3D->getIndexBuffer().getId());

	glDrawElements(
		GL_TRIANGLES,
		pShadelessModel3D->getIndexBuffer().getSize() / 4,
		GL_UNSIGNED_INT,
		(void*)0
	);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void SingleTextureModelShader::draw(const ShadedModel3D* pShadedModel3D)
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pShadedModel3DData->indexBuffer.getId());
	}

	for each (auto& triangleGroup in pShadedModel3DData->triangleGroups)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, triangleGroup.material.pDiffuseColorMap->getId());
		glUniform1i(textureSamplerUniformLocation_, 0);

		glDrawElements(
			GL_TRIANGLES,
			triangleGroup.numberOfTriangles * 3,
			GL_UNSIGNED_INT,
			(void*)(triangleGroup.indexBufferOffset * sizeof(GLuint))
		);
	}

	// Disable vertex attrib arrays
	{
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
}
