#include "BoneAnimatedModel.h"
#include <regex>
#include <string>
#include <fstream>
#include <iostream>

namespace ZL
{

	int getIndexByValue(const std::string& name, const std::vector<std::string>& words)
	{
		for (int i = 0; i < words.size(); i++)
		{
			if (words[i] == name)
			{
				return i;
			}
		}

		return -1;
	}


	void BoneSystem::LoadFromFile(const std::string& fileName)
	{
		

		std::ifstream f(fileName);

		//Skip first 5 lines
		std::string tempLine;
		for (int i = 0; i < 5; i++)
		{
			std::getline(f, tempLine);
		}
		std::getline(f, tempLine);

		static const std::regex pattern_count(R"(\d+)");
		static const std::regex pattern_float(R"([-]?\d+\.\d+)");
		static const std::regex pattern_int(R"([-]?\d+)");
		static const std::regex pattern_boneChildren(R"(\'([^\']+)\')");
		static const std::regex pattern_bone_weight(R"(\'([^\']+)\'.*?([-]?\d+\.\d+))");
		
		std::smatch match;

		int numberBones;

		if (std::regex_search(tempLine, match, pattern_count)) {
			std::string number_str = match.str();
			numberBones = std::stoi(number_str);
		}
		else {
			throw std::runtime_error("No number found in the input string.");
		}

		std::vector<Bone> bones;
		std::vector<std::string> boneNames;
		std::vector<std::string> boneParentNames;
		std::unordered_map<std::string, std::vector<std::string>> boneChildren;

		bones.resize(numberBones);
		boneNames.resize(numberBones);
		boneParentNames.resize(numberBones);

		for (int i = 0; i < numberBones; i++)
		{
			std::getline(f, tempLine);
			std::string boneName = tempLine.substr(6);

			boneNames[i] = boneName;

			std::getline(f, tempLine);

			std::vector<float> floatValues;

			auto b = tempLine.cbegin();
			auto e = tempLine.cend();
			while (std::regex_search(b, e, match, pattern_float)) {
				floatValues.push_back(std::stof(match.str()));
				b = match.suffix().first;
			}

			bones[i].boneStartWorld = Vector3f{ floatValues[0], floatValues[1], floatValues[2] };
			
			
			std::getline(f, tempLine); //skip tail

			std::getline(f, tempLine); //len

			if (std::regex_search(tempLine, match, pattern_float)) {
				std::string len_str = match.str();
				bones[i].boneLength = std::stof(len_str);
			}
			else {
				throw std::runtime_error("No number found in the input string.");
			}
			//---------- matrix begin

			std::getline(f, tempLine);

			b = tempLine.cbegin();
			e = tempLine.cend();
			floatValues.clear();
			while (std::regex_search(b, e, match, pattern_float)) {
				floatValues.push_back(std::stof(match.str()));
				b = match.suffix().first;
			}

			bones[i].boneMatrixWorld.m[0] = floatValues[0];
			bones[i].boneMatrixWorld.m[0 + 1 * 3] = floatValues[1];
			bones[i].boneMatrixWorld.m[0 + 2 * 3] = floatValues[2];


			std::getline(f, tempLine);

			b = tempLine.cbegin();
			e = tempLine.cend();
			floatValues.clear();
			while (std::regex_search(b, e, match, pattern_float)) {
				floatValues.push_back(std::stof(match.str()));
				b = match.suffix().first;
			}

			bones[i].boneMatrixWorld.m[1] = floatValues[0];
			bones[i].boneMatrixWorld.m[1 + 1 * 3] = floatValues[1];
			bones[i].boneMatrixWorld.m[1 + 2 * 3] = floatValues[2];


			std::getline(f, tempLine);

			b = tempLine.cbegin();
			e = tempLine.cend();
			floatValues.clear();
			while (std::regex_search(b, e, match, pattern_float)) {
				floatValues.push_back(std::stof(match.str()));
				b = match.suffix().first;
			}

			bones[i].boneMatrixWorld.m[2] = floatValues[0];
			bones[i].boneMatrixWorld.m[2 + 1 * 3] = floatValues[1];
			bones[i].boneMatrixWorld.m[2 + 2 * 3] = floatValues[2];

			//----------- matrix end
			std::getline(f, tempLine); //parent

			if (tempLine == "  Parent: None")
			{
				bones[i].parent = -1;
			}
			else
			{
				std::string boneParent = tempLine.substr(10);
				boneParentNames[i] = boneParent;
			}

			std::getline(f, tempLine); //children

			b = tempLine.cbegin();
			e = tempLine.cend();
			while (std::regex_search(b, e, match, pattern_boneChildren)) {
				
				boneChildren[boneName].push_back(match.str(1));
				b = match.suffix().first; 
			}

		}

		//Now process all the bones:
		for (int i = 0; i < numberBones; i++)
		{
			std::string boneName = boneNames[i];
			std::string boneParent = boneParentNames[i];
			if (boneParent == "")
			{
				bones[i].parent = -1;
			}
			else
			{
				bones[i].parent = getIndexByValue(boneParent, boneNames);
			}

			for (int j = 0; j < boneChildren[boneName].size(); j++)
			{
				bones[i].children.push_back(getIndexByValue(boneChildren[boneName][j], boneNames));
			}

			/*if (boneName == "Bone.020")
			{
				std::cout << i << std::endl;
			}*/
		}

		startBones = bones;
		currentBones = bones;

		///std::cout << "Hello!" << std::endl;
		
		std::getline(f, tempLine); //vertice count
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

			vertices[i] = Vector3f{floatValues[0], floatValues[1], floatValues[2]};
		}


