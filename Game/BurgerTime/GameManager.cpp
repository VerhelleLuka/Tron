#include "GameManager.h"
#include "BurgerTime.h"
#include "BurgerTimeStructs.h"
#include <string>
#include "SceneManager.h"
#include "Subject.cpp"
#include "Scene.h"
#include "PeterPepper.h"
#include "Enemy.h"
#include "BurgerComponent.h"
#include "InputManager.h"
#include "PointsDisplayComponent.h"
#include <iostream>
#include <fstream>
dae::GameManager::GameManager()
	: m_pBurgerTime(nullptr),
	m_Points(0),
	m_NrBurgers(0),
	m_CompletedBurgers(0),
	m_CurrentLevelIndex(0),
	m_Lives(3)
{
}
void dae::GameManager::SetBurgerTimeGame(BurgerTime* burgerTime)
{
	m_pBurgerTime = burgerTime;
}

void dae::GameManager::BurgerCompleted()
{
	m_CompletedBurgers++;
	if (m_CompletedBurgers >= m_NrBurgers &&  SceneManager::GetInstance().GetActiveScene().GetName() != "MainMenu")
	{
		m_CompletedBurgers = 0;
		++m_CurrentLevelIndex;
		std::shared_ptr<EventArgs> emptyArgs = std::make_shared<EventArgs>();
		Notify(EventType::WIN, emptyArgs);
		ClearObservers();
	}

}
//
void dae::GameManager::LoadLevel(const std::string& levelName)
{
	m_CompletedBurgers = 0;
	SceneManager::GetInstance().GetActiveScene().MarkForDestroy();

	if (levelName == "MainMenu")
	{
		m_pBurgerTime->LoadLevel1(m_GameMode, "MainMenu");
	}
	else if (levelName == "Same")
	{
		m_pBurgerTime->LoadLevel1(m_GameMode, SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
	{
		m_pBurgerTime->LoadLevel1(m_GameMode, "Level1");
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "Level1")
	{
		m_pBurgerTime->LoadLevel1(m_GameMode, "Level2");
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "Level2")
	{
		m_pBurgerTime->LoadLevel1(m_GameMode, "Level3");
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "Level3")
	{
		AddPoints(0);
		ResetScene(true);
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
	m_CompletedBurgers = 0;
}