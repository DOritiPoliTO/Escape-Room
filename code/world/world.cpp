#include "world.h"
#include "../game.h"

World::World() :
	pActor_(nullptr),
	skyDome_(nullptr),
	pDirLight_(nullptr)
{
	try
	{
		Scene& worldScene = Renderer::getInstance().scenes_[SceneId::WORLD_SCENE];

		// Create the directional light
		GLfloat ambient[] = { 0.1f, 0.1f, 0.125f };
		GLfloat diffuse[] = { 0.2f, 0.2f, 0.25f };
		GLfloat specular[] = { 0.3f, 0.3f, 0.3f };
		Vector3f position(0.1f, 0.0f, -1.0f);
		position.normalize();
		pDirLight_ = new DirectionalLight(ambient, diffuse, specular, &position.x);
		worldScene.pDirLight_ = pDirLight_;

		// Create the sky dome
		skyDome_ = new SkyDome("../EscapeRoom/data/models/skydome_clouds.smdl",
			"../EscapeRoom/data/models/skydome_moon.smdl",
			"../EscapeRoom/data/models/skydome_stars.smdl");

		position = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f rotationAxis(0.0f, 1.0f, 0.0f);
		float rotationAngle(0.0f);
		Prop* pProp = new Prop(
			"../EscapeRoom/data/physics/room.pbd",
			"../EscapeRoom/data/models/room.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);
		Prop* pRoom = pProp;

		Renderer::getInstance().createEnvironmentMap(SceneId::WORLD_SCENE, Vector3f(0.0f, 0.0f, 0.0f));

		position = Vector3f(0.6f, 1.8f, -3.84f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/curtains.pbd",
			"../EscapeRoom/data/models/curtains.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-2.0f, 1.8f, -3.84f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/curtains.pbd",
			"../EscapeRoom/data/models/curtains.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(0.6f, 1.3f, -4.5f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/window.pbd",
			"../EscapeRoom/data/models/window.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-2.0f, 1.3f, -4.5f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/window.pbd",
			"../EscapeRoom/data/models/window.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(2.8f, 0.0f, 0.5f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = -120.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/chair.pbd",
			"../EscapeRoom/data/models/chair.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-2.8f, 0.0f, -1.9f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = -260.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/chair.pbd",
			"../EscapeRoom/data/models/chair.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-0.7f, 0.0f, -1.9f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = -95.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/chair.pbd",
			"../EscapeRoom/data/models/chair.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-4.7f, 0.0f, 2.7f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/vase.pbd",
			"../EscapeRoom/data/models/vase.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-4.6f, 0.0f, -3.4f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/vase_rose.pbd",
			"../EscapeRoom/data/models/vase_rose.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-1.8f, 0.65f, -2.0f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/cup.pbd",
			"../EscapeRoom/data/models/cup.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(-1.7f, 0.0f, -2.0f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/table.pbd",
			"../EscapeRoom/data/models/table.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(3.4f, 0.0f, -2.7f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = -90.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/bookcase.pbd",
			"../EscapeRoom/data/models/bookcase.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(0.8f, 0.0f, 2.4f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = -180.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/chest.pbd",
			"../EscapeRoom/data/models/chest.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(0.61f, 0.29f, 2.51f);
		rotationAxis = Vector3f(1.0f, 0.0f, 0.0f);
		rotationAngle = -75.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/disk.pbd",
			"../EscapeRoom/data/models/disk.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle,
			"Disk"
		);
		pProps_.push_back(pProp);
		Prop* pDisk = pProp;

		position = Vector3f(1.8f, 0.87f, 1.2f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = -90.0f;
		pProp = new Gramophone(
			"../EscapeRoom/data/physics/gramophone.pbd",
			"../EscapeRoom/data/models/gramophone.mdl",
			position,
			rotationAxis,
			rotationAngle,
			pDisk
		);
		pProps_.push_back(pProp);

		position = Vector3f(-2.7f, 1.56f, 2.985f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 180.0f;
		pProp = new Painting(
			"../EscapeRoom/data/physics/painting.pbd",
			"../EscapeRoom/data/models/painting.mdl",
			position,
			rotationAxis,
			rotationAngle,
			0.33f
		);
		pProps_.push_back(pProp);

		position = Vector3f(-0.7f, 1.5f, 2.985f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 180.0f;
		pProp = new Painting(
			"../EscapeRoom/data/physics/painting2.pbd",
			"../EscapeRoom/data/models/painting2.mdl",
			position,
			rotationAxis,
			rotationAngle,
			0.33f
		);
		pProps_.push_back(pProp);

		position = Vector3f(3.3f, 1.14f, -2.4f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Book(
			"../EscapeRoom/data/physics/book.pbd",
			"../EscapeRoom/data/models/book.mdl",
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);

		position = Vector3f(1.9f, 0.0f, 0.5f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 90.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/desk.pbd",
			"../EscapeRoom/data/models/desk.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);
		Prop* pDesk = pProp;

		position = Vector3f(2.06f, 0.72f, -0.17f);
		rotationAngle = 90.0f;
		pProp = new Drawer(
			"../EscapeRoom/data/physics/drawer.pbd",
			"../EscapeRoom/data/models/drawer.mdl",
			position,
			rotationAxis,
			rotationAngle,
			pDesk
		);
		pProps_.push_back(pProp);

		position = Vector3f(2.06f, 0.74f, -0.17f);
		rotationAngle = 0.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/matchsticks.pbd",
			"../EscapeRoom/data/models/matchsticks.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle,
			"Matchsticks"
		);
		pProps_.push_back(pProp);

		position = Vector3f(1.72f, 0.87f, -0.2f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		pProp = new Candle(
			"../EscapeRoom/data/physics/candle.pbd",
			"../EscapeRoom/data/models/candle.mdl",
			position,
			rotationAxis,
			rotationAngle,
			"Candle"
		);
		pProps_.push_back(pProp);

		position = Vector3f(-2.7f, 1.34f, 3.14f);
		rotationAxis = Vector3f(1.0f, 0.0f, 0.0f);
		rotationAngle = 90.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/key.pbd",
			"../EscapeRoom/data/models/key.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle,
			"DoorKey"
		);
		pProps_.push_back(pProp);
		Prop* pKey = pProp;

		position = Vector3f(-4.82f, 1.23f, -2.2f);
		rotationAxis = Vector3f(1.0f, 0.0f, 0.0f);
		rotationAngle = 90.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/clock_key.pbd",
			"../EscapeRoom/data/models/clock_key.mdl",
			true,
			false,
			position,
			rotationAxis,
			rotationAngle,
			"ClockKey"
		);
		pProps_.push_back(pProp);
		Prop* pClockKey = pProp;

		position = Vector3f(-4.8f, 1.1f, -2.3f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 90.0f;
		pProp = new Clock(
			"../EscapeRoom/data/physics/clock.pbd",
			"../EscapeRoom/data/models/clock.mdl",
			position,
			rotationAxis,
			rotationAngle,
			pClockKey
		);
		pProps_.push_back(pProp);

		position = Vector3f(-4.99f, 0.0f, 0.3f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 90.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/door_frame.pbd",
			"../EscapeRoom/data/models/door_frame.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);
		Prop* pDoorFrame = pProp;

		position = Vector3f(-5.0f, 0.0f, 0.3f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 90.0f;
		pProp = new Door(
			"../EscapeRoom/data/physics/door.pbd",
			"../EscapeRoom/data/models/door.mdl",
			position,
			rotationAxis,
			rotationAngle,
			pDoorFrame,
			pKey
		);
		pProps_.push_back(pProp);

		position = Vector3f(-2.7f, 1.3f, 3.23f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 180.0f;
		pProp = new Prop(
			"../EscapeRoom/data/physics/safe.pbd",
			"../EscapeRoom/data/models/safe.mdl",
			false,
			false,
			position,
			rotationAxis,
			rotationAngle
		);
		pProps_.push_back(pProp);
		Prop* pSafe = pProp;

		position = Vector3f(-2.7f, 1.3f, 3.05f);
		rotationAxis = Vector3f(0.0f, 1.0f, 0.0f);
		rotationAngle = 180.0f;
		pProp = new SafeLid(
			"../EscapeRoom/data/physics/safe_lid.pbd",
			"../EscapeRoom/data/models/safe_lid.mdl",
			position,
			rotationAxis,
			rotationAngle,
			pSafe
		);
		pProps_.push_back(pProp);

		pActor_ = new Actor("../EscapeRoom/data/physics/actor.pbd");
	}
	catch (...)
	{
		if (pActor_ != nullptr)
		{
			delete pActor_;
			pActor_ = nullptr;
		}

		size_t i = pProps_.size() - 1;
		while (i != std::numeric_limits<size_t>().max())
		{
			if (pProps_[i] != nullptr)
			{
				delete pProps_[i];
				pProps_[i] = nullptr;
			}

			i--;
		}
		pProps_.clear();

		if (skyDome_ != nullptr)
		{
			delete skyDome_;
			skyDome_ = nullptr;
		}

		if (pDirLight_ != nullptr)
		{
			delete pDirLight_;
			pDirLight_ = nullptr;
		}

		throw;
	}
}

World::~World()
{
	if (pActor_ != nullptr)
	{
		delete pActor_;
		pActor_ = nullptr;
	}

	size_t i = pProps_.size() - 1;
	while (i != std::numeric_limits<size_t>().max())
	{
		if (pProps_[i] != nullptr)
		{
			delete pProps_[i];
			pProps_[i] = nullptr;
		}

		i--;
	}
	pProps_.clear();

	if (skyDome_ != nullptr)
	{
		delete skyDome_;
		skyDome_ = nullptr;
	}

	if (pDirLight_ != nullptr)
	{
		delete pDirLight_;
		pDirLight_ = nullptr;
	}
}

void World::update(float deltaTime)
{
	PhysicsEngine::getInstance().update(deltaTime);

	pActor_->update(deltaTime);

	for (size_t i = 0; i < pProps_.size(); i++)
	{
		pProps_[i]->update(deltaTime);
	}

	skyDome_->update(deltaTime);
}
