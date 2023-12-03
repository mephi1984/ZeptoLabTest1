#pragma once

#include "Math.h"
#include "Physics.h" 
#include "TextureManager.h"
#include "Renderer.h"
#include <memory>

namespace ZL
{

	namespace GameConsts
	{
		constexpr float yAcceleration = -0.0003f;

		constexpr float backgroundXVelocity = 0.15f;

		constexpr float jumpVelocity = 0.2f;

		constexpr float pipeScale = 0.5f;

		constexpr float birdScale = 0.1f;
	}

	namespace Env
	{
		extern int windowHeaderHeight;

		extern int width;
		extern int height;

		extern Vector2f birdStartPos;

		//Calculated depending on the background texture and screen size
		extern float backgroundSectionWidth;

		int getActualClientHeight();
	}


	struct PipePairConfig
	{
		float topPipeVShift;
		float bottomPipeVShift;

		float xPos;

		std::array<LinePhysicsObject, 6> tubePhysicsLines;

		static PipePairConfig CreateTube(float xPos);

		static PipePairConfig CreateLastTube();
	};

	struct GameState
	{
	protected:
		void UpdateBirdPos(size_t tickCountDiff);

		void UpdateBackgroundPos(size_t tickCountDiff);

		void UpdatePipePos(size_t tickCountDiff);

		void UpdatePhysics(size_t tickCountDiff);

	public:
		
		Vector2f birdCurrentPos = {0.f, 0.f};

		float xVelocity = -0.3f;
		float yVelocity = 0.f;

		float birdAngle = 0.f;

		float backgroundShift = 0.f;

		bool isGameOver = false;

		std::vector<PipePairConfig> pipePairArr;

		EllipsePhysicsObject birdEllipse;

		void RestartGame();

		void UpdateScene(size_t tickCountDiff);

		void BirdJump();
	};

	

	namespace GameObjects
	{
		extern std::shared_ptr<Texture> birdTexturePtr;
		extern std::shared_ptr<Texture> backgroundTexturePtr;
		extern std::shared_ptr<Texture> pipeTexturePtr;
		extern std::shared_ptr<Texture> gameOverTexturePtr;

		extern VertexDataStruct birdMesh;

		extern VertexDataStruct backgroundMesh;

		extern VertexDataStruct pipeMesh;

		extern VertexDataStruct gameOverMesh;
	}
}