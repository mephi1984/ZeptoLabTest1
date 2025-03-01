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
#include "TextModel.h"

namespace ZL
{

	namespace Env
	{
		int windowHeaderHeight = 0;

		int width = 0;
		int height = 0;
		/*
		Vector2f birdStartPos;

		float backgroundSectionWidth;

		int getActualClientHeight()
		{
			return height - windowHeaderHeight;
		}*/
	}

	namespace GameObjects
	{
		std::shared_ptr<Texture> testObjTexturePtr;
		std::shared_ptr<Texture> roomTexturePtr;
		
		VertexDataStruct colorCubeMesh;
		VertexRenderStruct colorCubeMeshMutable;

		VertexDataStruct testObjMesh;
		VertexRenderStruct testObjMeshMutable;

		BoneSystem bx;
		VertexRenderStruct bxMutable;

		VertexDataStruct textMesh;
		VertexRenderStruct textMeshMutable;

	}

	static SDL_Window* window = NULL;
	static SDL_GLContext gl_context;

	Renderer renderer;

	//GameState gs;


	const size_t CONST_TIMER_INTERVAL = 10;

	const size_t CONST_MAX_TIME_INTERVAL = 1000;

	bool ExitGameLoop = false;

	//To calculate when to call Update
	size_t NewTickCount;
	size_t LastTickCount;


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

		renderer.shaderManager.PushShader(defaultShaderName);
		renderer.RenderUniform1i(textureUniformName, 0);

		renderer.EnableVertexAttribArray(vPositionName);
		renderer.EnableVertexAttribArray(vTexCoordName);

		renderer.PushPerspectiveProjectionMatrix(1.0 / 6.0, static_cast<float>(Env::width) / static_cast<float>(Env::height), 10, 50000);
		renderer.PushMatrix();

		renderer.LoadIdentity();

		renderer.TranslateMatrix({ 0,0, -4000 });

		float t = 0.7;

		renderer.RotateMatrix(QuatFromRotateAroundX(t * M_PI / 2.0));

		GameObjects::textMeshMutable.AssignFrom(GameObjects::textMesh);
		GameObjects::textMeshMutable.RefreshVBO();


		glBindTexture(GL_TEXTURE_2D, GameObjects::roomTexturePtr->getTexID());
		renderer.DrawVertexRenderStruct(GameObjects::textMeshMutable);

		//renderer.RotateMatrix(QuatFromRotateAroundX(-M_PI / 2.0));
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



		//GameObjects::bxMutable.AssignFrom(GameObjects::bx.mesh);
		//GameObjects::bxMutable.RefreshVBO();
		//renderer.DrawVertexRenderStruct(GameObjects::bxMutable);

		renderer.PopMatrix();

		renderer.PopProjectionMatrix();

		renderer.DisableVertexAttribArray(vPositionName);
		renderer.DisableVertexAttribArray(vTexCoordName);

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
				//gs.UpdateScene(CONST_MAX_TIME_INTERVAL); //Limit game update speed to FPS
			}
			else
			{
				//gs.UpdateScene(NewTickCount - LastTickCount);
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

		//Load shaders:
		std::cout << "Hello test 1" << std::endl;
        renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");

		std::cout << "Hello test 2" << std::endl;
		renderer.shaderManager.AddShaderFromFiles("defaultColor", "./defaultColor.vertex", "./defaultColor.fragment");
		std::cout << "Hello test 2x" << std::endl;

		GameObjects::bx.LoadFromFile("mesh_armature_and_animation_data.txt");

		std::cout << "Hello test 3" << std::endl;

		CheckGlError();

		GameObjects::roomTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Kitchen_ceramics.bmp"));
		

		GameObjects::colorCubeMesh = CreateCube3D(5.0);
		GameObjects::colorCubeMeshMutable.data = CreateCube3D(5.0);
		GameObjects::colorCubeMeshMutable.RefreshVBO();

		GameObjects::testObjMesh = LoadFromObjFile("./chair_01.obj");
		GameObjects::testObjMesh.Scale(10);
		GameObjects::testObjMesh.SwapZandY();
		GameObjects::testObjMeshMutable.data = GameObjects::testObjMesh;
		GameObjects::testObjMeshMutable.RefreshVBO();

		GameObjects::textMesh = LoadFromTextFile("./mesh001.txt");

		std::cout << "Hello test 4x" << std::endl;
		
		renderer.InitOpenGL();

		CheckGlError();


		std::cout << "Hello test 5x" << std::endl;
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
