#include "renderer.h"

Renderer& Renderer::getInstance()
{
	static Renderer instance(
		std::atoi(Configurator::getInstance().getParameterValue("dir_light_shadow_map_size")),
		std::atoi(Configurator::getInstance().getParameterValue("point_light_shadow_map_size")),
		std::atoi(Configurator::getInstance().getParameterValue("environment_map_size")),
		std::atof(Configurator::getInstance().getParameterValue("fov")),
		std::atof(Configurator::getInstance().getParameterValue("z_near_plane")),
		std::atof(Configurator::getInstance().getParameterValue("z_far_plane"))
	);

	return instance;
}

Renderer::Renderer(GLsizei dirLightShadowMapSize, GLsizei pointLightShadowMapSize, GLsizei environmentMapSize,
	GLfloat fovy, GLfloat zNear, GLfloat zFar) :
	shadedModel3DShader_(),
	shadowMappingShader_(dirLightShadowMapSize),
	shadowCubeMappingShader_(pointLightShadowMapSize),
	singleTextureModelShader_(),
	text2DShader_(),
	windowWidth_(800),
	windowHeight_(600),
	rttDepthRenderBufferId_(-1),
	rttFrameBufferId_(-1),
	rttTextureId_(-1),
	environmentMapFrameBufferId_(-1),
	environmentMapDepthBufferId_(-1),
	environmentMapTextureCubeId_(-1),
	environmentMapSize_(environmentMapSize),
	fovy_(fovy),
	zNear_(zNear),
	zFar_(zFar),
	viewFrustum_()
{
	try
	{
		/*glEnable(GL_MULTISAMPLE);

		GLint iMultiSample = 0;
		GLint iNumSamples = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
		glGetIntegerv(GL_SAMPLES, &iNumSamples);*/

		// Create the framebuffer for the "render to texture" functionality
		{
			glGenFramebuffers(1, &rttFrameBufferId_);
			glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);

			glGenTextures(1, &rttTextureId_);
			glBindTexture(GL_TEXTURE_2D, rttTextureId_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth_, windowHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glGenRenderbuffers(1, &rttDepthRenderBufferId_);
			glBindRenderbuffer(GL_RENDERBUFFER, rttDepthRenderBufferId_);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth_, windowHeight_);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
				rttDepthRenderBufferId_);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rttTextureId_, 0);
			GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, drawBuffers);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				throw std::exception("Error during creation of the framebuffer used for rendering to texture");
			}
		}

		// Generate frame buffer and cube map for enviroment mapping
		{
			glGenFramebuffers(1, &environmentMapFrameBufferId_);
			glBindFramebuffer(GL_FRAMEBUFFER, environmentMapFrameBufferId_);

			glGenTextures(1, &environmentMapTextureCubeId_);
			glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMapTextureCubeId_);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, environmentMapSize_,
				environmentMapSize_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, environmentMapSize_,
				environmentMapSize_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, environmentMapSize_,
				environmentMapSize_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, environmentMapSize_,
				environmentMapSize_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, environmentMapSize_,
				environmentMapSize_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, environmentMapSize_,
				environmentMapSize_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glGenRenderbuffers(1, &environmentMapDepthBufferId_);
			glBindRenderbuffer(GL_RENDERBUFFER, environmentMapDepthBufferId_);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, environmentMapSize_,
				environmentMapSize_);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
				environmentMapDepthBufferId_);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				environmentMapTextureCubeId_, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				environmentMapTextureCubeId_, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				environmentMapTextureCubeId_, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				environmentMapTextureCubeId_, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				environmentMapTextureCubeId_, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				environmentMapTextureCubeId_, 0);
			GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, drawBuffers);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				throw std::exception();
			}
		}
	}
	catch (...)
	{
		{
			if (environmentMapFrameBufferId_ != -1)
			{
				glDeleteFramebuffers(1, &environmentMapFrameBufferId_);
				environmentMapFrameBufferId_ = -1;
			}

			if (environmentMapTextureCubeId_ != -1)
			{
				glDeleteTextures(1, &environmentMapTextureCubeId_);
				environmentMapTextureCubeId_ = -1;
			}

			if (environmentMapDepthBufferId_ != -1)
			{
				glDeleteRenderbuffers(1, &environmentMapDepthBufferId_);
				environmentMapDepthBufferId_ = -1;
			}
		}

		{
			if (rttFrameBufferId_ != -1)
			{
				glDeleteFramebuffers(1, &rttFrameBufferId_);
				rttFrameBufferId_ = -1;
			}

			if (rttTextureId_ != -1)
			{
				glDeleteTextures(1, &rttTextureId_);
				rttTextureId_ = -1;
			}

			if (rttDepthRenderBufferId_ != -1)
			{
				glDeleteRenderbuffers(1, &rttDepthRenderBufferId_);
				rttDepthRenderBufferId_ = -1;
			}
		}

		throw;
	}
}

