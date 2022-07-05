#pragma once
#include "RigidBodyComponent.h"
#include "Singleton.h"  
namespace dae
{
	class Physics : public Singleton<Physics>
	{
	public:
		Physics() {};
		~Physics() = default;

		Physics(const Physics& other) = delete;
		Physics(Physics&& other) = delete;
		Physics& operator=(const Physics& other) = delete;
		Physics& operator=(Physics&& other) = delete;

		void AddRigidBodyComponent(RigidBodyComponent* rigidBody);
		void RemoveRigidBodyComponent(RigidBodyComponent* rigidBody);
		bool FindRigidBodyComponent(RigidBodyComponent* rigidBody);
		void FixedUpdate(float deltaTime);
		void CheckOverlap();

		void SetSceneNr(int sceneNr);
		void DeleteScene(int index);

	private:
		std::vector<std::vector<RigidBodyComponent*>> m_pRigidBodies;
		int m_SceneNr{0};
	};
}
