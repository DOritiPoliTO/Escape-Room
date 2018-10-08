#pragma once

#include "actor.h"
#include "props/door.h"
#include "props/safe_lid.h"
#include "props/drawer.h"
#include "props/painting.h"
#include "props/candle.h"
#include "props/book.h"
#include "props/clock.h"
#include "props/gramophone.h"
#include "../physics/physics_engine.h"
#include "sky_dome.h"

class World
{
public:
	World();
	~World();

	void update(float deltaTime);

private:
	Actor* pActor_;
	DirectionalLight* pDirLight_;
	SkyDome* skyDome_;
	std::vector<Prop*> pProps_;
};
