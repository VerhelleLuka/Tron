#include "ButtonComponent.h"
#include "SceneManager.h"
#include "TextComponent.h"
void dae::ButtonComponent::OnNotify(EventType event_, std::shared_ptr<EventArgs> /*args*/)
{
	if (event_ == EventType::LOADLEVEL)
	{
		if (m_OverlappingPeter)
		{
			GameManager::GetInstance().SetGameMode(m_GameMode);
			GameManager::GetInstance().LoadLevel(m_SceneName);
		}
	}
}