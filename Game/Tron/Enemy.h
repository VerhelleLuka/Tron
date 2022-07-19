#pragma once
#include "RigidBodyComponent.h"
#include "BaseComponent.h"
#include "Scene.h"
namespace dae
{
	enum class EnemyType
	{
		TANK,
		RECOGNIZER
	};
	class Enemy final : public BaseComponent, public Subject
	{
	public:
		virtual void Update(float /*deltaTime*/) override {};
		virtual void FixedUpdate(float /*deltaTime*/) override ;
		virtual void Render() const {};
		Enemy(EnemyType enemyType);
		virtual ~Enemy() {};
		Enemy(const Enemy& other) = delete;
		Enemy(Enemy&& other) = delete;
		Enemy& operator=(const Enemy& other) = delete;
		Enemy& operator=(Enemy&& other) = delete;


		void SetOverlapEvent()
		{
			auto bindIng = std::bind(&Enemy::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(bindIng);
		}


	private:
		void OnOverlap(RigidBodyComponent* other);

		int m_NrHits;
		int m_MoveSpeed;
		bool m_Dead;

	};
}