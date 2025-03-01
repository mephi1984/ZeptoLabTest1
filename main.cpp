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

#include "Inventory.h"
#include "cmakeaudioplayer/include/AudioPlayer.hpp"
#include <memory>

namespace ZL
{
	void worldToScreenCoordinates(Vector3f objectPos,
		Matrix4f projectionModelView,
		int screenWidth, int screenHeight,
		int& screenX, int& screenY) {

		
		Vector4f inx = { objectPos.v[0], objectPos.v[1], objectPos.v[2], 1.0f};

		Vector4f clipCoords = MultMatrixVector(projectionModelView, inx);

		// ������������� �������
		float ndcX = clipCoords.v[0] / clipCoords.v[3];
		float ndcY = clipCoords.v[1] / clipCoords.v[3];

		// ����������� NDC � �������� ����������
		screenX = (int)((ndcX + 1.0f) * 0.5f * screenWidth);
		//screenY = (int)((1.0f - ndcY) * 0.5f * screenHeight);
		screenY = (int)((1.0f + ndcY) * 0.5f * screenHeight);
	}

	namespace Env
	{
		int windowHeaderHeight = 0;

		int width = 0;
		int height = 0;

		float zoom = 10.0;

		bool leftPressed = false;
		bool rightPressed = false;
		bool upPressed = false;
		bool downPressed = false;

		Vector3f cameraShift = {0,0,0};
		Vector3f characterPos = { 0,0,0 };

		/*
		Vector2f birdStartPos;

		float backgroundSectionWidth;

		int getActualClientHeight()
		{
			return height - windowHeaderHeight;
		}*/
	}

	struct ActiveObject
	{
		std::shared_ptr<Texture> activeObjectTexturePtr;
		VertexDataStruct activeObjectMesh;
		VertexRenderStruct activeObjectMeshMutable;

		std::shared_ptr<Texture> activeObjectScreenTexturePtr;
		VertexDataStruct activeObjectScreenMesh;
		VertexRenderStruct activeObjectScreenMeshMutable;


		Vector3f objectPos;
		bool highlighted = false;
	};

	namespace GameObjects
	{
		std::shared_ptr<Texture> testObjTexturePtr;
		std::shared_ptr<Texture> roomTexturePtr;
		std::shared_ptr<Texture> coneTexturePtr;
		//std::shared_ptr<Texture> activeObjectTexturePtr;

		VertexDataStruct colorCubeMesh;
		VertexRenderStruct colorCubeMeshMutable;

		VertexDataStruct testObjMesh;
		VertexRenderStruct testObjMeshMutable;

		BoneSystem bx;
		VertexRenderStruct bxMutable;

		VertexDataStruct textMesh;
		VertexRenderStruct textMeshMutable;

		VertexDataStruct coneMesh;
		VertexRenderStruct coneMeshMutable;

		std::vector<ActiveObject> activeObjects;

		// Add AudioPlayer instance
        std::unique_ptr<AudioPlayer> audioPlayer;

		// Добавляем структуры для иконок инвентаря
		VertexDataStruct inventoryIconMesh;
		VertexRenderStruct inventoryIconMeshMutable;
		const float INVENTORY_ICON_SIZE = 32.0f; // Размер иконки в пикселях
		const float INVENTORY_MARGIN = 10.0f;    // Отступ от края экрана
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

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glViewport(0, 0, Env::width, Env::height);

		renderer.shaderManager.PushShader(defaultShaderName);
		renderer.RenderUniform1i(textureUniformName, 0);

		renderer.EnableVertexAttribArray(vPositionName);
		renderer.EnableVertexAttribArray(vTexCoordName);

		renderer.PushPerspectiveProjectionMatrix(1.0 / 1.5, static_cast<float>(Env::width) / static_cast<float>(Env::height), 50, 10000);
		renderer.PushMatrix();

		renderer.LoadIdentity();

		renderer.TranslateMatrix({ 0,0, -100* Env::zoom });
	
		float t = 0.3;

		renderer.RotateMatrix(QuatFromRotateAroundX(t * M_PI / 2.0));

		//GameObjects::textMeshMutable.AssignFrom(GameObjects::textMesh);
		//GameObjects::textMeshMutable.RefreshVBO();

		glBindTexture(GL_TEXTURE_2D, GameObjects::coneTexturePtr->getTexID());
		renderer.DrawVertexRenderStruct(GameObjects::coneMeshMutable);

		renderer.TranslateMatrix(Env::cameraShift);


		//ActiveObject ao1;

		for (auto& ao : GameObjects::activeObjects)
		{
			renderer.PushMatrix();
			renderer.TranslateMatrix(ao.objectPos);
			glBindTexture(GL_TEXTURE_2D, ao.activeObjectTexturePtr->getTexID());
			renderer.DrawVertexRenderStruct(ao.activeObjectMeshMutable);
			renderer.PopMatrix();
		}

