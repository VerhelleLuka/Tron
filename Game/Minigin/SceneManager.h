#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render();
		void Initialize();
		void SetActiveScene(Scene* scene);
		const std::string& GetActiveSceneName() const;
		Scene& GetActiveScene();
		int GetActiveSceneNr() const;
		std::vector<std::shared_ptr<Scene>> GetScenes() { return m_Scenes; }

	private:
		friend class Singleton<SceneManager>;
		SceneManager() : m_pActiveScene(nullptr) {};
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		Scene* m_pActiveScene = nullptr;
		bool m_SceneChanged = false;

		void DestroyScene();


	};
}
