#pragma once

#pragma once

#include <iostream>
#include <bullet/LinearMath/btIDebugDraw.h>
#include <GL/glew.h>
#include "../physics/physics_engine.h"

class BulletDebugDrawer :public btIDebugDraw
{
public:
	BulletDebugDrawer(PhysicsEngine& physicsEngine, GLdouble fovy);

	void render(const Vector3f& cameraPosition, const Vector3f& cameraDirection);

	void setWindowSize(GLsizei windowWidth, GLsizei windowHeight);

private:
	int debugMode_;
	GLsizei windowWidth_;
	GLsizei windowHeight_;
	GLdouble fovy_;
	PhysicsEngine& physicsEngine_;

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
		int lifeTime, const btVector3& color) {}
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString) {}
	void setDebugMode(int debugMode) { debugMode_ = debugMode; }
	int getDebugMode() const { return debugMode_; }
};
