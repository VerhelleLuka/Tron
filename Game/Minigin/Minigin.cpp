#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FpsComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include <chrono>
#include <ctime>
#include <ratio>
//#include <steam_api.h>
//#include "LivesDisplayComponent.h"
//#include "PointsDisplayComponent.h"
#include "SpriteComponent.h"
#include "Animation.h"
#include "RigidBodyComponent.h"
//#include "LevelParser.h"
#include "Sound.h"
#include "SDL_mixer.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	//AUDIO INITIALIZATION

	 //Set up the audio stream
	int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
	if (result < 0)
	{
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	result = Mix_AllocateChannels(4);
	if (result < 0)
	{
		fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
		exit(-1);
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

	m_IsInitialized = true;

}

void dae::Minigin::Run()
{
	if(!m_IsInitialized)
		Initialize();

	// tell the resource manager where he can find the game data

	//LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& physics = Physics::GetInstance();
		bool doContinue = true;
	
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;
		float fixedTimeStep = 0.02f;

		ServiceLocator::RegisterSoundSystem(new sound_system());
		//By now all gameobjects and scenes should have been made; intitialize everything
		sceneManager.Initialize();
		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;
			doContinue = input.ProcessInput();
			input.Update();
			sceneManager.Update(deltaTime);

			while (lag >= fixedTimeStep)
			{
				sceneManager.FixedUpdate(fixedTimeStep);
				physics.FixedUpdate(fixedTimeStep);
				lag -= fixedTimeStep;

			}
			renderer.Render();
		}
	}

	Cleanup();
}

void dae::Minigin::Cleanup()
{
	ServiceLocator::DestroySoundSystem();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}