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
		m_BlockGrid[i][0] = true;
		m_BlockGrid[i][m_GridWidth -1] = true;
	}
	for (int i{}; i < m_GridWidth; ++i)
	{
		m_BlockGrid[0][i] = true;
		m_BlockGrid[m_GridHeight -1][i] = true;
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
		m_NrEnemies = 0;
		ResetGrid();
		LoadLevel("Next");
	}
}

void dae::GameManager::SetGridBlock(int row, int column)
{
	m_BlockGrid[column][row] = true;
}
bool dae::GameManager::GetGridBlock(Float2 pos) const
{
	int column = int((int(pos.x - m_WidthOffset) / m_CellSize));
	int row = int((int(pos.y + m_HeightOffset) / m_CellSize));
	//std::cout << column << " " << row << "\n";
	//std::cout << m_BlockGrid[row][column] << "\n";
	return m_BlockGrid[row][column];
}
void dae::GameManager::ResetGrid()
{
	for (int i{ 0 }; i < m_GridHeight; ++i)
	{
		for (int j{ 0 };j <  m_GridWidth; ++j)
			m_BlockGrid[i][j] = false;
	}
	for (int i{}; i < m_GridHeight; ++i)
	{
		m_BlockGrid[i][0] = true;
		m_BlockGrid[i][m_GridWidth - 1] = true;
	}
	for (int i{}; i < m_GridWidth; ++i)
	{
		m_BlockGrid[0][i] = true;
		m_BlockGrid[m_GridHeight - 1][i] = true;
	}
}

void dae::GameManager::LoadLevel(const std::string& levelName)
{
	SceneManager::GetInstance().GetActiveScene().MarkForDestroy();

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
	if(!isToRefresh)
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