		//==== process uv and normals begin

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

		//==== process uv and normals end

		std::getline(f, tempLine); //triangle count. 
		//numberTriangles; //Need to check if new value is the same as was read before

		if (std::regex_search(tempLine, match, pattern_count)) {
			std::string number_str = match.str();
			numberTriangles = std::stoi(number_str);
		}
		else {
			throw std::runtime_error("No number found in the input string.");
		}

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

		std::getline(f, tempLine);//=== Vertex Weights ===
		std::vector<std::array<BoneWeight, MAX_BONE_COUNT>> localVerticesBoneWeight;
		localVerticesBoneWeight.resize(numberVertices);

		for (int i = 0; i < numberVertices; i++)
		{
			std::getline(f, tempLine); //skip Vertex 0:
			std::getline(f, tempLine); //vertex group count
			int boneCount;

			if (std::regex_search(tempLine, match, pattern_count)) {
				std::string number_str = match.str();
				boneCount = std::stoi(number_str);
			}
			else {
				throw std::runtime_error("No number found in the input string.");
			}

			if (boneCount > MAX_BONE_COUNT)
			{
				throw std::runtime_error("more than 5 bones");
			}

			float sumWeights = 0;

			for (int j = 0; j < boneCount; j++)
			{
				std::getline(f, tempLine); //Group: 'Bone', Weight: 0.9929084181785583
				if (std::regex_search(tempLine, match, pattern_bone_weight)) {
					// ��������� ����� (��� �������)
					std::string word = match.str(1); 
					double weight = std::stod(match.str(2)); 

					int boneNumber = getIndexByValue(word, boneNames);
					localVerticesBoneWeight[i][j].boneIndex = boneNumber;
					localVerticesBoneWeight[i][j].weight = weight;
					sumWeights += weight;
				}
				else {
					throw std::runtime_error("No match found in the input string.");
				}
			}

			//Normalize weights:
			for (int j = 0; j < boneCount; j++)
			{
				localVerticesBoneWeight[i][j].weight = localVerticesBoneWeight[i][j].weight / sumWeights;
			}

		}

		std::getline(f, tempLine);//=== Animation Keyframes ===
		std::getline(f, tempLine);//=== Bone Transforms per Keyframe ===



		std::getline(f, tempLine);
		int numberKeyFrames;

		if (std::regex_search(tempLine, match, pattern_count)) {
			std::string number_str = match.str();
			numberKeyFrames = std::stoi(number_str);
		}
		else {
			throw std::runtime_error("No number found in the input string.");
		}

		animations.resize(1);

		animations[0].keyFrames.resize(numberKeyFrames);

