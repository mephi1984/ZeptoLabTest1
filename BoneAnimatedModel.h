#pragma once
#include "Math.h"
#include "Renderer.h"
#include <unordered_map>


namespace ZL
{
	struct Bone
	{
		Vector3f boneStartWorld;
		float boneLength;
		Matrix3f boneMatrixWorld;
		// boneVector = boneLength * (0, 1, 0) в осях блендера
		// Then multiply by boneMatrixWorld и вы получите конечную точку

		int parent;
		std::vector<int> children;
	};

	struct BoneWeight
	{
		int boneIndex = -1;
		float weight = 0;
	};

	struct AnimationKeyFrame
	{
		int frame;
		std::vector<Bone> bones;
	};

	struct Animation
	{
		std::vector<AnimationKeyFrame> keyFrames;
	};

	struct BoneSystem
	{
		VertexDataStruct mesh;
		VertexDataStruct startMesh;
		std::vector<std::array<BoneWeight, 3>> verticesBoneWeight;

		std::vector<Bone> startBones;
		std::vector<Bone> currentBones;

		std::vector<Animation> animations;

		void LoadFromFile(const std::string& fileName);

		void Interpolate(int frame);
	};




};