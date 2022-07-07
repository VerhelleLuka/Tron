#include "Tron.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
void dae::Tron::Initialize()
{
	GameManager::GetInstance().SetTronGame(this);
}

void dae::Tron::LoadGame()
{
	auto& menuScene = SceneManager::GetInstance().CreateScene("MainMenu");
	SceneManager::GetInstance().SetActiveScene(&menuScene);
	Physics::GetInstance().SetSceneNr(0);
	//CreateMenu(menuScene);
}

void dae::Tron::Run()
{

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	m_Minigin.Initialize();
	Initialize();
	LoadGame();

	{
		m_Minigin.Run();
	}
	//Cleanup();
}

void dae::Tron::LoadLevel(GameMode /*gameMode*/, const std::string& /*levelName*/) const
{

}