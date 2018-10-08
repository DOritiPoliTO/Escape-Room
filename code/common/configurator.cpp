#include "configurator.h"

Configurator& Configurator::getInstance()
{
	static Configurator instance;
	return instance;
}

const char* Configurator::getParameterValue(const char* parameterName) const
{
	auto it = parameterMap_.find(parameterName);
	if (it == parameterMap_.end())
	{
		throw std::exception();
	}

	return it->second.data();
}

Configurator::Configurator()
{
	std::ifstream file;

	try
	{
		file.open("../EscapeRoom/config/config.ini");
		if (!file.is_open())
		{
			throw std::exception();
		}

		char buffer[256];
		while (file.good())
		{
			file.getline(buffer, 256);

			if (buffer[0] == '#'
				|| buffer[0] == '['
				|| strcmp(buffer, "") == 0)
			{
				continue;
			}

			int i = 0;
			while (buffer[i] != '=')
			{
				i++;
			}

			buffer[i] = '\0';

			parameterMap_.insert(std::pair<std::string, std::string>(buffer, &buffer[i + 1]));
		}
	}
	catch (...)
	{
		if (file.is_open())
		{
			file.close();
		}

		throw;
	}
}
