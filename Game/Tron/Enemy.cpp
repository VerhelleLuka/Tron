#pragma once
#include "Enemy.h"
#include "GameManager.h"
#include "Bullet.h"
#include <SpriteComponent.h>
#include "Animation.h"
dae::Enemy::Enemy(EnemyType enemyType)
	:m_NrHits(3),
	m_Dead(false),
	m_CurMovDir(MovementDirection::DOWN),
	m_PrevMovDir(MovementDirection::DOWN),
	m_PlayerInRange(false),
	m_ChangeDirectionTime(2.5f),
	m_ChangeDirectionTimer(0.f),
	m_EnemyState(EnemyState::Wandering)
{
	m_MoveSpeed = 66.f;
	if (enemyType == EnemyType::RECOGNIZER)
		m_MoveSpeed = 200.f;
}

void dae::Enemy::FixedUpdate(float /* elapsedSec*/)
{
	if (m_Dead)
	{
		if (m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetFrameNr() == m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetNrFrames() - 1)
		{
			GameManager::GetInstance().EnemyKilled();
			m_pParent->MarkForDelete();
		}
	}
}
void dae::Enemy::Update(float elapsedSec)
{

	if (m_EnemyState == EnemyState::Dead)
		return;
		Move();

	if (m_EnemyState == EnemyState::Wandering)
	{
		m_ChangeDirectionTimer += elapsedSec;
		if (m_ChangeDirectionTimer >= m_ChangeDirectionTime)
		{
			//Change direction between 1.5s and 3s
			m_ChangeDirectionTime = static_cast<float>(500 + rand() % (1000 - 500 + 1));
			m_ChangeDirectionTime /= 1000;
			m_ChangeDirectionTime *= 3;
			m_ChangeDirectionTimer = 0.f;
			ChangeDirection();
		}
	}
}
void dae::Enemy::ChangeDirection()
{
	if (m_EnemyState == EnemyState::Dead)
		return;

	std::shared_ptr<SpriteEventArgs> args = std::make_shared<SpriteEventArgs>();

	m_PrevMovDir = m_CurMovDir;

	while (m_CurMovDir == m_PrevMovDir)
	{
		int newMovDir = rand() % 4;

		m_CurMovDir = static_cast<MovementDirection>(newMovDir);
	}

	switch (m_CurMovDir)
	{

	case MovementDirection::RIGHT:
		args->name = "WalkRight";
		break;
	case MovementDirection::LEFT:
		args->name = "WalkLeft";
		break;
	case MovementDirection::UP:
		args->name = "Climb";
		break;
	case MovementDirection::DOWN:
		args->name = "Descend";
		break;
	default:
		args->name = "Idle";
		break;
	}
	Notify(EventType::STATECHANGED, args);


}
void dae::Enemy::Move()
{
	if (m_EnemyState == EnemyState::Dead)
		return;

	std::shared_ptr<SpriteEventArgs> args = std::make_shared<SpriteEventArgs>();
	switch (m_CurMovDir)
	{

	case MovementDirection::RIGHT:
		args->name = "WalkRight";
		break;
	case MovementDirection::LEFT:
		args->name = "WalkLeft";
		break;
	case MovementDirection::UP:
		args->name = "Climb";
		break;
	case MovementDirection::DOWN:
		args->name = "Descend";
		break;
	default:
		args->name = "Idle";
		break;
	}
	Notify(EventType::STATECHANGED, args);
	float offset = 24.f;
	Float2 centerPoint = { m_pParent->GetTransform().GetPosition().x,m_pParent->GetTransform().GetPosition().y };
	float halfWidth = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth() / 2.f;
	float halfHeight = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() / 2.f;
	centerPoint.x += halfWidth;
	centerPoint.y -= halfHeight;
	Float2 direction = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetDirection();
	auto& gameManager = GameManager::GetInstance();


	if (gameManager.GetGridBlock(Float2{ (centerPoint.x - halfWidth), centerPoint.y + offset }) && m_CurMovDir == MovementDirection::LEFT)
	{
		ChangeDirection();
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ (centerPoint.x + halfWidth), centerPoint.y + offset }) && m_CurMovDir == MovementDirection::RIGHT)
	{
		ChangeDirection();
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y - halfHeight + offset) }) && m_CurMovDir == MovementDirection::UP)
	{
		ChangeDirection();
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y + halfHeight + offset) }) && m_CurMovDir == MovementDirection::DOWN)
	{
		ChangeDirection();
		return;
	}

	int x{ 0 }, y{ 0 };

	switch (m_CurMovDir)
	{
	case MovementDirection::DOWN:
		y = 1;
		break;
	case MovementDirection::LEFT:
		x = -1;
		break;
	case MovementDirection::RIGHT:
		x = 1;
		break;
	case MovementDirection::UP:
		y = -1;
		break;
	default:
		break;
	}

	m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2{ m_MoveSpeed * x, m_MoveSpeed * y });
}

bool dae::Enemy::PlayerInRange() const
{
	return true;
}
void dae::Enemy::Shoot() const
{

}
void dae::Enemy::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Bullet")
	{
		m_NrHits--;
		if (m_NrHits < -1)
		{
			m_Dead = true;
			std::shared_ptr<SpriteEventArgs> args = std::make_shared<SpriteEventArgs>();
			args->name = "Death";
			Notify(EventType::STATECHANGED, args);
		}
	}
}