		for (int i = 0; i < numberKeyFrames; i++)
		{
			std::getline(f, tempLine);
			int numberFrame;

			if (std::regex_search(tempLine, match, pattern_count)) {
				std::string number_str = match.str();
				numberFrame = std::stoi(number_str);
			}
			else {
				throw std::runtime_error("No number found in the input string.");
			}

			animations[0].keyFrames[i].frame = numberFrame;

			animations[0].keyFrames[i].bones.resize(numberBones);

			for (int j = 0; j < numberBones; j++)
			{
				std::getline(f, tempLine);
				std::string boneName = tempLine.substr(8);
				int boneNumber = getIndexByValue(boneName, boneNames);
				animations[0].keyFrames[i].bones[boneNumber] = startBones[boneNumber];

				std::getline(f, tempLine); //  Location: <Vector (0.0000, 0.0000, -0.0091)>

				std::vector<float> floatValues;

				auto b = tempLine.cbegin();
				auto e = tempLine.cend();
				while (std::regex_search(b, e, match, pattern_float)) {
					floatValues.push_back(std::stof(match.str()));
					b = match.suffix().first;
				}

				animations[0].keyFrames[i].bones[boneNumber].boneStartWorld = Vector3f{ floatValues[0], floatValues[1], floatValues[2] };
			
				std::getline(f, tempLine); // Rotation
				std::getline(f, tempLine); // Matrix

				//=============== Matrix begin ==================
				std::getline(f, tempLine);


				b = tempLine.cbegin();
				e = tempLine.cend();
				floatValues.clear();
				while (std::regex_search(b, e, match, pattern_float)) {
					floatValues.push_back(std::stof(match.str()));
					b = match.suffix().first;
				}

				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[0] = floatValues[0];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[0 + 1 * 4] = floatValues[1];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[0 + 2 * 4] = floatValues[2];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[0 + 3 * 4] = floatValues[3];


				std::getline(f, tempLine);
				b = tempLine.cbegin();
				e = tempLine.cend();
				floatValues.clear();
				while (std::regex_search(b, e, match, pattern_float)) {
					floatValues.push_back(std::stof(match.str()));
					b = match.suffix().first;
				}

				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[1] = floatValues[0];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[1 + 1 * 4] = floatValues[1];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[1 + 2 * 4] = floatValues[2];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[1 + 3 * 4] = floatValues[3];

				std::getline(f, tempLine);
				b = tempLine.cbegin();
				e = tempLine.cend();
				floatValues.clear();
				while (std::regex_search(b, e, match, pattern_float)) {
					floatValues.push_back(std::stof(match.str()));
					b = match.suffix().first;
				}

				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[2] = floatValues[0];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[2 + 1 * 4] = floatValues[1];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[2 + 2 * 4] = floatValues[2];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[2 + 3 * 4] = floatValues[3];


				std::getline(f, tempLine);
				b = tempLine.cbegin();
				e = tempLine.cend();
				floatValues.clear();
				while (std::regex_search(b, e, match, pattern_float)) {
					floatValues.push_back(std::stof(match.str()));
					b = match.suffix().first;
				}

				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[3] = floatValues[0];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[3 + 1 * 4] = floatValues[1];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[3 + 2 * 4] = floatValues[2];
				animations[0].keyFrames[i].bones[boneNumber].boneMatrixWorld.m[3 + 3 * 4] = floatValues[3];

				//std::getline(f, tempLine);// ignore last matrix line

				//=============== Matrix end ==================
			
			}

		}

		// Now let's process bone weights and vertices

		for (int i = 0; i < numberTriangles; i++)
		{
			
			mesh.PositionData.push_back(vertices[triangles[i][0]]);
			mesh.PositionData.push_back(vertices[triangles[i][1]]);
			mesh.PositionData.push_back(vertices[triangles[i][2]]);
			
			verticesBoneWeight.push_back(localVerticesBoneWeight[triangles[i][0]]);
			verticesBoneWeight.push_back(localVerticesBoneWeight[triangles[i][1]]);
			verticesBoneWeight.push_back(localVerticesBoneWeight[triangles[i][2]]);

			mesh.TexCoordData.push_back(uvCoords[i][0]);
			mesh.TexCoordData.push_back(uvCoords[i][1]);
			mesh.TexCoordData.push_back(uvCoords[i][2]);
		}

