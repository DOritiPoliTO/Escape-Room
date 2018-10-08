#include "utility.h"

std::ifstream& operator >> (std::ifstream& in, btVector3& vector3)
{
	btScalar x, y, z;

	in >> x >> y >> z;
	vector3.setValue(x, y, z);

	return in;
}

std::ifstream& operator >> (std::ifstream& in, btQuaternion& quaternion)
{
	btScalar x, y, z, w;

	in >> x >> y >> z >> w;
	quaternion.setX(x);
	quaternion.setY(y);
	quaternion.setZ(z);
	quaternion.setW(w);

	return in;
}

void extractDirectoryPath(std::string& filename)
{
	size_t length = filename.length();
	if (length == 0)
	{
		return;
	}

	char* path = new char[length + 1];

	size_t i;
	for (i = length - 1; i > 0; i--)
	{
		if (filename[i] == '/')
		{
			path[i + 1] = '\0';
			break;
		}
	}

	for (size_t j = 0; j <= i; j++)
	{
		path[j] = filename[j];
	}

	filename.clear();
	filename.assign(path);
}

void readNextCode(std::ifstream& stream, std::string& input)
{
	char c;

	stream >> input;
	if (input != "#")
	{
		return;
	}

	do
	{
		do
		{
			c = stream.get();
		} while (c != '/n');

		stream >> input;
	} while (input == "#");
}

void QuaternionToEulerXYZ(const btQuaternion& quaternion, btVector3& euler)
{
	float ysqr = quaternion.getY() * quaternion.getY();

	// Rotation around x axis
	float t0 = 2.0f * (quaternion.getW() * quaternion.getX() + quaternion.getY() * quaternion.getZ());
	float t1 = 1.0f - 2.0f * (quaternion.getX() * quaternion.getX() + ysqr);
	euler.setX(atan2f(t0, t1));

	// Rotation around y axis
	float t2 = 2.0f * (quaternion.getW() * quaternion.getY() - quaternion.getZ() * quaternion.getX());
	t2 = ((t2 > 1.0f) ? 1.0f : t2);
	t2 = ((t2 < -1.0f) ? -1.0f : t2);
	euler.setY(asinf(t2));

	// Rotation around z axis
	float t3 = 2.0f * (quaternion.getW() * quaternion.getZ() + quaternion.getX() * quaternion.getY());
	float t4 = 1.0f - 2.0f * (ysqr + quaternion.getZ() * quaternion.getZ());
	euler.setZ(atan2f(t3, t4));
}

void QuaternionToEulerXYZ(const btQuaternion& quaternion, Vector3f& euler)
{
	float ysqr = quaternion.getY() * quaternion.getY();

	// Rotation around x axis
	float t0 = 2.0f * (quaternion.getW() * quaternion.getX() + quaternion.getY() * quaternion.getZ());
	float t1 = 1.0f - 2.0f * (quaternion.getX() * quaternion.getX() + ysqr);
	euler.x = atan2f(t0, t1);

	// Rotation around y axis
	float t2 = +2.0f * (quaternion.getW() * quaternion.getY() - quaternion.getZ() * quaternion.getX());
	t2 = ((t2 > 1.0f) ? 1.0f : t2);
	t2 = ((t2 < -1.0f) ? -1.0f : t2);
	euler.y = asinf(t2);

	// Rotation around z axis
	float t3 = 2.0f * (quaternion.getW() * quaternion.getZ() + quaternion.getX() * quaternion.getY());
	float t4 = 1.0f - 2.0f * (ysqr + quaternion.getZ() * quaternion.getZ());
	euler.z = atan2f(t3, t4);
}

float clamp(float min, float max, float x)
{
	if (x > max)
	{
		return max;
	}

	if (x < min)
	{
		return min;
	}

	return x;
}

float smoothStart2(float t)
{
	return t * t;
}

float smoothStop2(float t)
{
	return 1.0f - (1.0f - t) * (1.0f - t);
}

float smoothStep2(float t)
{
	return t * smoothStop2(t) + (1.0f - t) * smoothStart2(t);
}
