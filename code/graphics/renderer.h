#pragma once

#include "shaders/shaded_model_3d_shader.h"
#include "shaders/shadow_mapping_shader.h"
#include "shaders/shadow_cube_mapping_shader.h"
#include "shaders/single_texture_model_shader.h"
#include "shaders/text_2d_shader.h"
#include "scene.h"
#include "../common/configurator.h"

class Renderer
{
public:
	Scene scenes_[8];

	static Renderer& getInstance();

	void createProjectionMatrices(GLsizei windowWidth, GLsizei windowHeight);
	void createEnvironmentMap(int sceneIndex, const Vector3f& eye);
	void renderScene(int sceneIndex);
	void renderSceneToTexture(int sceneIndex, Texture* pTexture);

private:
	ShadedModel3DShader shadedModel3DShader_;
	ShadowMappingShader shadowMappingShader_;
	ShadowCubeMappingShader shadowCubeMappingShader_;
	SingleTextureModelShader singleTextureModelShader_;
	Text2DShader text2DShader_;

	GLsizei windowWidth_;
	GLsizei windowHeight_;
	GLfloat aspectRatio_;
	GLfloat fovy_;
	GLfloat zNear_;
	GLfloat zFar_;

	Matrix44f perspectiveMatrix_;
	Matrix44f orthographicMatrix_;

	GLuint rttFrameBufferId_;
	GLuint rttTextureId_;
	GLuint rttDepthRenderBufferId_;

	GLsizei environmentMapSize_;
	GLuint environmentMapFrameBufferId_;
	GLuint environmentMapTextureCubeId_;
	GLuint environmentMapDepthBufferId_;

	Frustum viewFrustum_;

	Renderer(GLsizei dirLightShadowMapSize, GLsizei pointLightShadowMapSize, GLsizei environmentMapSize,
		GLfloat fovy, GLfloat zNear, GLfloat zFar);
	Renderer(const Renderer& other);
	~Renderer();
};