Renderer::~Renderer()
{
	{
		if (environmentMapFrameBufferId_ != -1)
		{
			glDeleteFramebuffers(1, &environmentMapFrameBufferId_);
			environmentMapFrameBufferId_ = -1;
		}

		if (environmentMapTextureCubeId_ != -1)
		{
			glDeleteTextures(1, &environmentMapTextureCubeId_);
			environmentMapTextureCubeId_ = -1;
		}

		if (environmentMapDepthBufferId_ != -1)
		{
			glDeleteRenderbuffers(1, &environmentMapDepthBufferId_);
			environmentMapDepthBufferId_ = -1;
		}
	}

	{
		if (rttFrameBufferId_ != -1)
		{
			glDeleteFramebuffers(1, &rttFrameBufferId_);
			rttFrameBufferId_ = -1;
		}

		if (rttTextureId_ != -1)
		{
			glDeleteTextures(1, &rttTextureId_);
			rttTextureId_ = -1;
		}

		if (rttDepthRenderBufferId_ != -1)
		{
			glDeleteRenderbuffers(1, &rttDepthRenderBufferId_);
			rttDepthRenderBufferId_ = -1;
		}
	}
}

void Renderer::createProjectionMatrices(GLsizei windowWidth, GLsizei windowHeight)
{
	if (windowHeight == 0)
	{
		windowHeight = 1;
	}

	windowWidth_ = windowWidth;
	windowHeight_ = windowHeight;

	aspectRatio_ = (float)windowHeight / (float)windowWidth;
	perspectiveMatrix_.setPerspective(fovy_, 1.0f / aspectRatio_, zNear_, zFar_);

	orthographicMatrix_.setOrthographic(1.0f, aspectRatio_, -1.0f, 1.0f);

	// Destroy the framebuffer for render to texture and recreate it
	{
		if (rttFrameBufferId_ != -1)
		{
			glDeleteFramebuffers(1, &rttFrameBufferId_);
			rttFrameBufferId_ = -1;
		}

		if (rttTextureId_ != -1)
		{
			glDeleteTextures(1, &rttTextureId_);
			rttTextureId_ = -1;
		}

		if (rttDepthRenderBufferId_ != -1)
		{
			glDeleteRenderbuffers(1, &rttDepthRenderBufferId_);
			rttDepthRenderBufferId_ = -1;
		}

		glGenFramebuffers(1, &rttFrameBufferId_);
		glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);

		glGenTextures(1, &rttTextureId_);
		glBindTexture(GL_TEXTURE_2D, rttTextureId_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth_, windowHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glGenRenderbuffers(1, &rttDepthRenderBufferId_);
		glBindRenderbuffer(GL_RENDERBUFFER, rttDepthRenderBufferId_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth_, windowHeight_);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
			rttDepthRenderBufferId_);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rttTextureId_, 0);
		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::exception("Error during creation of the framebuffer used for rendering to texture");
		}
	}
}

void Renderer::createEnvironmentMap(int sceneIndex, const Vector3f& eye)
{
	const Scene& scene = scenes_[sceneIndex];

	glBindFramebuffer(GL_FRAMEBUFFER, environmentMapFrameBufferId_);
	glViewport(0, 0, environmentMapSize_, environmentMapSize_);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Matrix44f perspectiveMatrix;
	perspectiveMatrix.setPerspective(90.0f, 1.0f, 0.1, 1000.0);

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

	for (size_t i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			environmentMapTextureCubeId_, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const Vector3f center = eye + layerDirections[i];

		Matrix44f viewMatrix;
		viewMatrix.setLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z,
			upDirections[i].x, upDirections[i].y, upDirections[i].z);

		singleTextureModelShader_.execute(perspectiveMatrix, viewMatrix, scene.pShadedModel3Ds_);

		singleTextureModelShader_.execute(perspectiveMatrix, viewMatrix, scene.pShadelessModel3Ds_);
	}
}

