#include "MiniginPCH.h"
#include "RigidBodyComponent.h"
#include "Structs.h"
#include <algorithm>
#include "Physics.h"
#include "Scene.h"
#include "SceneManager.h"
#include "../BurgerTime/PeterPepper.h"
#include "../BurgerTime/PlatformComponent.h"
#include "LadderComponent.h"
#include "../BurgerTime/BurgerComponent.h"
dae::RigidBodyComponent::RigidBodyComponent(float width, float height, bool isTrigger)
	:m_Width(width),
	m_Height(height),
	m_IsTrigger(isTrigger),
	m_OverlapEvent(),
	m_PositionOffset(Float2{ 0.f,0.f })
	, m_Direction(Float2{0.f, 0.f})
	,m_LastDirection(Float2{0.f, 0.f})
	,m_IsStatic(false)
{
	if (&SceneManager::GetInstance().GetActiveScene())
	{
		Physics::GetInstance().SetSceneNr(SceneManager::GetInstance().GetActiveScene().GetIndex());
		Physics::GetInstance().AddRigidBodyComponent(this);
	}


}
void dae::RigidBodyComponent::Initialize(Scene& scene)
{
	Physics::GetInstance().SetSceneNr(scene.GetIndex());
	Physics::GetInstance().AddRigidBodyComponent(this);

}
//dae::RigidBodyComponent::RigidBodyComponent(RigidBodyComponent* rb)
//{
//}
////
void dae::RigidBodyComponent::SetDirection(Float2 direction)
{
	m_Direction = direction;
	m_LastDirection = direction;
}

void dae::RigidBodyComponent::Update(float /*elapsedSec*/)
{

}

dae::RigidBodyComponent::~RigidBodyComponent()
{

}


void dae::RigidBodyComponent::FixedUpdate(float elapsedSec)
{
	if (m_IsStatic)
		return;
	if (abs(m_Direction.x) > 0.01f || abs(m_Direction.y) > 0.01f)
	{
		m_pParent->SetTransform(m_pParent->GetTransform().GetPosition().x + m_Direction.x * elapsedSec,
			m_pParent->GetTransform().GetPosition().y + m_Direction.y * elapsedSec,
			m_pParent->GetTransform().GetPosition().z);
		m_Direction = { 0.f,0.f };
	}

}
void dae::RigidBodyComponent::Reverse(float elapsedSec)
{
	if (m_IsStatic)
		return;
	m_pParent->SetTransform(m_pParent->GetTransform().GetPosition().x + m_LastDirection.x * elapsedSec,
		m_pParent->GetTransform().GetPosition().y + m_LastDirection.y * elapsedSec,
		m_pParent->GetTransform().GetPosition().z);
}

void dae::RigidBodyComponent::OnOverlap(RigidBodyComponent* other)
{
	if (m_OverlapEvent != nullptr)
	{
		m_OverlapEvent(other);
	}
}

void dae::RigidBodyComponent::OnTriggerExit(RigidBodyComponent* other)
{
	if (m_OnTriggerExit != nullptr)
	{
		m_OnTriggerExit(other);
	}
}

void dae::RigidBodyComponent::AddOverlappingBody(RigidBodyComponent* overlappingBody)
{
	//std::weak_ptr<RigidBodyComponent> weak_RB = overlappingBody;

	auto it = std::find(m_OverlappingBodies.begin(), m_OverlappingBodies.end(), overlappingBody);
	//it = m_OverlappingBodies.end();
	if (it != m_OverlappingBodies.end())
	{
		return;
	}
	m_OverlappingBodies.push_back(overlappingBody);
}

void dae::RigidBodyComponent::RemoveOverlappingBody(RigidBodyComponent* overlappingBody)
{
	//std::weak_ptr<RigidBodyComponent> weak_RB = overlappingBody;

	auto it = std::find(m_OverlappingBodies.begin(), m_OverlappingBodies.end(), overlappingBody);
	if (it != m_OverlappingBodies.end())
	{
		OnTriggerExit(overlappingBody);
		m_OverlappingBodies.erase(std::remove(m_OverlappingBodies.begin(), m_OverlappingBodies.end(), *it), m_OverlappingBodies.end());
	}

}

std::vector<dae::RigidBodyComponent*> dae::RigidBodyComponent::GetOverlappingBodies() const
{
	return m_OverlappingBodies;
}