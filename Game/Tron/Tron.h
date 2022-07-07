#pragma once
#include "Minigin.h"

namespace dae
{
	class GameManager;
	enum class GameMode
	{
		SINGLE,
		COOP,
		VERSUS
	};
	class Scene;
	enum class EnemyType;
	class Tron
	{
	public:
		void Run();
		void LoadLevel(GameMode gameMode, const std::string& levelName) const;

	private:
		void Initialize();
		void LoadGame();
		//void Cleanup();
		//std::vector<Float2> ParseLevel(Scene& scene, int sceneNr, const std::string& levelName)const;
		//void CreatePeterPepperAndHUD(Transform spawnPos, Scene& scene, int playerNr, bool andHUD) const;
		//void CreateEvilPepper(Transform spawnPos, Scene& scene, int playerNr) const;
		//void MakeEnemySpawner(std::vector<Float2> spawnPositions) const;
		//void CreateMenu(Scene& scene) const;
		//void CreateTray(Scene& scene, int sceneNr, Float2 position) const;
		//void CreateMenuButton(Scene& scene, Float2 position, GameMode gameMode, const std::string& text) const;
		//std::vector<Float2> MakeLaddersAndPlatforms(Scene& scene, const std::vector<Ladder>& ladders, const std::vector<Platform>& platforms, int sceneNr)const;
		//void MakeBurgers(Scene& scene, const std::vector<Burger>& burgers, int sceneNr) const;
		//void MakeBurgerSpawner(std::vector<Float2> spawnPositions) const;
		//void CreateHighScoreDisplay(Scene& scene) const;


		Minigin m_Minigin;
	};
}