#pragma once
struct SDL_Window;
#include "Transform.h"
#include "Physics.h"
namespace dae
{
	class Scene;
	class Minigin
	{
	public:
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
		SDL_Window* GetWindow() const { return m_Window; }
	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};

		//void CreatePeterPepperAndHUD(Transform spawnPos, Scene& scene,int playerNr ) const;
		//Transform ParseLevel(Scene& scene)const;
		//void MakeLaddersAndPlatforms(Scene& scene, const std::vector<Ladder>& ladders, const std::vector<Platform>& platforms)const;
		//void MakeBurgers(Scene& scene, const std::vector<Burger>& burgers) const;
		bool m_SteamApi;
		bool m_IsInitialized = false;

	};
}