#include "body.h"
#include "physics_engine.h"

Body::Body(const char* bodyFilename, void* pUserPointer, const Vector3f& position, const Vector3f& rotationAxis,
	float rotationAngle) :
	pMotionState_(nullptr),
	pRigidBody_(nullptr),
	pUserPointer_(pUserPointer)
{
	std::ifstream file;
	std::string code;
	std::string shape;
	btVector3 inertia;
	btVector3 halfExtents;
	btScalar radius;
	btTransform localTransform;
	btCollisionShape* pChildShape = nullptr;
	btVector3 localPosition;
	btQuaternion localRotation;
	btVector3 planeNormal;
	btScalar planeConstant;
	btScalar angle;
	btVector3 axis;

	try
	{
		file.open(bodyFilename);
		if (!file.is_open())
		{
			throw std::exception();
		}

		// Shape
		readNextCode(file, code);
		if (code != "shape")
		{
			throw std::exception();
		}

		file >> shape;
		if (shape == "CompoundShape")
		{
			btCompoundShape* pCompoundShape = new btCompoundShape;
			pShapes_.push_back(pCompoundShape);

			size_t numberOfChildShapes = 0;
			readNextCode(file, code);
			while (code == "child_shape")
			{
				numberOfChildShapes++;

				file >> shape;
				if (shape == "BoxShape")
				{
					readNextCode(file, code);
					if (code != "half_extents")
					{
						throw std::exception();
					}
					file >> halfExtents;

					pChildShape = new btBoxShape(halfExtents);
				}
				else if (shape == "CylinderShape")
				{
					readNextCode(file, code);
					if (code != "half_extents")
					{
						throw std::exception();
					}
					file >> halfExtents;

					pChildShape = new btCylinderShape(halfExtents);
				}
				else if (shape == "SphereShape")
				{
					readNextCode(file, code);
					if (code != "radius")
					{
						throw std::exception();
					}
					file >> radius;

					pChildShape = new btSphereShape(radius);
				}
				else if (shape == "StaticPlaneShape")
				{
					readNextCode(file, code);
					if (code != "plane_normal")
					{
						throw std::exception();
					}
					file >> planeNormal;

					readNextCode(file, code);
					if (code != "plane_constant")
					{
						throw std::exception();
					}
					file >> planeConstant;

					pChildShape = new btStaticPlaneShape(planeNormal, planeConstant);
				}
				else
				{
					throw std::exception();
				}

				readNextCode(file, code);
				if (code != "local_rotation")
				{
					throw std::exception();
				}
				file >> localRotation;

				readNextCode(file, code);
				if (code != "local_position")
				{
					throw std::exception();
				}
				file >> localPosition;

				localTransform.setIdentity();
				localTransform.setRotation(localRotation);
				localTransform.setOrigin(localPosition);
				pCompoundShape->addChildShape(localTransform, pChildShape);

				pShapes_.push_back(pChildShape);
				pChildShape = nullptr;

				readNextCode(file, code);
			}

			if (numberOfChildShapes == 0)
			{
				throw std::exception();
			}
		}
		else if (shape == "BoxShape")
		{
			readNextCode(file, code);
			if (code != "half_extents")
			{
				throw std::exception();
			}
			file >> halfExtents;

			btBoxShape* pBoxShape = new btBoxShape(halfExtents);
			pShapes_.push_back(pBoxShape);
		}
		else if (shape == "CylinderShape")
		{
			readNextCode(file, code);
			if (code != "half_extents")
			{
				throw std::exception();
			}
			file >> halfExtents;

			btCylinderShape* pCylinderShape = new btCylinderShape(halfExtents);
			pShapes_.push_back(pCylinderShape);
		}
		else
		{
			throw std::exception();
		}

		if (code != "mass")
		{
			readNextCode(file, code);
		}
		if (code != "mass")
		{
			throw std::exception();
		}
		file >> mass_;
		inertia = { 0.0f,0.0f,0.0f };
		pShapes_[0]->calculateLocalInertia(mass_, inertia);

		// Motion state
		btQuaternion rotation(btVector3(rotationAxis.x, rotationAxis.y, rotationAxis.z), rotationAngle * 0.017444f);
		btTransform transform(rotation, btVector3(position.x, position.y, position.z));
		pMotionState_ = new btDefaultMotionState(transform);

		// Finally create the rigid body
		btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(
			mass_,
			pMotionState_,
			pShapes_[0],
			inertia
		);
		pRigidBody_ = new btRigidBody(rigidBodyConstructionInfo);
		pRigidBody_->setRestitution(0.1f);
		pRigidBody_->setFriction(0.3f);
		pRigidBody_->setRollingFriction(0.005f);
		pRigidBody_->setSpinningFriction(0.005f);
		pRigidBody_->setDamping(0.3f, 0.3f);

		pRigidBody_->setUserPointer(this);

		// Enable continuous collision detection for dynamic bodies
		if (mass_ != 0.0f)
		{
			pRigidBody_->setCcdMotionThreshold(0.001f);
			pRigidBody_->setCcdSweptSphereRadius(0.02f);
		}

		file.close();
	}
	catch (...)
	{
		if (pChildShape != nullptr)
		{
			delete pChildShape;
		}

		if (file.is_open())
		{
			file.close();
		}

		if (pMotionState_ != nullptr)
		{
			delete pMotionState_;
			pMotionState_ = nullptr;
		}

		if (pRigidBody_ != nullptr)
		{
			delete pRigidBody_;
			pRigidBody_ = nullptr;
		}

		for (size_t i = 0; i < pShapes_.size(); i++)
		{
			if (pShapes_[i] != nullptr)
			{
				delete pShapes_[i];
				pShapes_[i] = nullptr;
			}
		}

		throw;
	}
}

