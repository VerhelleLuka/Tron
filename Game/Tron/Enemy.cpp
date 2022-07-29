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
	m_EnemyState(EnemyState::Wandering),
	m_ShootDelayTimer(m_ShootDelayTime),
	m_EnemyType(enemyType)
{
	m_MoveSpeed = 66.f;
	if (enemyType == EnemyType::RECOGNIZER)
		m_MoveSpeed = 200.f;

	m_CellSize = GameManager::GetInstance().GetCellSize();
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
	m_ShootDelayTimer += elapsedSec;
	if (PlayerInRange())
	{
		m_EnemyState = EnemyState::Targeting;
	}
	else
	{
		m_EnemyState = EnemyState::Wandering;
		m_ShootDelayTimer = m_ShootDelayTime;
	}
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
	else if (m_EnemyState == EnemyState::Targeting && m_EnemyType == EnemyType::TANK)
	{
		if (m_ShootDelayTimer >= m_ShootDelayTime)
		{
			m_ShootDelayTimer = 0.f;
			Shoot();
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


	if (gameManager.GetGridBlock(Float2{ (centerPoint.x - halfWidth), centerPoint.y + offset }).hasBlock && m_CurMovDir == MovementDirection::LEFT)
	{
		ChangeDirection();
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ (centerPoint.x + halfWidth), centerPoint.y + offset }).hasBlock && m_CurMovDir == MovementDirection::RIGHT)
	{
		ChangeDirection();
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y - halfHeight + offset) }).hasBlock && m_CurMovDir == MovementDirection::UP)
	{
		ChangeDirection();
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y + halfHeight + offset) }).hasBlock && m_CurMovDir == MovementDirection::DOWN)
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
	Float2 pos = { m_pParent->GetTransform().GetPosition().x,m_pParent->GetTransform().GetPosition().y };
	float halfWidth = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth() / 2.f;
	float halfHeight = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() / 2.f;
	for (size_t i{}; i < 25; ++i)
	{
		GridBlock gridBlock = GameManager::GetInstance().GetGridBlock(Float2{ pos.x + halfWidth + (m_CellSize * i * x), pos.y + halfHeight + (m_CellSize * i * y) });

		if (gridBlock.gameObject != nullptr)
		{
			if (gridBlock.gameObject->GetTag() == "Player")
				return true;
		}
		if (gridBlock.hasBlock)
			return false;

	}
	return false;
}
void dae::Enemy::Shoot() const
{
	auto bullet = std::make_shared<GameObject>();

	auto bulletSprite = std::make_shared<SpriteComponent>();
	auto bulletAnimation = std::make_shared<Animation>(1, 1);

	Transform transform = m_pParent->GetTransform();
	transform.SetPosition(transform.GetPosition().x + 5, transform.GetPosition().y + 5, transform.GetPosition().z);

	bullet->SetTransform(transform);

	bullet->SetTag("Bullet");

	bullet->AddComponent(bulletSprite, "BulletSprite");

	bulletAnimation->SetTexture("Hamburger/Cheese.png");

	bulletSprite->AddAnimation(bulletAnimation, "Bullet");
	bulletSprite->SetActiveAnimation("Bullet");
	bulletAnimation->SetScale(2.f);

	//RigidBodyComponent
	auto pRigidBody = std::make_shared<RigidBodyComponent>(bulletSprite->GetAnimation().GetScaledWidth(),
		bulletSprite->GetAnimation().GetScaledHeight(),
		true);
	pRigidBody->SetGameObject(bullet.get());
	pRigidBody->SetVelocityPreservation(true);
	bullet->AddComponent(pRigidBody, "RigidBody");

	float bulletSpeed = 50.f;
	Float2 aimDir{0.f,0.f};
	switch (m_CurMovDir)
	{
	case MovementDirection::DOWN:
		aimDir.y = -1;
		break;
	case MovementDirection::LEFT:
		aimDir.x = -1;
		break;
	case MovementDirection::RIGHT:
		aimDir.x = 1;
		break;
	case MovementDirection::UP:
		aimDir.y = 1;
		break;
	default:
		break;
	}
	if (abs(aimDir.x) < 0.001f && abs(aimDir.y) < 0.001f)
	{
		aimDir.x = 1.f;
	}
	pRigidBody->SetDirection(Float2{ aimDir.x * bulletSpeed, aimDir.y * -bulletSpeed });

	auto bulletComp = std::make_shared<BulletComponent>(true, 1);
	bullet->AddComponent(bulletComp, "Bullet");
	bulletComp->SetOverlapEvent();
	SceneManager::GetInstance().GetActiveScene().Add(bullet);
}
void dae::Enemy::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Bullet")
	{
		if (other->GetParent()->GetComponent<BulletComponent>("Bullet")->GetEvil())
			return;
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