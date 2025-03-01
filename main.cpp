#include "TextureManager.h"
#include "ShaderManager.h"
#include "Renderer.h"
#include "ObjLoader.h"

#include "Physics.h"
#include <string>


#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <fstream>

#include "Game.h"
#include "AnimatedModel.h"
#include "BoneAnimatedModel.h"

ZL::AnimatedModel testLoadModel();

namespace ZL
{

	static SDL_Window* window = NULL;
	static SDL_GLContext gl_context;

	Renderer renderer;

	GameState gs;


	const size_t CONST_TIMER_INTERVAL = 10;

	const size_t CONST_MAX_TIME_INTERVAL = 1000;

	bool ExitGameLoop = false;

	//To calculate when to call Update
	size_t NewTickCount;
	size_t LastTickCount;


	void DrawBackground()
	{
		renderer.PushMatrix();

		renderer.LoadIdentity();

		//renderer.TranslateMatrix({ -gs.backgroundShift, 0.0f, 0.f });

		glBindTexture(GL_TEXTURE_2D, GameObjects::backgroundTexturePtr->getTexID());

		renderer.DrawVertexRenderStruct(GameObjects::backgroundMesh);

		renderer.PopMatrix();
	}

	void DrawBird()
	{
		renderer.PushMatrix();

		renderer.LoadIdentity();

		Vector2f birdScreenShift = gs.birdCurrentPos - Env::birdStartPos;

		renderer.TranslateMatrix({ 200.f + birdScreenShift.v[0], Env::getActualClientHeight() * 0.5f + birdScreenShift.v[1], 0.f });

		renderer.RotateMatrix(QuatFromRotateAroundZ(gs.birdAngle));

		glBindTexture(GL_TEXTURE_2D, GameObjects::birdTexturePtr->getTexID());

		renderer.DrawVertexRenderStruct(GameObjects::birdMesh);

		renderer.PopMatrix();
	}

	void DrawPipes()
	{
		for (auto& pipePair : gs.pipePairArr)
		{
			glBindTexture(GL_TEXTURE_2D, GameObjects::pipeTexturePtr->getTexID());


			//Draw bottom pipe:
			renderer.PushMatrix();

			renderer.LoadIdentity();

			renderer.TranslateMatrix({ (pipePair.xPos), pipePair.bottomPipeVShift, 0.f });

			renderer.DrawVertexRenderStruct(GameObjects::pipeMesh);


			//Draw top pipe:

			renderer.LoadIdentity();

			renderer.TranslateMatrix({ (pipePair.xPos), Env::getActualClientHeight() + pipePair.topPipeVShift, 0.f });

			renderer.ScaleMatrix({ 1.f, -1.f, 1.f });

			renderer.DrawVertexRenderStruct(GameObjects::pipeMesh);

			renderer.PopMatrix();
		}
	}

	void DrawUi()
	{
		if (gs.isGameOver)
		{
			renderer.PushMatrix();

			renderer.LoadIdentity();

			glBindTexture(GL_TEXTURE_2D, GameObjects::gameOverTexturePtr->getTexID());

			renderer.DrawVertexRenderStruct(GameObjects::gameOverMesh);

			renderer.PopMatrix();
		}
	}

