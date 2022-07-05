#pragma once

#include "RigidBodyComponent.h"
namespace dae
{
	class SpriteComponent;
	class Enemy;
	class EnemyState
	{
	public:
		
		virtual ~EnemyState() = default;
		virtual void Update() {};
		void SetRigidBody(RigidBodyComponent* rigidBody) { m_pRigidBody = rigidBody; }
		void SetSprite(SpriteComponent* sprite) { m_pSprite = sprite; }
		void SetEnemy(Enemy* enemy) { m_pEnemy = enemy; }
		void SetWalking(bool walking) { m_WalkingPlatform = walking; }
		bool GetWalking() const { return m_WalkingPlatform; }

		void SetClimbing(bool climbing) { m_ClimbingLadder = climbing; }
		bool GetClimbing() { return m_ClimbingLadder;  }

	protected:

	 RigidBodyComponent* m_pRigidBody;
	 SpriteComponent* m_pSprite;
	 Enemy* m_pEnemy;

		bool m_ClimbingLadder = false;
		bool m_WalkingPlatform = false;
	};

	class MovingLeft final : public EnemyState
	{
	public:

		virtual void Update() override;
	};

	class MovingUp final : public EnemyState
	{
	public:
		virtual void Update() override;
	};
	class MovingRight final : public EnemyState
	{
	public:
		virtual void Update() override;
	};

	class MovingDown final : public EnemyState
	{
	public:
		virtual void Update() override;
	};

	class Dying final : public EnemyState
	{
	public: 
		virtual void Update() override;
	};

	class Falling final : public EnemyState
	{
		virtual void Update() override;
	};
	class Peppered final : public EnemyState
	{
		virtual void Update() override;
	};
}

