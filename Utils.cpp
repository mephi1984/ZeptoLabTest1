#include "Utils.h"
#include <cstring>
#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <zip.h>

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
		std::cout << "started reserving" + filename << std::endl; 
		vec.reserve(fileSize);
		std::cout << "ended reserving" << std::endl; 
		vec.insert(vec.begin(),
			std::istream_iterator<char>(file),
			std::istream_iterator<char>());

		return vec;
	}

	std::vector<char> readFileFromZIP(const std::string& filename, const std::string& zipfilename) {
		const std::string zipPath = zipfilename;
		int zipErr;
		zip_t* archive = zip_open(zipPath.c_str(), ZIP_RDONLY, &zipErr);
		if (!archive) {
			throw std::runtime_error("Ошибка открытия ZIP: " + zipPath);
		}
	
		std::string cleanFilename = filename;
		if (cleanFilename.rfind("./", 0) == 0) {
			cleanFilename = cleanFilename.substr(2);
		}
	
		std::cout << "Ищем в ZIP: " << cleanFilename << std::endl;
		
		zip_file_t* zipFile = zip_fopen(archive, cleanFilename.c_str(), 0);
		if (!zipFile) {
			zip_close(archive);
			throw std::runtime_error("Файл не найден в ZIP: " + cleanFilename);
		}
	
		zip_stat_t fileStat;
		if (zip_stat(archive, cleanFilename.c_str(), 0, &fileStat) != 0) {
			zip_fclose(zipFile);
			zip_close(archive);
			throw std::runtime_error("Ошибка чтения ZIP-статистики.");
		}
	
		std::vector<char> fileData;
		fileData.resize(fileStat.size);
		
		zip_fread(zipFile, fileData.data(), fileData.size());
	
		zip_fclose(zipFile);
		zip_close(archive);
	
		return fileData;
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
