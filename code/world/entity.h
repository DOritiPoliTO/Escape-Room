#pragma once

#include <string>

class Entity
{
public:
	Entity() :tag_("") {}
	Entity(const char* tag) :tag_(tag) {}

	const std::string& getTag() const { return tag_; }

private:
	std::string tag_;
};
