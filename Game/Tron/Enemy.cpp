#pragma once
#include "Enemy.h"
#include "GameManager.h"
#include "Bullet.h"

dae::Enemy::Enemy(EnemyType enemyType)
	:m_NrHits(3),
	m_Dead(false)
{
	m_MoveSpeed = 100;
	if (enemyType == EnemyType::RECOGNIZER)
		m_MoveSpeed = 200;
}

void dae::Enemy::FixedUpdate(float /* elapsedSec*/)
{
	if (m_Dead)
	{
		std::cout << "Enemy mark for delete\n";
		GameManager::GetInstance().EnemyKilled();
		m_pParent->MarkForDelete();
	}
}

void dae::Enemy::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Bullet")
	{
		m_NrHits--;

		if (m_NrHits == 0)
		{
			m_Dead = true;
		}
	}
}