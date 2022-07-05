#include "PointsDisplayComponent.h"
#include "TextComponent.h"
#include "GameManager.h"
dae::PointsDisplayComponent::~PointsDisplayComponent()
{
	GameManager::GetInstance().RemoveObserver(this);

}
void dae::PointsDisplayComponent::OnNotify(EventType event_, std::shared_ptr<EventArgs> args)
{
	if (event_ == EventType::GAINEDPOINTS)
	{
		int points = std::static_pointer_cast<PointsEventArgs>(args)->points;
		
		m_pParent->GetComponent<TextComponent>("TextComponent")->SetText("Points: " + std::to_string(points));
	}

}
