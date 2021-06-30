#include "TextureManager.h"
#include "ShaderManager.h"
#include "Renderer.h"

#include "Physics.h"
#include <string>

#include <windows.h>

#include <cstdlib> 
#include <ctime> 
#include <iostream>

#include "Game.h"

namespace ZL
{

	HGLRC hRC;		//Render context
	HWND Hwnd;		//Main window handle
	HDC hDC;		//Device context

	Renderer renderer;

	GameState gs;


	//There might be anything
	const wchar_t CONST_WINDOW_CLASS_NAME[] = L"ZeptoLabClass";

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
		CheckGlError();

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

		SwapBuffers(hDC);
	}

	

	void ProcessTickCount()
	{

		if (LastTickCount == 0)
		{
			LastTickCount = GetTickCount64();
			return;
		}

		NewTickCount = GetTickCount64();
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


	LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static PAINTSTRUCT ps;

		switch (uMsg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			ExitGameLoop = true;
			break;
		case WM_PAINT:

			DrawScene();
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);

			ProcessTickCount();
			break;
		case WM_SIZE:

			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:

			if (gs.isGameOver)
			{
				gs.RestartGame();
			}
			else
			{
				gs.BirdJump();
			}
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}


	bool CreateOpenGLWindow(const wchar_t* title, int x, int y, int width, int height, HWND& hWnd, HDC& hDC, HGLRC& hRC)
	{
		int pf;
		WNDCLASS wc;
		PIXELFORMATDESCRIPTOR pfd;
		static HINSTANCE hInstance = 0;

		if (!hInstance)
		{
			hInstance = GetModuleHandle(NULL);
			wc.style = CS_OWNDC;
			wc.lpfnWndProc = (WNDPROC)WindowProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = NULL;
			wc.lpszMenuName = NULL;
			wc.lpszClassName = CONST_WINDOW_CLASS_NAME;

			if (!RegisterClass(&wc))
			{
				return NULL;
			}
		}

		RECT r;
		r.left = x;
		r.right = x + width;
		r.top = y;
		r.bottom = y + height;

		DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		AdjustWindowRect(&r, windowStyle, false);

		Env::windowHeaderHeight = abs(r.top);

		hWnd = CreateWindow(CONST_WINDOW_CLASS_NAME, title, windowStyle, 0, 0, r.right - r.left, r.bottom - r.top, NULL, NULL, hInstance, NULL);

		if (hWnd == NULL)
		{
			return false;
		}

		hDC = GetDC(hWnd);

		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;

		pf = ChoosePixelFormat(hDC, &pfd);
		if (pf == 0)
		{
			return false;
		}

		if (SetPixelFormat(hDC, pf, &pfd) == FALSE)
		{
			return false;
		}

		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		return true;
	}


	void DestroyEngine()
	{
		
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(Hwnd, hDC);
		wglDeleteContext(hRC);
		DestroyWindow(Hwnd);

	}

	void MainLoop()
	{

		MSG msg;

		ShowWindow(Hwnd, SW_SHOW);
		UpdateWindow(Hwnd);

		NewTickCount = GetTickCount64();
		LastTickCount = NewTickCount;

		while (!ExitGameLoop)
		{
			while (PeekMessage(&msg, Hwnd, 0, 0, PM_NOREMOVE))
			{
				if (GetMessage(&msg, Hwnd, 0, 0))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					ExitGameLoop = true;
				}
			}
			DrawScene();

			ProcessTickCount();
		}
	}

};

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	using namespace ZL;

	try {

		srand((unsigned)time(NULL));

		constexpr int CONST_WIDTH = 1280;
		constexpr int CONST_HEIGHT = 720;

		Env::width = CONST_WIDTH;
		Env::height = CONST_HEIGHT;

		if (!CreateOpenGLWindow(L"ZeptoLabTest", 0, 0, Env::width, Env::height, Hwnd, hDC, hRC))
		{
			throw std::exception("Failed to create OpenGL Window!");
		}

		BindOpenGlFunctions();

		//Load shaders:
		renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");

		//Load textures
		GameObjects::birdTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./bird.bmp32"));
		GameObjects::backgroundTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./background.bmp"));
		GameObjects::pipeTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./pipe.bmp32"));
		GameObjects::gameOverTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./game_over.bmp32"));



		//Create bird mesh
		GameObjects::birdMesh = CreateRect2D({ 0.f, 0.f }, { GameConsts::birdScale * GameObjects::birdTexturePtr->getWidth(), GameConsts::birdScale * GameObjects::birdTexturePtr->getHeight() }, 0);

		float backgroundTextureScale = Env::height / static_cast<float>(GameObjects::backgroundTexturePtr->getHeight());


		//Create pipe mesh
		GameObjects::pipeMesh = CreateRect2D({ GameObjects::pipeTexturePtr->getWidth() * GameConsts::pipeScale * (-0.5f), Env::getActualClientHeight() * 0.5f + GameObjects::pipeTexturePtr->getHeight() * GameConsts::pipeScale * (-0.5f) }, { GameObjects::pipeTexturePtr->getWidth() * GameConsts::pipeScale * 0.5f, GameObjects::pipeTexturePtr->getHeight() * GameConsts::pipeScale * 0.5f }, 0);


		//Create background mesh depending on screen size

		Env::backgroundSectionWidth = GameObjects::backgroundTexturePtr->getWidth() * backgroundTextureScale;

		GameObjects::backgroundMesh = CreateRectHorizontalSections2D({ GameObjects::backgroundTexturePtr->getWidth() * backgroundTextureScale * (0.5f), GameObjects::backgroundTexturePtr->getHeight() * backgroundTextureScale * (0.5f) }, { GameObjects::backgroundTexturePtr->getWidth() * backgroundTextureScale * 0.5f, GameObjects::backgroundTexturePtr->getHeight() * backgroundTextureScale * 0.5f }, 0, 2);



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

		MainLoop();

		DestroyEngine();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Oops: " << e.what() << std::endl;
	}
	
	return 0;

}