#include "bullet_debug_drawer.h"

BulletDebugDrawer::BulletDebugDrawer(PhysicsEngine& physicsEngine, GLdouble fovy) :
	fovy_(fovy),
	physicsEngine_(physicsEngine)
{
	setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	physicsEngine.setDebugDrawer(this);
}

void BulletDebugDrawer::render(const Vector3f& cameraPosition, const Vector3f& cameraDirection)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth_, windowHeight_);

	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glMatrixMode(GL_PROJECTION);
	GLdouble ratio = (GLdouble)windowWidth_ / (GLdouble)windowHeight_;
	glLoadIdentity();
	gluPerspective(fovy_, ratio, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		cameraPosition.x + cameraDirection.x, cameraPosition.y + cameraDirection.y,
		cameraPosition.z + cameraDirection.z, 0.0, 1.0, 0.0);

	physicsEngine_.debugDrawWorld();
}

void BulletDebugDrawer::setWindowSize(GLsizei windowWidth, GLsizei windowHeight)
{
	windowWidth_ = windowWidth;
	windowHeight_ = windowHeight;
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glUseProgram(0);

	glPushMatrix();

	glBegin(GL_LINES);
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
	glVertex3f(to.getX(), to.getY(), to.getZ());
	glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
	glEnd();

	glPopMatrix();
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString)
{
	std::cout << warningString << std::endl;
}
