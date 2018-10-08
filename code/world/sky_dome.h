#pragma once

#include "../graphics/renderer.h"

class SkyDome
{
public:
	float cloudsAngularSpeed_;

	SkyDome(const char* cloudsImageFilename, const char* moonImageFilename,
		const char* starsImageFilename);
	~SkyDome();

	void update(float deltaTime);

private:
	ShadelessModel3D* pCloudsDome_;
	ShadelessModel3D* pMoonModel_;
	ShadelessModel3D* pStarsDome_;
};
