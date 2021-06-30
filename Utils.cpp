#include "Utils.h"

#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>

namespace ZL
{

	std::string readTextFile(const std::string& filename)
	{
		std::ifstream f(filename);

		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

		return str;
	}

	std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary);

		file.unsetf(std::ios::skipws);

		std::streampos fileSize;

		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> vec;
		vec.reserve(fileSize);

		vec.insert(vec.begin(),
			std::istream_iterator<char>(file),
			std::istream_iterator<char>());

		return vec;
	}

	bool findString(const char* in, char* list)
	{
		size_t thisLength = strlen(in);
		while (*list != 0)
		{
			size_t length = strcspn(list, " ");

			if (thisLength == length)
				if (!strncmp(in, list, length))
					return true;

			list += length;
			list += 1;
		}
		return false;
	}
};
