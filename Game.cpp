#include "Game.h"

namespace ZL
{
	namespace Env
	{
		int windowHeaderHeight = 0;

		int width = 0;
		int height = 0;

		Vector2f birdStartPos;

		float backgroundSectionWidth;

		int getActualClientHeight()
		{
			return height - windowHeaderHeight;
		}
	}

	namespace GameObjects
	{
		std::shared_ptr<Texture> birdTexturePtr;
		std::shared_ptr<Texture> backgroundTexturePtr;
		std::shared_ptr<Texture> pipeTexturePtr;
		std::shared_ptr<Texture> gameOverTexturePtr;

		VertexDataStruct birdMesh;

		VertexDataStruct backgroundMesh;

		VertexDataStruct pipeMesh;

		VertexDataStruct gameOverMesh;
	}


	PipePairConfig PipePairConfig::CreateTube(float xPos)
	{
		PipePairConfig r;

		int vShift = rand() % (Env::getActualClientHeight() / 2) - Env::getActualClientHeight() / 4;

		int holeSize = rand() % (Env::getActualClientHeight() / 4) + Env::getActualClientHeight() / 4;

		r.topPipeVShift = holeSize * 0.5f + vShift;
		r.bottomPipeVShift = -holeSize * 0.5f + vShift;

		r.xPos = xPos;


		// Create pipe lines to check intersection
		// There are 2 pipes, each consist of 3 lines, total 6 as indexed below:

		/*
		  | |
		3 | | 5
		  ---
		   4


		   1
		   __
		  |  |
		0 |  |  2
		*/


		float xRight = r.xPos - GameConsts::pipeScale * GameObjects::pipeTexturePtr->getWidth() * 0.5f;
		float xLeft = r.xPos + GameConsts::pipeScale * GameObjects::pipeTexturePtr->getWidth() * 0.5f;

		float yUpBottomPipe = (Env::getActualClientHeight() * 0.5f + r.bottomPipeVShift);
		float yDownBottomPipe = yUpBottomPipe - GameConsts::pipeScale * GameObjects::pipeTexturePtr->getHeight();

		float yUpTopPipe = (Env::getActualClientHeight() * 0.5f + r.topPipeVShift);
		float yDownTopPipe = yUpTopPipe + GameConsts::pipeScale * GameObjects::pipeTexturePtr->getHeight();

		r.tubePhysicsLines[0].start.v[0] = xRight;
		r.tubePhysicsLines[0].end.v[0] = xRight;

		r.tubePhysicsLines[0].start.v[1] = yDownBottomPipe;
		r.tubePhysicsLines[0].end.v[1] = yUpBottomPipe;


		r.tubePhysicsLines[1].start.v[0] = xRight;
		r.tubePhysicsLines[1].end.v[0] = xLeft;

		r.tubePhysicsLines[1].start.v[1] = yUpBottomPipe;
		r.tubePhysicsLines[1].end.v[1] = yUpBottomPipe;

		r.tubePhysicsLines[2].start.v[0] = xLeft;
		r.tubePhysicsLines[2].end.v[0] = xLeft;

		r.tubePhysicsLines[2].start.v[1] = yUpBottomPipe;
		r.tubePhysicsLines[2].end.v[1] = yDownBottomPipe;


		r.tubePhysicsLines[3].start.v[0] = xRight;
		r.tubePhysicsLines[3].end.v[0] = xRight;

		r.tubePhysicsLines[3].start.v[1] = yUpTopPipe;
		r.tubePhysicsLines[3].end.v[1] = yDownTopPipe;


		r.tubePhysicsLines[4].start.v[0] = xRight;
		r.tubePhysicsLines[4].end.v[0] = xLeft;

		r.tubePhysicsLines[4].start.v[1] = yDownTopPipe;
		r.tubePhysicsLines[4].end.v[1] = yDownTopPipe;

		r.tubePhysicsLines[5].start.v[0] = xLeft;
		r.tubePhysicsLines[5].end.v[0] = xLeft;

		r.tubePhysicsLines[5].start.v[1] = yDownTopPipe;
		r.tubePhysicsLines[5].end.v[1] = yUpTopPipe;

		return r;
	}

	PipePairConfig PipePairConfig::CreateLastTube()
	{
		return PipePairConfig::CreateTube(Env::width * 1.6f);
	}


	void GameState::UpdateBirdPos(size_t tickCountDiff)
	{
		birdCurrentPos.v[1] = birdCurrentPos.v[1] + yVelocity * tickCountDiff;

		yVelocity += GameConsts::yAcceleration * tickCountDiff;

		birdAngle = -atan(yVelocity / xVelocity);


		if (birdCurrentPos.v[1] <= 0)
		{
			isGameOver = true;
		}

		if (birdCurrentPos.v[1] >= Env::height)
		{
			isGameOver = true;
		}
	}

	void GameState::UpdateBackgroundPos(size_t tickCountDiff)
	{
		backgroundShift += GameConsts::backgroundXVelocity * tickCountDiff;

		if (backgroundShift > Env::backgroundSectionWidth)
		{
			float c = floor(backgroundShift / Env::backgroundSectionWidth);

			backgroundShift -= c * Env::backgroundSectionWidth;
		}
	}

	void GameState::UpdatePipePos(size_t tickCountDiff)
	{
		for (size_t i = 0; i < pipePairArr.size(); )
		{

			pipePairArr[i].xPos += xVelocity * tickCountDiff;

			if (pipePairArr[i].xPos < 0)
			{
				pipePairArr.erase(pipePairArr.begin() + i);

				pipePairArr.push_back(PipePairConfig::CreateLastTube());
			}
			else
			{
				i++;
			}
		}

	}

	void GameState::UpdatePhysics(size_t tickCountDiff)
	{
		birdEllipse.center = birdCurrentPos;

		for (size_t i = 0; i < pipePairArr.size(); i++)
		{

			for (size_t j = 0; j < pipePairArr[i].tubePhysicsLines.size(); j++)
			{

				pipePairArr[i].tubePhysicsLines[j].start.v[0] += xVelocity * tickCountDiff;
				pipePairArr[i].tubePhysicsLines[j].end.v[0] += xVelocity * tickCountDiff;

				if (TestIntersection(pipePairArr[i].tubePhysicsLines[j], birdEllipse))
				{
					isGameOver = true;
					break;
				}
			}
		}
	}

	void GameState::RestartGame()
	{

		birdCurrentPos = Env::birdStartPos;

		birdEllipse.center = birdCurrentPos;

		xVelocity = -0.3f;
		yVelocity = 0.f;

		birdAngle = 0.f;

		backgroundShift = 0.f;

		pipePairArr.clear();

		pipePairArr.push_back(PipePairConfig::CreateTube(Env::width * 0.4f));
		pipePairArr.push_back(PipePairConfig::CreateTube(Env::width * 0.8f));
		pipePairArr.push_back(PipePairConfig::CreateTube(Env::width * 1.2f));
		pipePairArr.push_back(PipePairConfig::CreateTube(Env::width * 1.6f));

		isGameOver = false;
	}

	void GameState::UpdateScene(size_t tickCountDiff)
	{

		if (isGameOver)
		{
			return;
		}

		UpdateBirdPos(tickCountDiff);

		UpdatePipePos(tickCountDiff);

		UpdateBackgroundPos(tickCountDiff);

		UpdatePhysics(tickCountDiff);
	}

	void GameState::BirdJump()
	{
		yVelocity = GameConsts::jumpVelocity;
	}
}