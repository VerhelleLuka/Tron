#include "Bullet.h"

dae::BulletComponent::BulletComponent()
	:m_Bounces(5),
	m_ChangeDirectionTimer(0.f),
	m_ChangedDir(false)
{

}

dae::BulletComponent::~BulletComponent() {}

void dae::BulletComponent::Update(float elapsedSec)
{
	if (m_ChangedDir)
	{
		m_ChangeDirectionTimer += elapsedSec;
		if (m_ChangeDirectionTimer >= m_ChangeDirectionTime)
		{
			m_ChangedDir = false;
			m_ChangeDirectionTimer = 0.f;
		}
	}
}
void dae::BulletComponent::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Block" && !m_ChangedDir)
	{
		Float2 centerPoint = { m_pParent->GetTransform().GetPosition().x,m_pParent->GetTransform().GetPosition().y };
		centerPoint.x += m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth() / 2.f;
		centerPoint.y -= m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() / 2.f;
		Float2 centerPointOther{ 0.f,0.f };
		centerPointOther.x = other->GetParent()->GetTransform().GetPosition().x + (other->GetWidth() / 2.f);
		centerPointOther.y = other->GetParent()->GetTransform().GetPosition().y - (other->GetHeight() / 2.f);
		
		float dX = centerPoint.x- centerPointOther.x  ;
		float dY = centerPoint.y - centerPointOther.y ;

		Float2 direction = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetDirection();

		if (abs(dY) > abs(dX))
		{
			direction.y *= -1;
			std::cout << "Y-reversed\n";
		}
		else
		{
			direction.x *= -1;
			std::cout << "X-reversed\n";

		}
		m_ChangedDir = true;
		m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(direction);
	}
}