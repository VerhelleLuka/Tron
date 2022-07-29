#include "Teleporter.h"
#include "GameManager.h"
dae::TeleporterComponent::TeleporterComponent()
{}

dae::TeleporterComponent::~TeleporterComponent()
{}

void dae::TeleporterComponent::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Player" || other->GetParent()->GetTag() == "Enemy")
	{
		int randomXPos = 0;
		int randomYPos = 0;

		while (GameManager::GetInstance().GetGridBlock(Float2{ static_cast<float>(randomXPos), static_cast<float>(randomYPos) }).hasBlock)
		{
			randomXPos = (std::rand() % (m_Width - m_TopLeft + 1) + m_TopLeft);
			randomYPos = (std::rand() % (m_Height - m_TopLeft + 1) + m_TopLeft);
		}

		other->GetParent()->SetTransform(static_cast<float>(randomXPos), static_cast<float>(randomYPos), 0.f);
	}
}	