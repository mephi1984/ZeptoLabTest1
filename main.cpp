#include "TextureManager.h"
#include "ShaderManager.h"
#include "Renderer.h"

#include "Physics.h"
#include <string>


#include <cstdlib> 
#include <ctime> 
#include <iostream>

#include "Game.h"

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

		renderer.TranslateMatrix({ -gs.backgroundShift, 0.0f, 0.f });

		glBindTexture(GL_TEXTURE_2D, GameObjects::backgroundTexturePtr->getTexID());

		renderer.DrawVertexDataStruct(GameObjects::backgroundMesh);

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

		renderer.DrawVertexDataStruct(GameObjects::birdMesh);

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

			renderer.DrawVertexDataStruct(GameObjects::pipeMesh);


			//Draw top pipe:

			renderer.LoadIdentity();

			renderer.TranslateMatrix({ (pipePair.xPos), Env::getActualClientHeight() + pipePair.topPipeVShift, 0.f });

			renderer.ScaleMatrix({ 1.f, -1.f, 1.f });

			renderer.DrawVertexDataStruct(GameObjects::pipeMesh);

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

			renderer.DrawVertexDataStruct(GameObjects::gameOverMesh);

			renderer.PopMatrix();
		}
	}

	void DrawScene()
	{
		static const std::string defaultShaderName = "default";
		static const std::string vPositionName = "vPosition";
		static const std::string vTexCoordName = "vTexCoord";
		static const std::string textureUniformName = "Texture";

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, Env::width, Env::height);

		renderer.shaderManager.PushShader(defaultShaderName);

		renderer.RenderUniform1i(textureUniformName, 0);

		renderer.EnableVertexAttribArray(vPositionName);

		renderer.EnableVertexAttribArray(vTexCoordName);

		renderer.PushProjectionMatrix(static_cast<float>(Env::width), static_cast<float>(Env::height));

		DrawBackground();
	
		DrawPipes();

		DrawBird();

		DrawUi();

		renderer.PopProjectionMatrix();

		renderer.DisableVertexAttribArray(vTexCoordName);
		renderer.DisableVertexAttribArray(vPositionName);

		renderer.shaderManager.PopShader();

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


		ZL::BindOpenGlFunctions();

		CheckGlError();

		//Load shaders:
		renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");

		//Load textures
		GameObjects::birdTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./bird.bmp32"));
		GameObjects::backgroundTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./background.bmp"));
		GameObjects::pipeTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./pipe.bmp32"));
		GameObjects::gameOverTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./game_over.bmp32"));


		CheckGlError();
		//Create bird mesh
		GameObjects::birdMesh = CreateRect2D({ 0.f, 0.f }, { GameConsts::birdScale * GameObjects::birdTexturePtr->getWidth(), GameConsts::birdScale * GameObjects::birdTexturePtr->getHeight() }, 0);

		float backgroundTextureScale = Env::height / static_cast<float>(GameObjects::backgroundTexturePtr->getHeight());


		//Create pipe mesh
		GameObjects::pipeMesh = CreateRect2D({ GameObjects::pipeTexturePtr->getWidth() * GameConsts::pipeScale * (-0.5f), Env::getActualClientHeight() * 0.5f + GameObjects::pipeTexturePtr->getHeight() * GameConsts::pipeScale * (-0.5f) }, { GameObjects::pipeTexturePtr->getWidth() * GameConsts::pipeScale * 0.5f, GameObjects::pipeTexturePtr->getHeight() * GameConsts::pipeScale * 0.5f }, 0);


		//Create background mesh depending on screen size

		Env::backgroundSectionWidth = GameObjects::backgroundTexturePtr->getWidth() * backgroundTextureScale;

		GameObjects::backgroundMesh = CreateRectHorizontalSections2D({ GameObjects::backgroundTexturePtr->getWidth() * backgroundTextureScale * (0.5f), GameObjects::backgroundTexturePtr->getHeight() * backgroundTextureScale * (0.5f) }, { GameObjects::backgroundTexturePtr->getWidth() * backgroundTextureScale * 0.5f, GameObjects::backgroundTexturePtr->getHeight() * backgroundTextureScale * 0.5f }, 0, 2);

		CheckGlError();

		//Create Game Over UI mesh depending on screen size

		float gameOverTextureScale = Env::height / static_cast<float>(GameObjects::gameOverTexturePtr->getHeight());

		GameObjects::gameOverMesh = CreateRect2D({ GameObjects::gameOverTexturePtr->getWidth() * gameOverTextureScale * 0.5f, GameObjects::gameOverTexturePtr->getHeight() * gameOverTextureScale * 0.5f }, { GameObjects::gameOverTexturePtr->getWidth() * gameOverTextureScale * 0.5f, GameObjects::gameOverTexturePtr->getHeight() * gameOverTextureScale * 0.5f }, 0.1f);



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

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
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
				if (gs.isGameOver)
				{
					gs.RestartGame();
				}
				else
				{
					gs.BirdJump();
				}
			}
		}

		render();

	};

};

int main(int argc, char* argv[])
{
#ifdef EMSCRIPTEN
	SDL_Renderer* renderer = NULL;
	SDL_CreateWindowAndRenderer(512, 512, SDL_WINDOW_OPENGL, &ZL::window, &renderer);
#else
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	constexpr int CONST_WIDTH = 1280;
	constexpr int CONST_HEIGHT = 720;

	ZL::Env::width = CONST_WIDTH;
	ZL::Env::height = CONST_HEIGHT;

	

	// Use a core profile setup.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	ZL::window = SDL_CreateWindow("Jumping Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CONST_WIDTH, CONST_HEIGHT, SDL_WINDOW_OPENGL);

	//todo
	ZL::Env::windowHeaderHeight = 0;

	ZL::gl_context = SDL_GL_CreateContext(ZL::window);

	ZL::CheckGlError();

	
#endif
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
