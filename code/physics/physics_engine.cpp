#include "physics_engine.h"

PhysicsEngine& PhysicsEngine::getInstance()
{
	static PhysicsEngine instance;
	return instance;
}

void PhysicsEngine::update(float deltaTime)
{
	pDynamicsWorld_->stepSimulation(deltaTime, 16);
}

void PhysicsEngine::addBody(Body* pBody)
{
	pDynamicsWorld_->addRigidBody(pBody->pRigidBody_);
}

void PhysicsEngine::removeBody(Body* pBody)
{
	if (pBody->pRigidBody_ != nullptr)
	{
		pDynamicsWorld_->removeRigidBody(pBody->pRigidBody_);
	}
}

void PhysicsEngine::addConstraint(Constraint* pConstraint)
{
	pDynamicsWorld_->addConstraint(
		pConstraint->pBtTypedConstraint_,
		!pConstraint->collisionsBetweenLinkedBodiesEnabled_
	);
}

void PhysicsEngine::removeConstraint(Constraint* pConstraint)
{
	if (pConstraint->pBtTypedConstraint_ != nullptr)
	{
		pDynamicsWorld_->removeConstraint(pConstraint->pBtTypedConstraint_);
	}
}

// @DEBUG
void PhysicsEngine::setDebugDrawer(btIDebugDraw* pDebugDrawer)
{
	pDynamicsWorld_->setDebugDrawer(pDebugDrawer);
}

void PhysicsEngine::debugDrawWorld() const
{
	pDynamicsWorld_->debugDrawWorld();
}

Body* PhysicsEngine::raycast(const Vector3f& from, const Vector3f& direction, float distance)
{
	btVector3 rayFromWorld(from.x, from.y, from.z);
	btVector3 dir(direction.x, direction.y, direction.z);
	btVector3 rayToWorld = rayFromWorld + dir * distance;
	btCollisionWorld::ClosestRayResultCallback resultCallback(rayFromWorld, rayToWorld);
	pDynamicsWorld_->rayTest(rayFromWorld, rayToWorld, resultCallback);
	if (resultCallback.hasHit())
	{
		return reinterpret_cast<Body*>(resultCallback.m_collisionObject->getUserPointer());
	}

	return nullptr;
}

PhysicsEngine::PhysicsEngine() :
	pBroadphase_(nullptr),
	pCollisionConfiguration_(nullptr),
	pDispatcher_(nullptr),
	pSolver_(nullptr),
	pDynamicsWorld_(nullptr)
{
	try
	{
		pBroadphase_ = new btDbvtBroadphase();

		pCollisionConfiguration_ = new btDefaultCollisionConfiguration();

		pDispatcher_ = new btCollisionDispatcher(pCollisionConfiguration_);

		pSolver_ = new btSequentialImpulseConstraintSolver;

		pDynamicsWorld_ = new btDiscreteDynamicsWorld(pDispatcher_, pBroadphase_, pSolver_, pCollisionConfiguration_);

		pDynamicsWorld_->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	}
	catch (...)
	{
		if (pDynamicsWorld_ != nullptr)
		{
			delete pDynamicsWorld_;
			pDynamicsWorld_ = nullptr;
		}

		if (pSolver_ != nullptr)
		{
			delete pSolver_;
			pSolver_ = nullptr;
		}

		if (pDispatcher_ != nullptr)
		{
			delete pDispatcher_;
			pDispatcher_ = nullptr;
		}

		if (pCollisionConfiguration_ != nullptr)
		{
			delete pCollisionConfiguration_;
			pCollisionConfiguration_ = nullptr;
		}

		if (pBroadphase_ != nullptr)
		{
			delete pBroadphase_;
			pBroadphase_ = nullptr;
		}

		throw;
	}
}

PhysicsEngine::~PhysicsEngine()
{
	if (pDynamicsWorld_ != nullptr)
	{
		delete pDynamicsWorld_;
		pDynamicsWorld_ = nullptr;
	}

	if (pSolver_ != nullptr)
	{
		delete pSolver_;
		pSolver_ = nullptr;
	}

	if (pDispatcher_ != nullptr)
	{
		delete pDispatcher_;
		pDispatcher_ = nullptr;
	}

	if (pCollisionConfiguration_ != nullptr)
	{
		delete pCollisionConfiguration_;
		pCollisionConfiguration_ = nullptr;
	}

	if (pBroadphase_ != nullptr)
	{
		delete pBroadphase_;
		pBroadphase_ = nullptr;
	}
}