		startMesh = mesh;
	}

	void BoneSystem::Interpolate(int frame)
	{
		int startingFrame = -1;
		for (int i = 0; i < animations[0].keyFrames.size() - 1; i++)
		{
			int oldFrame = animations[0].keyFrames[i].frame;
			int nextFrame = animations[0].keyFrames[i + 1].frame;
			if (frame >= oldFrame && frame < nextFrame)
			{
				startingFrame = i;
				break;
			}
		}

		if (startingFrame == -1)
		{
			throw std::runtime_error("Exception here");
		}

		int modifiedFrameNumber = frame - animations[0].keyFrames[startingFrame].frame;

		int diffFrames = animations[0].keyFrames[startingFrame + 1].frame - animations[0].keyFrames[startingFrame].frame;

		float t = (modifiedFrameNumber + 0.f) / diffFrames;

		std::vector<Bone>& oneFrameBones = animations[0].keyFrames[startingFrame].bones;
		std::vector<Bone>& nextFrameBones = animations[0].keyFrames[startingFrame+1].bones;

		std::vector<Matrix4f> skinningMatrixForEachBone;
		//std::vector<Matrix3f> skinningMatrixForEachBone;
		skinningMatrixForEachBone.resize(currentBones.size());

		
		for (int i = 0; i < currentBones.size(); i++)
		{
			currentBones[i].boneStartWorld.v[0] = oneFrameBones[i].boneStartWorld.v[0] + t * (nextFrameBones[i].boneStartWorld.v[0] - oneFrameBones[i].boneStartWorld.v[0]);
			currentBones[i].boneStartWorld.v[1] = oneFrameBones[i].boneStartWorld.v[1] + t * (nextFrameBones[i].boneStartWorld.v[1] - oneFrameBones[i].boneStartWorld.v[1]);
			currentBones[i].boneStartWorld.v[2] = oneFrameBones[i].boneStartWorld.v[2] + t * (nextFrameBones[i].boneStartWorld.v[2] - oneFrameBones[i].boneStartWorld.v[2]);
			
			Matrix3f oneFrameBonesMatrix;

			oneFrameBonesMatrix.m[0] = oneFrameBones[i].boneMatrixWorld.m[0];
			oneFrameBonesMatrix.m[1] = oneFrameBones[i].boneMatrixWorld.m[1];
			oneFrameBonesMatrix.m[2] = oneFrameBones[i].boneMatrixWorld.m[2];

			oneFrameBonesMatrix.m[3] = oneFrameBones[i].boneMatrixWorld.m[0 + 1*4];
			oneFrameBonesMatrix.m[4] = oneFrameBones[i].boneMatrixWorld.m[1 + 1*4];
			oneFrameBonesMatrix.m[5] = oneFrameBones[i].boneMatrixWorld.m[2 + 1*4];

			oneFrameBonesMatrix.m[6] = oneFrameBones[i].boneMatrixWorld.m[0 + 2*4];
			oneFrameBonesMatrix.m[7] = oneFrameBones[i].boneMatrixWorld.m[1 + 2*4];
			oneFrameBonesMatrix.m[8] = oneFrameBones[i].boneMatrixWorld.m[2 + 2*4];
			
			Matrix3f nextFrameBonesMatrix;

			nextFrameBonesMatrix.m[0] = nextFrameBones[i].boneMatrixWorld.m[0];
			nextFrameBonesMatrix.m[1] = nextFrameBones[i].boneMatrixWorld.m[1];
			nextFrameBonesMatrix.m[2] = nextFrameBones[i].boneMatrixWorld.m[2];

			nextFrameBonesMatrix.m[3] = nextFrameBones[i].boneMatrixWorld.m[0 + 1 * 4];
			nextFrameBonesMatrix.m[4] = nextFrameBones[i].boneMatrixWorld.m[1 + 1 * 4];
			nextFrameBonesMatrix.m[5] = nextFrameBones[i].boneMatrixWorld.m[2 + 1 * 4];

			nextFrameBonesMatrix.m[6] = nextFrameBones[i].boneMatrixWorld.m[0 + 2 * 4];
			nextFrameBonesMatrix.m[7] = nextFrameBones[i].boneMatrixWorld.m[1 + 2 * 4];
			nextFrameBonesMatrix.m[8] = nextFrameBones[i].boneMatrixWorld.m[2 + 2 * 4];

			Vector4f q1 = MatrixToQuat(oneFrameBonesMatrix);
			Vector4f q2 = MatrixToQuat(nextFrameBonesMatrix);
			Vector4f q1_norm = q1.normalized();
			Vector4f q2_norm = q2.normalized();

			Vector4f result = slerp(q1_norm, q2_norm, t);

			Matrix3f boneMatrixWorld3 = QuatToMatrix(result);

			currentBones[i].boneMatrixWorld = MakeMatrix4x4(boneMatrixWorld3, currentBones[i].boneStartWorld);
			
			Matrix4f currentBoneMatrixWorld4 = currentBones[i].boneMatrixWorld;
			Matrix4f startBoneMatrixWorld4 = animations[0].keyFrames[0].bones[i].boneMatrixWorld;

			Matrix4f inverstedStartBoneMatrixWorld4 = InverseMatrix(startBoneMatrixWorld4);

			skinningMatrixForEachBone[i] = MultMatrixMatrix(currentBoneMatrixWorld4, inverstedStartBoneMatrixWorld4);
			
		}
		
		
		/*
		for (int i = 0; i < currentBones.size(); i++)
		{
			currentBones[i].boneStartWorld = oneFrameBones[i].boneStartWorld;
			currentBones[i].boneMatrixWorld = oneFrameBones[i].boneMatrixWorld;
			//Matrix4f currentBoneMatrixWorld4 = MakeMatrix4x4(currentBones[i].boneMatrixWorld, currentBones[i].boneStartWorld);
			//Matrix4f startBoneMatrixWorld4 = MakeMatrix4x4(animations[0].keyFrames[0].bones[i].boneMatrixWorld, animations[0].keyFrames[0].bones[i].boneStartWorld);
			Matrix4f currentBoneMatrixWorld4 = currentBones[i].boneMatrixWorld;
			Matrix4f startBoneMatrixWorld4 = animations[0].keyFrames[0].bones[i].boneMatrixWorld;
			Matrix4f inverstedStartBoneMatrixWorld4 = InverseMatrix(startBoneMatrixWorld4);
			skinningMatrixForEachBone[i] = MultMatrixMatrix(currentBoneMatrixWorld4, inverstedStartBoneMatrixWorld4);

			if (i == 10)
			{
				std::cout << i << std::endl;
			}
		}*/
		
		for (int i = 0; i < mesh.PositionData.size(); i++)
		{
			Vector4f originalPos = { 
				startMesh.PositionData[i].v[0], 
				startMesh.PositionData[i].v[1], 
				startMesh.PositionData[i].v[2], 1.0};

			Vector4f finalPos = Vector4f{0.f, 0.f, 0.f, 0.f};

			bool vMoved = false;
			//Vector3f finalPos = Vector3f{ 0.f, 0.f, 0.f };

			for (int j = 0; j < MAX_BONE_COUNT; j++)
			{
				if (verticesBoneWeight[i][j].weight != 0)
				{
					vMoved = true;
					//finalPos = finalPos + MultVectorMatrix(originalPos, skinningMatrixForEachBone[verticesBoneWeight[i][j].boneIndex]) * verticesBoneWeight[i][j].weight;
					finalPos = finalPos + MultMatrixVector(skinningMatrixForEachBone[verticesBoneWeight[i][j].boneIndex], originalPos) * verticesBoneWeight[i][j].weight;
				}
			}

			if (abs(finalPos.v[0] - originalPos.v[0]) > 1 || abs(finalPos.v[1] - originalPos.v[1]) > 1 || abs(finalPos.v[2] - originalPos.v[2]) > 1)
			{

			}

			if (!vMoved)
			{
				finalPos = originalPos;
			}

			mesh.PositionData[i].v[0] = finalPos.v[0];
			mesh.PositionData[i].v[1] = finalPos.v[1];
			mesh.PositionData[i].v[2] = finalPos.v[2];
		}




	}


}