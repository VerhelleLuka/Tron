#pragma once
#include "SceneManager.h"
#include "RigidBodyComponent.h"
namespace dae
{
	class SceneObject;
	class Physics;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<SceneObject>& object);
		void SetIndex(int index) { m_Index = index; }
		int GetIndex() const { return m_Index; }
		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Initialize();

		std::vector<std::shared_ptr<SceneObject>>& GetSceneObjects() { return m_Objects; }
		//void DeleteSceneObject(SceneObject& object) { }
		void Render() ;
		const std::string& GetName() const { return m_Name; }
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		void MarkForDestroy() { m_MarkedForDestroy = true; }
		bool GetMarkedForDestroy() const { return m_MarkedForDestroy; }
		void SetEmpty(bool empty) { m_Empty = empty; }
		bool GetEmpty() const { return m_Empty; }
	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::shared_ptr<SceneObject>> m_Objects{};
		int m_Index;

		static unsigned int m_IdCounter; 
		
		bool m_MarkedForDestroy;
		bool m_Empty;
	};

}