Body::~Body()
{
	if (pMotionState_ != nullptr)
	{
		delete pMotionState_;
		pMotionState_ = nullptr;
	}

	if (pRigidBody_ != nullptr)
	{
		delete pRigidBody_;
		pRigidBody_ = nullptr;
	}

	for (size_t i = 0; i < pShapes_.size(); i++)
	{
		if (pShapes_[i] != nullptr)
		{
			delete pShapes_[i];
			pShapes_[i] = nullptr;
		}
	}
}

void Body::applyImpulse(const Vector3f& impulse)
{
	pRigidBody_->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
	pRigidBody_->activate(true);
}

void Body::applyImpulse(const Vector3f& impulse, const Vector3f& position)
{
	pRigidBody_->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(position.x, position.y, position.z));
	pRigidBody_->activate(true);
}

void Body::applyTorqueImpulse(const Vector3f& torqueImpulse)
{
	pRigidBody_->applyTorqueImpulse(btVector3(torqueImpulse.x, torqueImpulse.y, torqueImpulse.z));
	pRigidBody_->activate(true);
}

void Body::setVelocity(const Vector3f& velocity)
{
	pRigidBody_->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

void Body::setAngularVelocity(const Vector3f& angularVelocity)
{
	pRigidBody_->setAngularVelocity(btVector3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
}

void Body::setLocalScaling(const Vector3f& scale)
{
	pShapes_[0]->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
}

void Body::convertFromLocalToWorldSpace(const Vector3f& localPosition, Vector3f& worldPosition)
{
	btVector3 btLocalPosition(localPosition.x, localPosition.y, localPosition.z);
	btVector3 btWorldPosition(pRigidBody_->getWorldTransform() * btLocalPosition);
	worldPosition = btWorldPosition;
}

void Body::enableCollisionWithBody(Body* pOtherBody, bool enable)
{
	pRigidBody_->setIgnoreCollisionCheck(pOtherBody->pRigidBody_, !enable);
}

const Vector3f& Body::getPosition() const
{
	return reinterpret_cast<const Vector3f&>(pRigidBody_->getCenterOfMassPosition());
}

const Vector3f& Body::getVelocity() const
{
	return reinterpret_cast<const Vector3f&>(pRigidBody_->getLinearVelocity());
}

Vector3f Body::getRotationAxis() const
{
	return getRotation().getAxis();
}

btQuaternion Body::getRotation() const
{
	return pRigidBody_->getCenterOfMassTransform().getRotation();
}

float Body::getRotationAngle() const
{
	return getRotation().getAngle() * 57.32484076f;
}

float Body::getMass() const
{
	return mass_;
}

void* Body::getUserPointer() const
{
	return pUserPointer_;
}

void* Body::operator new(std::size_t count)
{
	return BatchAllocator::getInstance().allocateBlock<Body>();
}

void Body::operator delete(void* ptr) noexcept
{
	BatchAllocator::getInstance().releaseBlock<Body>(ptr);
}
