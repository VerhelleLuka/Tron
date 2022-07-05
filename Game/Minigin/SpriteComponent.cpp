#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "Structs.h"
#include "Renderer.h"
#include "Animation.h"
void dae::SpriteComponent::SetActiveAnimation(const std::string& name)
{
	m_pActiveAnimation.reset();
	m_pActiveAnimation = m_pAnimations[name];

	m_pActiveAnimationName = name;

	//Update all animation positions to prevent weird teleporting
	for (auto& element : m_pAnimations)
	{
		element.second->Update(0.f, m_pParent->GetTransform());
	}
}

void dae::SpriteComponent::Update(float elapsedSec)
{
	if (m_pActiveAnimation != nullptr)
		m_pActiveAnimation->Update(elapsedSec, m_pParent->GetTransform());
}

void dae::SpriteComponent::Render() const
{
	if (m_pActiveAnimation != nullptr)
		m_pActiveAnimation->Render();
}

void dae::SpriteComponent::AddAnimation(std::shared_ptr<Animation> animation,const std::string& name)
{
	m_pAnimations[name] = animation;

}

void dae::SpriteComponent::OnNotify(EventType event_, std::shared_ptr<EventArgs> args)
{
	if (event_ == EventType::STATECHANGED)
	{
		std::string name = std::static_pointer_cast<SpriteEventArgs>(args)->name;
		SetActiveAnimation(name);
	}
}