	void DrawScene()
	{
		static const std::string defaultShaderName = "default";
		static const std::string colorShaderName = "defaultColor";

		static const std::string vPositionName = "vPosition";
		static const std::string vTexCoordName = "vTexCoord";
		static const std::string vColorName = "vColor";
		static const std::string textureUniformName = "Texture";

		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glViewport(0, 0, Env::width, Env::height);

		//renderer.shaderManager.PushShader(defaultShaderName);
		renderer.shaderManager.PushShader(colorShaderName);
		//renderer.RenderUniform1i(textureUniformName, 0);

		renderer.EnableVertexAttribArray(vPositionName);
		//renderer.EnableVertexAttribArray(vTexCoordName);

		renderer.PushPerspectiveProjectionMatrix(1.0 / 6.0, static_cast<float>(Env::width) / static_cast<float>(Env::height), 0.1, 1000);
		renderer.PushMatrix();

		renderer.LoadIdentity();

		renderer.TranslateMatrix({ 0,0, -200 });

		renderer.RotateMatrix(QuatFromRotateAroundX(-M_PI / 2.0));
		//renderer.RotateMatrix(QuatFromRotateAroundZ(-M_PI / 4.0));

		/*
		renderer.RotateMatrix(QuatFromRotateAroundX(-M_PI / 3.0));


		for (int i = 0; i < GameObjects::testmd3.parts.size(); i++)
		{

			for (int j = 0; j < GameObjects::testmd3.parts[i].renderMeshes.size(); j++)
			{
				
				glBindTexture(GL_TEXTURE_2D, GameObjects::testmd3.parts[i].textures[0]->getTexID());
				renderer.DrawVertexRenderStruct(GameObjects::testmd3.parts[i].renderMeshes[j]);
			}
		}
		*/
		/*
		GameObjects::testObjMeshMutable.AssignFrom(GameObjects::testObjMesh);
		GameObjects::testObjMeshMutable.data.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundZ(gs.rotateTimer * M_PI / 3.0)));
		GameObjects::testObjMeshMutable.RefreshVBO();*/


		//GameObjects::testmd3mutable[0].RefreshVBO();
		//GameObjects::testmd3mutable[1].RefreshVBO();

		//glBindTexture(GL_TEXTURE_2D, GameObjects::testObjTexturePtr->getTexID());	
		//renderer.DrawVertexRenderStruct(GameObjects::testObjMeshMutable);

		/*
		glBindTexture(GL_TEXTURE_2D, GameObjects::md3TexturePtr->getTexID());
		renderer.DrawVertexRenderStruct(GameObjects::testmd3mutable[0]);
		renderer.DrawVertexRenderStruct(GameObjects::testmd3mutable[1]);
		*/

		GameObjects::bxMutable.AssignFrom(GameObjects::bx.mesh);
		GameObjects::bxMutable.RefreshVBO();
		renderer.DrawVertexRenderStruct(GameObjects::bxMutable);

		renderer.PopMatrix();

		renderer.PopProjectionMatrix();

		renderer.DisableVertexAttribArray(vPositionName);
		//renderer.DisableVertexAttribArray(vTexCoordName);

		renderer.shaderManager.PopShader();


		/*
		renderer.shaderManager.PushShader(colorShaderName);

		//renderer.RenderUniform1i(textureUniformName, 0);

		renderer.EnableVertexAttribArray(vPositionName);

		renderer.EnableVertexAttribArray(vColorName);

		renderer.PushPerspectiveProjectionMatrix(1.0 / 6.0, static_cast<float>(Env::width)/ static_cast<float>(Env::height), 10, 10000);
		renderer.PushMatrix();

		renderer.LoadIdentity();

		renderer.TranslateMatrix({ 0,0, -1000 });

		
		renderer.RotateMatrix(QuatFromRotateAroundX(-M_PI / 3.0));

		GameObjects::colorCubeMeshMutable.AssignFrom(GameObjects::colorCubeMesh);
		GameObjects::colorCubeMeshMutable.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundZ(gs.rotateTimer * M_PI / 3.0)));

		renderer.DrawVertexRenderStruct(GameObjects::colorCubeMeshMutable);

		renderer.PopMatrix();
				
		renderer.PopProjectionMatrix();

		renderer.DisableVertexAttribArray(vColorName);
		renderer.DisableVertexAttribArray(vPositionName);

		renderer.shaderManager.PopShader();*/
		
		CheckGlError();

	}

	

	void ProcessTickCount()
	{

		if (LastTickCount == 0)
		{
			LastTickCount = SDL_GetTicks64();
			return;
		}

		NewTickCount = SDL_GetTicks64();
		if (NewTickCount - LastTickCount > CONST_TIMER_INTERVAL)
		{
			if (NewTickCount - LastTickCount > CONST_MAX_TIME_INTERVAL)
			{
				gs.UpdateScene(CONST_MAX_TIME_INTERVAL); //Limit game update speed to FPS
			}
			else
			{
				gs.UpdateScene(NewTickCount - LastTickCount);
			}
			
			LastTickCount = NewTickCount;
		}

	}

