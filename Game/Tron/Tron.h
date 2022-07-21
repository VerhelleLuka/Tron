#pragma once
#include "Minigin.h"
#include "TronStructs.h"
namespace dae
{
	class GameManager;

	class Scene;
	struct Block;
	class Tron
	{
	public:
		void Run();
		void LoadLevel(GameMode gameMode, const std::string& levelName) const;
		
	private:
		void Initialize();
		void LoadGame();
		//void Cleanup();
		std::vector<Float2> ParseLevel(Scene& scene, int sceneNr, const std::string& levelName)const;
		void CreateTronAndHUD(Scene& scene, int playerNr, bool andHUD) const;
		void CreateTeleporter(Scene& scene) const;
		void CreateEvilTron( Scene& scene, int playerNr) const;
		void MakeEnemy(Scene& scene) const;
		void CreateMenu(Scene& scene) const;
		void CreateBlocks(Scene& scene, int sceneNr, std::vector<Block>& blocks) const;
		void CreateFixedBlocks(Scene& scene, int sceneNr) const;
		void CreateMenuButton(Scene& scene, Float2 position, GameMode gameMode, const std::string& text) const;
		//std::vector<Float2> MakeLaddersAndPlatforms(Scene& scene, const std::vector<Ladder>& ladders, const std::vector<Platform>& platforms, int sceneNr)const;
		//void MakeBurgers(Scene& scene, const std::vector<Burger>& burgers, int sceneNr) const;
		//void MakeBurgerSpawner(std::vector<Float2> spawnPositions) const;
		void CreateHighScoreDisplay(Scene& scene) const;


		Minigin m_Minigin;
	};
}