void Renderer::renderScene(int sceneIndex)
{
	const Scene& scene = scenes_[sceneIndex];

	// Update view frustum
	new (&viewFrustum_) Frustum(scene.camera_.position_, scene.camera_.direction_, fovy_, zNear_, zFar_, aspectRatio_);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth_, windowHeight_);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render shaded 3d models
	if (scene.pShadedModel3Ds_.size() != 0
		&& (scene.pDirLight_ != nullptr || scene.pPointLight_ != nullptr))
	{
		if (scene.pDirLight_ != nullptr)
		{
			// Create shadow maps for directional light
			shadowMappingShader_.execute(
				scene.camera_.position_,
				scene.camera_.direction_,
				scene.pShadedModel3Ds_,
				scene.pDirLight_
			);
		}

		if (scene.pPointLight_ != nullptr)
		{
			// Create shadow (cube) maps for point light
			shadowCubeMappingShader_.execute(
				scene.camera_.position_,
				scene.camera_.direction_,
				scene.pShadedModel3Ds_,
				scene.pPointLight_
			);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth_, windowHeight_);

		// Draw shaded models 3d
		shadedModel3DShader_.execute(
			scene.camera_.position_,
			scene.camera_.direction_,
			scene.pShadedModel3Ds_,
			scene.pPointLight_,
			shadowCubeMappingShader_.getShadowMapId(),
			scene.pDirLight_,
			shadowMappingShader_.getShadowMapIds(),
			shadowMappingShader_.depthModelViewProjectionMatrices_,
			environmentMapTextureCubeId_,
			perspectiveMatrix_,
			viewFrustum_
		);
	}

	// Draw shadeless models 3d
	if (scene.pShadelessModel3Ds_.size() != 0)
	{
		singleTextureModelShader_.execute(
			perspectiveMatrix_,
			scene.camera_.position_,
			scene.camera_.direction_,
			scene.pShadelessModel3Ds_
		);
	}

	// Render billboards in perspective projection
	if (scene.pBillboard3Ds_.size() != 0)
	{
		singleTextureModelShader_.execute(
			perspectiveMatrix_,
			scene.camera_.position_,
			scene.camera_.direction_,
			scene.pBillboard3Ds_
		);
	}

	// Render billboards in orthographic projection
	if (scene.pBillboard2Ds_.size() != 0)
	{
		singleTextureModelShader_.execute(orthographicMatrix_, scene.pBillboard2Ds_);
	}

	// Render 2d texts
	if (scene.pText2Ds_.size() != 0)
	{
		text2DShader_.execute(aspectRatio_, scene.pText2Ds_);
	}
}

void Renderer::renderSceneToTexture(int sceneIndex, Texture* pTexture)
{
	const Scene& scene = scenes_[sceneIndex];

	// Update view frustum
	new (&viewFrustum_) Frustum(scene.camera_.position_, scene.camera_.direction_, fovy_, zNear_, zFar_, aspectRatio_);

	glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);
	glViewport(0, 0, windowWidth_, windowHeight_);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render shaded 3d models
	if (scene.pShadedModel3Ds_.size() != 0
		&& (scene.pDirLight_ != nullptr || scene.pPointLight_ != nullptr))
	{
		if (scene.pDirLight_ != nullptr)
		{
			// Create shadow maps for directional light
			shadowMappingShader_.execute(
				scene.camera_.position_,
				scene.camera_.direction_,
				scene.pShadedModel3Ds_,
				scene.pDirLight_
			);
		}

		if (scene.pPointLight_ != nullptr)
		{
			// Create shadow (cube) maps for point light
			shadowCubeMappingShader_.execute(
				scene.camera_.position_,
				scene.camera_.direction_,
				scene.pShadedModel3Ds_,
				scene.pPointLight_
			);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);
		glViewport(0, 0, windowWidth_, windowHeight_);

		// Draw shaded models 3d
		shadedModel3DShader_.execute(
			scene.camera_.position_,
			scene.camera_.direction_,
			scene.pShadedModel3Ds_,
			scene.pPointLight_,
			shadowCubeMappingShader_.getShadowMapId(),
			scene.pDirLight_,
			shadowMappingShader_.getShadowMapIds(),
			shadowMappingShader_.depthModelViewProjectionMatrices_,
			environmentMapTextureCubeId_,
			perspectiveMatrix_,
			viewFrustum_
		);
	}

	// Render shadeless 3d models
	if (scene.pShadelessModel3Ds_.size() != 0)
	{
		singleTextureModelShader_.execute(
			perspectiveMatrix_,
			scene.camera_.position_,
			scene.camera_.direction_,
			scene.pShadelessModel3Ds_
		);
	}

	// Render 3d billboards
	if (scene.pBillboard3Ds_.size() != 0)
	{
		singleTextureModelShader_.execute(
			perspectiveMatrix_,
			scene.camera_.position_,
			scene.camera_.direction_,
			scene.pBillboard3Ds_
		);
	}

	// Copy pixels from rtt texture to texture
	{
		void* pixels = new char[windowWidth_ * windowHeight_ * 3];

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, rttTextureId_);
		glReadPixels(0, 0, windowWidth_, windowHeight_, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, pTexture->id_);

		if (pTexture->width_ == windowWidth_
			&& pTexture->height_ == windowHeight_)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, windowWidth_, windowHeight_, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth_, windowHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			pTexture->width_ = windowWidth_;
			pTexture->height_ = windowHeight_;
		}

		delete[] pixels;
	}
}
