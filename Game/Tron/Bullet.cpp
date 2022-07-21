#include "Bullet.h"
#include "GameManager.h"
dae::BulletComponent::BulletComponent()
	:m_Bounces(5),
	m_BounceTimer(0.f),
	m_Bounced(false),
	m_Destroy(false)
{

}

dae::BulletComponent::~BulletComponent() {}

void dae::BulletComponent::Update(float elapsedSec)
{
	if (!m_Bounced)
		CheckBounce();
	else
	{
		m_BounceTimer += elapsedSec;
		if (m_BounceTimer >= m_BounceTime)
		{
			m_BounceTimer = 0;
			m_Bounced = false;
		}
	}

}

void dae::BulletComponent::CheckBounce()
{
	Float2 centerPoint = { m_pParent->GetTransform().GetPosition().x,m_pParent->GetTransform().GetPosition().y };
	float halfWidth = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth() / 2.f;
	float halfHeight = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() / 2.f;
	centerPoint.x += halfWidth;
	centerPoint.y += halfHeight;
	Float2 direction = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetDirection();
	auto& gameManager = GameManager::GetInstance();


	if (gameManager.GetGridBlock(Float2{ (centerPoint.x - halfWidth), centerPoint.y }) ||
		gameManager.GetGridBlock(Float2{ (centerPoint.x + halfWidth), centerPoint.y }))
	{
		m_Bounced = true;
		direction.x *= -1;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y - halfHeight) }) ||
		gameManager.GetGridBlock(Float2{ centerPoint.x, (centerPoint.y + halfHeight) }))

	{
		m_Bounced = true;
		direction.y *= -1;
	}
	else
	{
		m_Bounced = false;
	}
	m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(direction);

}
void dae::BulletComponent::FixedUpdate(float /*elapsedSec*/)
{
	if (m_Bounced)
	{
		m_Bounced = false;
		m_Bounces--;
		if (m_Bounces < 1)
		{
			m_pParent->MarkForDelete();
		}
	}
	if (m_Destroy)
	{
		m_pParent->MarkForDelete();
	}

}

void dae::BulletComponent::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Enemy")
	{
		m_Destroy = true;
	}
}