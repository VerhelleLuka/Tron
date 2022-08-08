#pragma once
#include "RigidBodyComponent.h"
#include "BaseComponent.h"
#include "Scene.h"
#include "TronStructs.h"
namespace dae
{
	//Enemy states

	class EnemyState
	{
	public:

		EnemyState();
		virtual ~EnemyState();
		EnemyState(const EnemyState& other) = delete;
		EnemyState(EnemyState&& other) = delete;
		EnemyState& operator=(const EnemyState& other) = delete;
		EnemyState& operator=(EnemyState&& other) = delete;

		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual void Update() = 0;
	};

	class EnemyDead : public EnemyState
	{
		EnemyDead();
		virtual ~EnemyDead() {};
		EnemyDead(const EnemyDead& other) = delete;
		EnemyDead(EnemyDead&& other) = delete;
		EnemyDead& operator=(const EnemyDead& other) = delete;
		EnemyDead& operator=(EnemyDead&& other) = delete;

		virtual void Enter() override ;
		virtual void Exit() override;
		virtual void Update() override;
	};
	class EnemyWander : public EnemyState
	{
		EnemyWander();
		virtual ~EnemyWander() {};
		EnemyWander(const EnemyWander& other) = delete;
		EnemyWander(EnemyWander&& other) = delete;
		EnemyWander& operator=(const EnemyWander& other) = delete;
		EnemyWander& operator=(EnemyWander&& other) = delete;

		virtual void Enter() override;
		virtual void Exit() override;
		virtual void Update() override;
	};
	class EnemyTarget : public EnemyState
	{
		EnemyTarget();
		virtual ~EnemyTarget() {};
		EnemyTarget(const EnemyTarget& other) = delete;
		EnemyTarget(EnemyTarget&& other) = delete;
		EnemyTarget& operator=(const EnemyTarget& other) = delete;
		EnemyTarget& operator=(EnemyTarget&& other) = delete;

		virtual void Enter() override;
		virtual void Exit() override;
		virtual void Update() override;
	};
	enum class EnemyType
	{
		TANK,
		RECOGNIZER
	};
	class Enemy final : public BaseComponent, public Subject
	{
	public:
		virtual void Update(float /*deltaTime*/) override;
		virtual void FixedUpdate(float /*deltaTime*/) override;
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
		void ChangeMoveDir();
		EnemyState m_EnemyState;
		MovementDirection m_CurMovDir;
		MovementDirection m_PrevMovDir;
		float m_ChangeDirectionTimer;
		float m_ChangeDirectionTime;
		bool m_PlayerInRange;

		const float m_ShootDelayTime{ 1.5f };
		float m_ShootDelayTimer;
		Float2 m_LookDir;
		float m_CellSize;

		EnemyType m_EnemyType;
		EnemyState* m_pState;

	};
}