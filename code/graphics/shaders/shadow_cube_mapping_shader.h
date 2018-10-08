#pragma once

#include "../shader.h"
#include "../shaded_model_3d.h"
#include "../point_light.h"
#include "../../common/matrix44.h"

class ShadowCubeMappingShader :public Shader
{
public:
	ShadowCubeMappingShader(GLsizei shadowMapSize);
	~ShadowCubeMappingShader();

	void execute(const Vector3f& cameraPosition, const Vector3f& cameraDirection,
		const std::vector<ShadedModel3D*>& pShadedModel3Ds, const PointLight* pPointLight);

	GLuint getShadowMapId() const;

private:
	static const float zNear_;
	static const float zFar_;

	GLint depthModelViewProjectionMatrixUniformLocation_;
	GLint modelMatrixUniformLocation_;
	GLint lightPosition_WS_UniformLocation_;

	GLsizei shadowMapSize_;
	GLuint frameBufferId_;
	GLuint shadowMapId_;

	void draw(const ShadedModel3D* pShadedModel3D);
};
