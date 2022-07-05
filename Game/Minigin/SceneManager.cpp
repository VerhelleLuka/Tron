#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Physics.h"
#include "InputManager.h"
void dae::SceneManager::Initialize()
{
	for (auto& scene : m_Scenes)
	{
		scene->Initialize();
	}
}

void dae::SceneManager::Update(float deltaTime)
{
	if (m_pActiveScene)
		m_pActiveScene->Update(deltaTime);

}

void dae::SceneManager::FixedUpdate(float deltaTime)
{
	if (m_pActiveScene)
		m_pActiveScene->FixedUpdate(deltaTime);
}

void dae::SceneManager::Render()
{
	if (m_pActiveScene)
		m_pActiveScene->Render();

	for (size_t i = 0; i < m_Scenes.size(); i++)
	{
		if (m_Scenes[i]->GetMarkedForDestroy())
		{
			auto it = m_Scenes.begin();
			std::advance(it, i);
			m_Scenes.erase(it);
		}
	}

}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	scene->SetIndex(int(m_Scenes.size() - 1));
	return *scene;
}

void dae::SceneManager::SetActiveScene(Scene* scene)
{
	m_SceneChanged = true;
	m_pActiveScene = scene;
}

const std::string& dae::SceneManager::GetActiveSceneName() const
{
	return m_pActiveScene->GetName();
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	return *m_pActiveScene;
}

int dae::SceneManager::GetActiveSceneNr() const
{
	for (size_t i{}; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i].get() == m_pActiveScene)
		{
			return (int)i;
		}
	}
	return 0;
}
void dae::SceneManager::DestroyScene()
{
	Physics::GetInstance().DeleteScene(m_pActiveScene->GetIndex());

	for (size_t i = 0; i < m_Scenes.size(); i++)
	{
		if (m_Scenes[i].get()->GetMarkedForDestroy())
			m_Scenes[i]->GetSceneObjects().clear();
	}
}