#pragma once

#include <string>
#include <fstream>
#include <bullet/btBulletDynamicsCommon.h>
#include "vector3.h"

std::ifstream& operator >> (std::ifstream& in, btVector3& vector3);
std::ifstream& operator >> (std::ifstream& in, btQuaternion& quaternion);

void extractDirectoryPath(std::string& filename);

void readNextCode(std::ifstream& stream, std::string& input);

void QuaternionToEulerXYZ(const btQuaternion& quaternion, btVector3& euler);

void QuaternionToEulerXYZ(const btQuaternion& quaternion, Vector3f& euler);

float clamp(float min, float max, float x);

float smoothStart2(float t);
float smoothStop2(float t);
float smoothStep2(float t);
