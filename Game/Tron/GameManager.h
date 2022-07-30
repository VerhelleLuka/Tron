#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
#include "Subject.h"
#include "Structs.h"
#include "Tron.h"
#include "TronStructs.h"
namespace dae
{

	struct GridBlock
	{
		bool hasBlock;
		GameObject* gameObject;
	};
	class GameManager : public Singleton<GameManager>, public Subject
	{
	public:
		GameManager();
		~GameManager() = default;

		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

		void SetTronGame(Tron* tron);
		void LoadLevel( const std::string& levelName) ;


		void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; }
		void ResetScene(bool fullReset);
		GameMode GetGameMode() const { return m_GameMode; }

		void ReduceLife(bool isToRefresh);
		int GetLives() const { return m_Lives; }

		void AddPoints(int points);
		int GetPoints() const { return m_Points; }

		void SetGridBlock(int row, int column);
		GridBlock& GetGridBlock(Float2 pos) ;
		void SetGameObjectOnGrid(Float2 pos, GameObject* go);
		void RemoveGameObjectOnGrid(Float2 pos);
		void RemoveGameObjectOnGrid(GridBlock* gridBlock);
		GameObject* GetGameObjectOnGrid(Float2 pos) const;
		void AddEnemy() { ++m_NrEnemies; }
		void EnemyKilled();
		void ResetGrid();
		float GetCellSize() const { return m_CellSize; }
		
	private:

		int m_Points;
		int m_Lives;
		int m_NrEnemies;
		Tron* m_pTron;
		GameMode m_GameMode;
		int m_CurrentLevelIndex;

		const int m_GridWidth = 26;
		const int m_GridHeight = 18;
		GridBlock m_BlockGrid[18][26] = { false };

		const int m_WidthOffset = 8;
		const int m_HeightOffset = 0;
		const float m_CellSize = 24.f;

	};
}