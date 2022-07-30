#pragma once
#include "RigidBodyComponent.h"
#include "BaseComponent.h"
#include "Scene.h"
#include "TronStructs.h"
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
		virtual void Update(float /*deltaTime*/) override ;
		virtual void FixedUpdate(float /*deltaTime*/) override ;
		virtual void Render() const {};
		Enemy(EnemyType enemyType);
		virtual ~Enemy();
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
		void Move();
		bool PlayerInRange() const;
		void Shoot() const;
		void ChangeDirection();
		enum class EnemyState
		{
			Dead,
			Wandering,
			Targeting
		};
		void OnOverlap(RigidBodyComponent* other);

		int m_NrHits;
		float m_MoveSpeed;

		EnemyState m_EnemyState;
		MovementDirection m_CurMovDir;
		MovementDirection m_PrevMovDir;
		float m_ChangeDirectionTimer;
		float m_ChangeDirectionTime;
		bool m_PlayerInRange;

		const float m_ShootDelayTime{ 1.5f };
		float m_ShootDelayTimer;

		float m_CellSize;

		EnemyType m_EnemyType;

	};
}