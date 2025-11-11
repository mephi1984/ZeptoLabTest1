#pragma once

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <stack>
#include <memory>
#include <unordered_map>
namespace ZL
{
	std::string readTextFile(const std::string& filename);

	std::vector<char> readFile(const std::string& filename);

	std::vector<char> readFileFromZIP(const std::string& filename, const std::string& zipfilename);

	bool findString(const char* in, char* list);

}