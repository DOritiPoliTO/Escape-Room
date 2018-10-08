#include "sky_dome.h"
#include "../game.h"

SkyDome::SkyDome(const char* cloudsImageFilename, const char* moonImageFilename,
	const char* starsImageFilename) :
	pCloudsDome_(nullptr),
	pMoonModel_(nullptr),
	pStarsDome_(nullptr),
	cloudsAngularSpeed_(2.0f)
{
	try
	{
		pCloudsDome_ = new ShadelessModel3D(cloudsImageFilename);
		pMoonModel_ = new ShadelessModel3D(moonImageFilename);
		pStarsDome_ = new ShadelessModel3D(starsImageFilename);

		Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];
		worldScene.add(pStarsDome_);
		worldScene.add(pMoonModel_);
		worldScene.add(pCloudsDome_);
	}
	catch (...)
	{
		if (pStarsDome_ != nullptr)
		{
			delete pStarsDome_;
			pStarsDome_ = nullptr;
		}

		if (pMoonModel_ != nullptr)
		{
			delete pMoonModel_;
			pMoonModel_ = nullptr;
		}

		if (pCloudsDome_ != nullptr)
		{
			delete pCloudsDome_;
			pCloudsDome_ = nullptr;
		}

		throw;
	}
}

SkyDome::~SkyDome()
{
	if (pStarsDome_ != nullptr)
	{
		delete pStarsDome_;
		pStarsDome_ = nullptr;
	}

	if (pMoonModel_ != nullptr)
	{
		delete pMoonModel_;
		pMoonModel_ = nullptr;
	}

	if (pCloudsDome_ != nullptr)
	{
		delete pCloudsDome_;
		pCloudsDome_ = nullptr;
	}
}

void SkyDome::update(float deltaTime)
{
	pCloudsDome_->angle_ += deltaTime * cloudsAngularSpeed_;
	if (pCloudsDome_->angle_ > 360.0f)
	{
		pCloudsDome_->angle_ -= 360.0f;
	}
}
