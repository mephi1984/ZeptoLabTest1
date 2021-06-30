#pragma once

#include <string>
#include <vector>

namespace ZL
{
	std::string readTextFile(const std::string& filename);

	std::vector<char> readFile(const std::string& filename);

	bool findString(const char* in, char* list);

}