#include "GameManager.h"
#include "Tron.h"
#include <string>
#include "SceneManager.h"
#include "Subject.cpp"
#include "Scene.h"
#include "InputManager.h"
#include "PointsDisplayComponent.h"
#include <iostream>
#include <fstream>
dae::GameManager::GameManager()
	: m_pTron(nullptr),
	m_Points(0),
	m_CurrentLevelIndex(0),
	m_Lives(3),
	m_GameMode(GameMode::SINGLE)
{
	for (int i{}; i < m_GridHeight; ++i)
	{
		m_BlockGrid[i][0].hasBlock = true;
		m_BlockGrid[i][0].gameObject = nullptr;
		m_BlockGrid[i][m_GridWidth - 1].hasBlock = true;
		m_BlockGrid[i][m_GridWidth - 1].gameObject = nullptr;
	}
	for (int i{}; i < m_GridWidth; ++i)
	{
		m_BlockGrid[0][i].hasBlock = true;
		m_BlockGrid[0][i].gameObject = nullptr;
		m_BlockGrid[m_GridHeight - 1][i].hasBlock = true;
		m_BlockGrid[m_GridHeight - 1][i].gameObject = nullptr;
	}
}
void dae::GameManager::SetTronGame(Tron* burgerTime)
{
	m_pTron = burgerTime;
}
void dae::GameManager::EnemyKilled()
{
	m_NrEnemies--;
	if (m_NrEnemies <= 0)
	{
		std::shared_ptr<EventArgs> emptyArgs = std::make_shared<EventArgs>();
		Notify(EventType::LEVELCLEAR, emptyArgs);
		m_NrEnemies = 0;
	}
}

void dae::GameManager::SetGridBlock(int row, int column)
{
	m_BlockGrid[column][row].hasBlock = true;
}
dae::GridBlock& dae::GameManager::GetGridBlock(Float2 pos) 
{
	int column = int((int(pos.x - m_WidthOffset) / m_CellSize));
	int row = int((int(pos.y + m_HeightOffset) / m_CellSize));
	return m_BlockGrid[row][column];
}

void dae::GameManager::SetGameObjectOnGrid(Float2 pos, GameObject* go)
{
	int column = int((int(pos.x - m_WidthOffset) / m_CellSize));
	int row = int((int(pos.y + m_HeightOffset) / m_CellSize));
	m_BlockGrid[row][column].gameObject = go;
}
void dae::GameManager::RemoveGameObjectOnGrid(Float2 pos)
{
	int column = int((int(pos.x - m_WidthOffset) / m_CellSize));
	int row = int((int(pos.y + m_HeightOffset) / m_CellSize));
	m_BlockGrid[row][column].gameObject = nullptr;
}
void dae::GameManager::RemoveGameObjectOnGrid(GridBlock* gridBlock)
{
	for (int i{ 0 }; i < m_GridHeight; ++i)
	{
		for (int j{ 0 }; j < m_GridWidth; ++j)
		{
			if (&m_BlockGrid[i][j] == gridBlock)
				m_BlockGrid[i][j].gameObject = nullptr;
		}
	}

}
dae::GameObject* dae::GameManager::GetGameObjectOnGrid(Float2 pos) const
{
	int column = int((int(pos.x - m_WidthOffset) / m_CellSize));
	int row = int((int(pos.y + m_HeightOffset) / m_CellSize));
	return m_BlockGrid[row][column].gameObject;
}

void dae::GameManager::ResetGrid()
{
	for (int i{ 0 }; i < m_GridHeight; ++i)
	{
		for (int j{ 0 }; j < m_GridWidth; ++j)
		{
			m_BlockGrid[i][j].hasBlock = false;
			m_BlockGrid[i][j].gameObject = nullptr;

		}
	}
	for (int i{}; i < m_GridHeight; ++i)
	{
		m_BlockGrid[i][0].hasBlock = true;
		m_BlockGrid[i][0].gameObject = nullptr;
		m_BlockGrid[i][m_GridWidth - 1].gameObject = nullptr;
		m_BlockGrid[i][m_GridWidth - 1].hasBlock = true;
	}
	for (int i{}; i < m_GridWidth; ++i)
	{
		m_BlockGrid[0][i].hasBlock = true;
		m_BlockGrid[0][i].gameObject = nullptr;
		m_BlockGrid[m_GridHeight - 1][i].hasBlock = true;
		m_BlockGrid[m_GridHeight - 1][i].gameObject = nullptr;
	}
}

void dae::GameManager::LoadLevel(const std::string& levelName)
{

	SceneManager::GetInstance().GetActiveScene().MarkForDestroy();
	m_NrEnemies = 0;
	ResetGrid();
	//Physics::GetInstance().DeleteScene(SceneManager::GetInstance().GetActiveSceneNr());
	if (levelName == "MainMenu")
	{
		m_pTron->LoadLevel(m_GameMode, "MainMenu");
	}
	else if (levelName == "Same")
	{
		m_pTron->LoadLevel(m_GameMode, SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
	{
		m_pTron->LoadLevel(m_GameMode, "Level1");
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "Level1")
	{
		m_pTron->LoadLevel(m_GameMode, "Level2");
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "Level2")
	{
		m_pTron->LoadLevel(m_GameMode, "Level3");
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "Level3")
	{
		m_pTron->LoadLevel(m_GameMode, "Level1");

	}
	ReduceLife(true);
	AddPoints(0);
}

void dae::GameManager::AddPoints(int points)
{
	m_Points += points;
	std::shared_ptr<PointsEventArgs> args = std::make_shared<PointsEventArgs>();
	args->points = m_Points;
	Notify(EventType::GAINEDPOINTS, args);
}

void dae::GameManager::ReduceLife(bool isToRefresh)
{
	if (!isToRefresh)
		--m_Lives;
	std::shared_ptr<EventArgs> emptyArgs = std::make_shared<EventArgs>();
	Notify(EventType::LOSTLIFE, emptyArgs);
	if (m_Lives <= 0)
	{
		ResetScene(true);
	}
}
void dae::GameManager::ResetScene(bool fullReset)
{
	auto& scene = SceneManager::GetInstance().GetActiveScene();
	scene.MarkForDestroy();
	if (!fullReset)
	{

		LoadLevel("Same");
		ReduceLife(true);
		AddPoints(0);
	}
	else
	{
		std::string fileName("../Data/Menu/HighScores.txt");
		std::ofstream  hiscoreFile;
		hiscoreFile.open(fileName, std::ios_base::app);
		hiscoreFile << m_Points << "\n";
		hiscoreFile.close();
		m_Lives = 3;
		m_Points = 0;
		LoadLevel("MainMenu");
	}
}