		glBindTexture(GL_TEXTURE_2D, GameObjects::roomTexturePtr->getTexID());
		renderer.DrawVertexRenderStruct(GameObjects::textMeshMutable);

		auto latestProjectionModelView = renderer.GetProjectionModelViewMatrix();

		renderer.PopMatrix();

		renderer.PopProjectionMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
		renderer.PushProjectionMatrix(static_cast<float>(Env::width), static_cast<float>(Env::height));
		renderer.PushMatrix();

		renderer.LoadIdentity();

		for (auto& ao : GameObjects::activeObjects)
		{
			if (ao.highlighted)
			{
				int screenX, screenY;
				worldToScreenCoordinates(ao.objectPos, latestProjectionModelView, Env::width, Env::height, screenX, screenY);
				renderer.PushMatrix();
				renderer.TranslateMatrix(Vector3f{screenX + 0.f, screenY + 0.f, 0.0f});
				glBindTexture(GL_TEXTURE_2D, ao.activeObjectScreenTexturePtr->getTexID());
				renderer.DrawVertexRenderStruct(ao.activeObjectScreenMeshMutable);
				renderer.PopMatrix();
			}
		}

		// Отрисовка иконок инвентаря
		const auto& inventory = ReturnInventory();
		for (size_t i = 0; i < inventory.size(); ++i) {
			renderer.PushMatrix();
			float xPos = Env::width - GameObjects::INVENTORY_MARGIN - GameObjects::INVENTORY_ICON_SIZE;
			float yPos = GameObjects::INVENTORY_MARGIN + i * (GameObjects::INVENTORY_ICON_SIZE + GameObjects::INVENTORY_MARGIN);
			renderer.TranslateMatrix(Vector3f{xPos, yPos, 0.0f});
			glBindTexture(GL_TEXTURE_2D, inventory[i].texture->getTexID());
			renderer.DrawVertexRenderStruct(GameObjects::inventoryIconMeshMutable);
			renderer.PopMatrix();
		}

		renderer.PopMatrix();
		renderer.PopProjectionMatrix();

		renderer.DisableVertexAttribArray(vPositionName);
		renderer.DisableVertexAttribArray(vTexCoordName);

		renderer.shaderManager.PopShader();
		
		CheckGlError();

	}

	
	void UpdateScene(size_t ms)
	{
		const float SPEED = 0.1f;
		if (Env::leftPressed)
		{
			Env::cameraShift.v[0] += SPEED * ms;
		}
		if (Env::rightPressed)
		{
			Env::cameraShift.v[0] -= SPEED * ms;
		}

		if (Env::upPressed)
		{
			Env::cameraShift.v[2] += SPEED * ms;
		}
		if (Env::downPressed)
		{
			Env::cameraShift.v[2] -= SPEED * ms;
		}

		Env::characterPos.v[0] = -Env::cameraShift.v[0];
		Env::characterPos.v[1] = -Env::cameraShift.v[1];
		Env::characterPos.v[2] = -Env::cameraShift.v[2];

		for (auto& ao : GameObjects::activeObjects)
		{
			if (sqrtf(
				(Env::characterPos.v[0] - ao.objectPos.v[0]) * (Env::characterPos.v[0] - ao.objectPos.v[0])
				+
				(Env::characterPos.v[1] - ao.objectPos.v[1]) * (Env::characterPos.v[1] - ao.objectPos.v[1])
				+
				(Env::characterPos.v[2] - ao.objectPos.v[2]) * (Env::characterPos.v[2] - ao.objectPos.v[2])
			) < 50.f)
			{
				ao.highlighted = true;
			}
			else
			{
				ao.highlighted = false;
			}
		}

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
				UpdateScene(CONST_MAX_TIME_INTERVAL); //Limit game update speed to FPS
			}
			else
			{
				UpdateScene(NewTickCount - LastTickCount);
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
		GameObjects::coneTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./conus.bmp"));
		
		GameObjects::colorCubeMesh = CreateCube3D(5.0);
		GameObjects::colorCubeMeshMutable.data = CreateCube3D(5.0);
		GameObjects::colorCubeMeshMutable.RefreshVBO();

		GameObjects::testObjMesh = LoadFromObjFile("./chair_01.obj");
		GameObjects::testObjMesh.Scale(10);
		GameObjects::testObjMesh.SwapZandY();
		GameObjects::testObjMeshMutable.data = GameObjects::testObjMesh;
		GameObjects::testObjMeshMutable.RefreshVBO();

		GameObjects::textMesh = LoadFromTextFile("./mesh001.txt");

		GameObjects::coneMesh = LoadFromTextFile("./cone001.txt");

		GameObjects::coneMesh.Scale(200);

		GameObjects::textMeshMutable.AssignFrom(GameObjects::textMesh);
		GameObjects::textMeshMutable.RefreshVBO();
		GameObjects::coneMeshMutable.AssignFrom(GameObjects::coneMesh);
		GameObjects::coneMeshMutable.RefreshVBO();


		ActiveObject ao1;



		ao1.activeObjectMesh = LoadFromTextFile("./book001.txt");
		ao1.activeObjectMesh.Scale(4);

		ao1.activeObjectMeshMutable.AssignFrom(ao1.activeObjectMesh);
		ao1.activeObjectMeshMutable.RefreshVBO();

		ao1.objectPos = Vector3f{50, 0, -300};
		ao1.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./book03.bmp"));

		ao1.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));

		ao1.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
		ao1.activeObjectScreenMeshMutable.AssignFrom(ao1.activeObjectScreenMesh);
		ao1.activeObjectScreenMeshMutable.RefreshVBO();

		GameObjects::activeObjects.push_back(ao1);



		std::cout << "Hello test 4x" << std::endl;
		
		renderer.InitOpenGL();

		CheckGlError();


		std::cout << "Hello test 5x" << std::endl;

