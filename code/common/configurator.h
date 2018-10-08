#pragma once

#include <map>
#include <fstream>
#include <string>

class Configurator
{
public:
	static Configurator& getInstance();

	const char* getParameterValue(const char* parameterName) const;

private:
	Configurator();
	Configurator(const Configurator& other);

	std::map<std::string, std::string> parameterMap_;
};
