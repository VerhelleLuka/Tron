#include "EnemyState.h"
#include "RigidBodyComponent.h"
#include "Enemy.h"
#include "SpriteComponent.h"
#include "Animation.h"
#include "Physics.h"
void dae::MovingDown::Update()
{
	if (m_pEnemy->GetCanDescend())
	{
		m_pRigidBody->SetDirection(Float2{ 0.f, 50.f });
		m_pSprite->SetActiveAnimation("Descend");

	}
}
void dae::MovingUp::Update()
{
	if (m_pEnemy->GetCanClimb())
	{
		m_pRigidBody->SetDirection(Float2{ 0.f, -50.f });
		m_pSprite->SetActiveAnimation("Climb");
		
	}
}
void dae::MovingLeft::Update()
{
	if (m_pEnemy->GetCanWalkLeft())
	{
		m_pRigidBody->SetDirection(Float2{ -50.f, 0.f });
		m_pSprite->SetActiveAnimation("WalkLeft");

	}
}

void dae::MovingRight::Update()
{
	if (m_pEnemy->GetCanWalkRight())
	{
		m_pRigidBody->SetDirection(Float2{ 50.f, 0.f });
		m_pSprite->SetActiveAnimation("WalkRight");
	}
}

void dae::Dying::Update()
{
	if (m_pEnemy->IsDead())
	{
		m_pSprite->SetActiveAnimation("Death");
	}
	if (m_pSprite->GetAnimation().GetFrameNr() == m_pSprite->GetAnimation().GetNrFrames() -1 )
	{
		/*Physics::GetInstance().*/
		m_pRigidBody->GetParent()->MarkForDelete();
	}
}

void dae::Falling::Update()
{
	m_pRigidBody->SetDirection(Float2{ 0.f, 100.f });

}
void dae::Peppered::Update()
{
	m_pSprite->SetActiveAnimation("Peppered");

}