//               ТЕСТИРУЕМ ВЗАИМОДЕЙСТВИЕ С ИНВЕНТАРЕМ
        auto roomTexturePtr = std::make_shared<ZL::Texture>(ZL::CreateTextureDataFromBmp24("./Kitchen_ceramics.bmp"));
        auto coneTexturePtr = std::make_shared<ZL::Texture>(ZL::CreateTextureDataFromBmp24("./conus.bmp"));

    // Добавляем предметы в инвентарь
        ZL::AddItemToInventory("RoomCeramics", roomTexturePtr);
        ZL::AddItemToInventory("Cone", coneTexturePtr);

        ZL::PrintInventory();

		 // Initialize audio player and start background music
        GameObjects::audioPlayer = std::make_unique<AudioPlayer>();
        if (GameObjects::audioPlayer) {
            GameObjects::audioPlayer->playMusic("Symphony No.6 (1st movement).ogg");
        }

		// Инициализация меша иконки инвентаря
		GameObjects::inventoryIconMesh = CreateRect2D(
			{0.0f, 0.0f},
			{GameObjects::INVENTORY_ICON_SIZE/2, GameObjects::INVENTORY_ICON_SIZE/2},
			0.5f
		);
		GameObjects::inventoryIconMeshMutable.AssignFrom(GameObjects::inventoryIconMesh);
		GameObjects::inventoryIconMeshMutable.RefreshVBO();

		///
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
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                static int x = 0;

                GameObjects::bx.Interpolate(x);
                x = x + 2;
            }
            if (event.type == SDL_MOUSEWHEEL) {

                static const float zoomstep = 1.0f;
                if (event.wheel.y > 0) {
                    // ��������� ����� - ����������� zoom
                    Env::zoom -= zoomstep;
                }
                else if (event.wheel.y < 0) {
                    // ��������� ���� - ��������� zoom
                    Env::zoom += zoomstep;
                }
                // ������������ zoom, ����� �� �� ���� �������������
                if (Env::zoom < zoomstep) {
                    Env::zoom = zoomstep;
                }
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                case SDLK_a:
                    Env::leftPressed = true;
                    if (GameObjects::audioPlayer) {
                        GameObjects::audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                    }
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    Env::rightPressed = true;
                    if (GameObjects::audioPlayer) {
                        GameObjects::audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                    }
                    break;
                case SDLK_UP:
                case SDLK_w:
                    Env::upPressed = true;
                    if (GameObjects::audioPlayer) {
                        GameObjects::audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                    }
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    Env::downPressed = true;
                    if (GameObjects::audioPlayer) {
                        GameObjects::audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                    }
                    break;
                case SDLK_SPACE:
                    if (GameObjects::audioPlayer) {
                        GameObjects::audioPlayer->playMusic("Symphony No.6 (1st movement).ogg");
                    }
                    break;
                }
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                case SDLK_a:
                    Env::leftPressed = false;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    Env::rightPressed = false;
                    break;
                case SDLK_UP:
                case SDLK_w:
                    Env::upPressed = false;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    Env::downPressed = false;
                    break;
                }
            }
        }

        render();
    }

};

#include "Game.h"

int main(int argc, char* argv[]) {
    constexpr int CONST_WIDTH = 1280;
    constexpr int CONST_HEIGHT = 720;

    Environment::width = CONST_WIDTH;
    Environment::height = CONST_HEIGHT;

    Game game;
    game.setup();

#ifdef EMSCRIPTEN
    emscripten_set_main_loop([]() { game.update(); }, 0, 1);
#else
    while (!game.shouldExit()) {
        game.update();
        SDL_Delay(2);
    }
#endif

    return 0;
}
