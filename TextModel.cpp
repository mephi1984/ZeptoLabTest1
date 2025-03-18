#include "TextModel.h"
#include <regex>
#include <string>
#include <fstream>
#include <iostream>

namespace ZL
{


	VertexDataStruct LoadFromTextFile(const std::string& fileName,  const std::string& ZIPFileName)
	{
		VertexDataStruct result;
		std::ifstream filestream;
		std::istringstream zipStream;
	
		if (!ZIPFileName.empty())
		{
			std::vector<char> fileData = readFileFromZIP(fileName, ZIPFileName);
			std::string fileContents(fileData.begin(), fileData.end());
			zipStream.str(fileContents);
		}
		else
		{
			filestream.open(fileName);
		}
	
		// Создаем ссылку f на нужный поток – после этого код ниже остается без изменений
		std::istream& f = (!ZIPFileName.empty()) ? static_cast<std::istream&>(zipStream) : static_cast<std::istream&>(filestream);
	

		//Skip first 5 lines
		std::string tempLine;	

		std::getline(f, tempLine);

		static const std::regex pattern_count(R"(\d+)");
		static const std::regex pattern_float(R"([-]?\d+\.\d+)");
		static const std::regex pattern_int(R"([-]?\d+)");


		std::smatch match;

		int numberVertices;

		if (std::regex_search(tempLine, match, pattern_count)) {
			std::string number_str = match.str();
			numberVertices = std::stoi(number_str);
		}
		else {
			throw std::runtime_error("No number found in the input string.");
		}

		std::vector<Vector3f> vertices;

		vertices.resize(numberVertices);
		for (int i = 0; i < numberVertices; i++)
		{
			std::getline(f, tempLine);

			std::vector<float> floatValues;

			auto b = tempLine.cbegin();
			auto e = tempLine.cend();
			while (std::regex_search(b, e, match, pattern_float)) {
				floatValues.push_back(std::stof(match.str()));
				b = match.suffix().first;
			}

			vertices[i] = Vector3f{ floatValues[0], floatValues[1], floatValues[2] };
		}

		std::cout << "Hello x1" << std::endl;

		std::getline(f, tempLine); //===UV Coordinates:

		std::getline(f, tempLine); //triangle count
		int numberTriangles;

		if (std::regex_search(tempLine, match, pattern_count)) {
			std::string number_str = match.str();
			numberTriangles = std::stoi(number_str);
		}
		else {
			throw std::runtime_error("No number found in the input string.");
		}


		// Now process UVs
		std::vector<std::array<Vector2f, 3>> uvCoords;

		uvCoords.resize(numberTriangles);

		for (int i = 0; i < numberTriangles; i++)
		{
			std::getline(f, tempLine); //Face 0

			int uvCount;
			std::getline(f, tempLine);
			if (std::regex_search(tempLine, match, pattern_count)) {
				std::string number_str = match.str();
				uvCount = std::stoi(number_str);
			}
			else {
				throw std::runtime_error("No number found in the input string.");
			}

			if (uvCount != 3)
			{
				throw std::runtime_error("more than 3 uvs");
			}

			std::vector<float> floatValues;

			for (int j = 0; j < 3; j++)
			{
				std::getline(f, tempLine); //UV <Vector (-0.3661, -1.1665)>
				
				auto b = tempLine.cbegin();
				auto e = tempLine.cend();
				floatValues.clear();
				while (std::regex_search(b, e, match, pattern_float)) {
					floatValues.push_back(std::stof(match.str()));
					b = match.suffix().first;
				}

				if (floatValues.size() != 2)
				{
					throw std::runtime_error("more than 2 uvs---");
				}

				uvCoords[i][j] = Vector2f{ floatValues[0],floatValues[1] };
			}

			




		}

		std::cout << "Hello eee" << std::endl;

		std::getline(f, tempLine); //===Normals:


		std::vector<Vector3f> normals;

		normals.resize(numberVertices);
		for (int i = 0; i < numberVertices; i++)
		{
			std::getline(f, tempLine);

			std::vector<float> floatValues;

			auto b = tempLine.cbegin();
			auto e = tempLine.cend();
			while (std::regex_search(b, e, match, pattern_float)) {
				floatValues.push_back(std::stof(match.str()));
				b = match.suffix().first;
			}

			normals[i] = Vector3f{ floatValues[0], floatValues[1], floatValues[2] };
		}

		std::cout << "Hello x4" << std::endl;

		std::getline(f, tempLine); //===Triangles: 3974


		std::vector<std::array<int, 3>> triangles;

		triangles.resize(numberTriangles);
		for (int i = 0; i < numberTriangles; i++)
		{
			std::getline(f, tempLine);

			std::vector<int> intValues;

			auto b = tempLine.cbegin();
			auto e = tempLine.cend();
			while (std::regex_search(b, e, match, pattern_int)) {
				intValues.push_back(std::stoi(match.str()));
				b = match.suffix().first;
			}

			triangles[i] = { intValues[0], intValues[1], intValues[2] };
		}


		std::cout << "Hello x5" << std::endl;



		// Now let's process bone weights and vertices

		for (int i = 0; i < numberTriangles; i++)
		{
			
			result.PositionData.push_back(vertices[triangles[i][0]]);
			result.PositionData.push_back(vertices[triangles[i][1]]);
			result.PositionData.push_back(vertices[triangles[i][2]]);

			/*
			result.NormalData.push_back(normals[triangles[i][0]]);
			result.NormalData.push_back(normals[triangles[i][1]]);
			result.NormalData.push_back(normals[triangles[i][2]]);
			*/
			result.TexCoordData.push_back(uvCoords[i][0]);
			result.TexCoordData.push_back(uvCoords[i][1]);
			result.TexCoordData.push_back(uvCoords[i][2]);

		}

		//Swap from Blender format to OpenGL format
		for (int i = 0; i < result.PositionData.size(); i++)
		{
			Vector3f tempVec = result.PositionData[i];
			result.PositionData[i].v[0] = tempVec.v[1];
			result.PositionData[i].v[1] = tempVec.v[2];
			result.PositionData[i].v[2] = tempVec.v[0];

			/*
			tempVec = result.NormalData[i];
			result.NormalData[i].v[0] = tempVec.v[1];
			result.NormalData[i].v[1] = tempVec.v[2];
			result.NormalData[i].v[2] = tempVec.v[0];*/

		}

		return result;

	}




}