	void setup() {
		static const int BIRD_WIDTH = 512;
		static const int BIRD_HEIGHT = 512;

		static const int BACKGROUND_WIDTH = 900;
		static const int BACKGROUND_HEIGHT = 504;

		static const int PIPE_WIDTH = 244;
		static const int PIPE_HEIGHT = 1500;

		static const int GAMEOVER_WIDTH = 1400;
		static const int GAMEOVER_HEIGHT = 720;

		ZL::BindOpenGlFunctions();

		CheckGlError();

		GameObjects::testmd3 = testLoadModel();

		/*
		GameObjects::testmd3mutable.resize(GameObjects::testmd3.size());

		for (int i = 0; i < GameObjects::testmd3.size(); i++)
		{
			GameObjects::testmd3[i].Scale(0.01);

			GameObjects::testmd3mutable[i].data = GameObjects::testmd3[i];
			GameObjects::testmd3mutable[i].RefreshVBO();
		}*/

		//Load shaders:
		renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");
		renderer.shaderManager.AddShaderFromFiles("defaultColor", "./defaultColor.vertex", "./defaultColor.fragment");
		
		//Load textures
		GameObjects::birdTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./bird.bmp32"));
		GameObjects::backgroundTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./background.bmp"));
		GameObjects::pipeTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./pipe.bmp32"));
		GameObjects::gameOverTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./game_over.bmp32"));
		//GameObjects::testObjTexturePtr = std::make_shared<Texture>(CreateTextureDataFromPng("./chair_01_Base_Color.png"));
		GameObjects::testObjTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./chair_01_Base_Color.bmp"));

		//GameObjects::md3TexturePtr = std::make_shared<Texture>(CreateTextureDataFromPng("./model/sarge/band.png"));

		GameObjects::bx.LoadFromFile("mesh_armature_and_animation_data.txt");
		//GameObjects::bx.LoadFromFile("C:\\Work\\GameJam2025-02\\mesh_armature_and_animation_data02.txt");


		CheckGlError();
		//Create bird mesh
		GameObjects::birdMesh.data = CreateRect2D({ 0.f, 0.f }, { GameConsts::birdScale * BIRD_WIDTH, GameConsts::birdScale * BIRD_HEIGHT }, 0);
		GameObjects::birdMesh.RefreshVBO();

		float backgroundTextureScale = Env::height / static_cast<float>(BACKGROUND_HEIGHT);


		//Create pipe mesh
		GameObjects::pipeMesh.data = CreateRect2D({ PIPE_WIDTH * GameConsts::pipeScale * (-0.5f), Env::getActualClientHeight() * 0.5f + PIPE_HEIGHT * GameConsts::pipeScale * (-0.5f) }, { PIPE_WIDTH * GameConsts::pipeScale * 0.5f, PIPE_HEIGHT * GameConsts::pipeScale * 0.5f }, 0);
		GameObjects::pipeMesh.RefreshVBO();

		//Create background mesh depending on screen size

		Env::backgroundSectionWidth = BACKGROUND_WIDTH * backgroundTextureScale;

		GameObjects::backgroundMesh.data = CreateRectHorizontalSections2D({ BACKGROUND_WIDTH * backgroundTextureScale * (0.5f), BACKGROUND_HEIGHT * backgroundTextureScale * (0.5f) }, { BACKGROUND_WIDTH * backgroundTextureScale * 0.5f, BACKGROUND_HEIGHT * backgroundTextureScale * 0.5f }, -0.5, 2);
		GameObjects::backgroundMesh.RefreshVBO();
		CheckGlError();

		//Create Game Over UI mesh depending on screen size

		float gameOverTextureScale = Env::height / static_cast<float>(GAMEOVER_HEIGHT);

		GameObjects::gameOverMesh.data = CreateRect2D({ GAMEOVER_WIDTH * gameOverTextureScale * 0.5f, GAMEOVER_HEIGHT * gameOverTextureScale * 0.5f }, { GAMEOVER_WIDTH * gameOverTextureScale * 0.5f, GAMEOVER_HEIGHT * gameOverTextureScale * 0.5f }, 0.1f);
		GameObjects::gameOverMesh.RefreshVBO();

		GameObjects::colorCubeMesh = CreateCube3D(5.0);
		GameObjects::colorCubeMeshMutable.data = CreateCube3D(5.0);
		GameObjects::colorCubeMeshMutable.RefreshVBO();

		GameObjects::testObjMesh = LoadFromObjFile("./chair_01.obj");
		GameObjects::testObjMesh.Scale(10);
		GameObjects::testObjMesh.SwapZandY();
		GameObjects::testObjMeshMutable.data = GameObjects::testObjMesh;
		GameObjects::testObjMeshMutable.RefreshVBO();

		//Set some game values
		Env::birdStartPos = { Env::width * 0.2f, Env::getActualClientHeight() * 0.5f };

		//Exact value depends on the bird look texture and must be calculated manually:
		gs.birdEllipse.a = GameConsts::birdScale * 236.f;
		gs.birdEllipse.b = GameConsts::birdScale * 160.f;

		gs.RestartGame();

		renderer.InitOpenGL();

		CheckGlError();
	}

	void render() {

		SDL_GL_MakeCurrent(window, gl_context);

		CheckGlError();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawScene();
		ProcessTickCount();

		SDL_GL_SwapWindow(window);

	}

	void update() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				ExitGameLoop = true;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				static int x = 0;

				GameObjects::bx.Interpolate(x);
				x = x + 2;
				/*
				if (gs.isGameOver)
				{
					gs.RestartGame();
				}
				else
				{
					gs.BirdJump();
				}*/
			}
		}

		render();

	};

};

int main(int argc, char* argv[])
{

	constexpr int CONST_WIDTH = 1280;
	constexpr int CONST_HEIGHT = 720;

	ZL::Env::width = CONST_WIDTH;
	ZL::Env::height = CONST_HEIGHT;

#ifdef EMSCRIPTEN
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Renderer* renderer = NULL;
	SDL_CreateWindowAndRenderer(CONST_WIDTH, CONST_HEIGHT, SDL_WINDOW_OPENGL, &ZL::window, &renderer);
#else
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}


	// Use a core profile setup.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	ZL::window = SDL_CreateWindow("Jumping Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CONST_WIDTH, CONST_HEIGHT, SDL_WINDOW_OPENGL);
#endif
	//todo
	ZL::Env::windowHeaderHeight = 0;

	ZL::gl_context = SDL_GL_CreateContext(ZL::window);

	ZL::CheckGlError();

	ZL::setup();
#ifdef EMSCRIPTEN
	// register update as callback
	emscripten_set_main_loop(ZL::update, 0, 1);
#else
	while (!ZL::ExitGameLoop) {

		ZL::update();
		SDL_Delay(2);

	}
	SDL_GL_DeleteContext(ZL::gl_context);
	SDL_DestroyWindow(ZL::window);
	SDL_Quit();

	exit(0);
#endif

}
