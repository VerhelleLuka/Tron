#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
#include "Subject.h"
#include "Structs.h"
#include "Tron.h"
namespace dae
{


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

		void SetNrOfBurgers(int nrBurgers) { m_NrBurgers = nrBurgers; }
		void BurgerCompleted();

		void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; }
		void ResetScene(bool fullReset);
		GameMode GetGameMode() const { return m_GameMode; }

		void ReduceLife(bool isToRefresh);
		int GetLives() const { return m_Lives; }

		void AddPoints(int points);
		int GetPoints() const { return m_Points; }

	private:
		int m_Points;
		int m_Lives;
		Tron* m_pTron;
		GameMode m_GameMode;
		int m_NrBurgers;
		int m_CompletedBurgers;
		int m_CurrentLevelIndex;
		
	};
}