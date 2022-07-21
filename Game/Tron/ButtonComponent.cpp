#include "ButtonComponent.h"
#include "SceneManager.h"
#include "TextComponent.h"
#include "GameManager.h"
void dae::ButtonComponent::OnNotify(EventType event_, std::shared_ptr<EventArgs> /*args*/)
{
	if (event_ == EventType::LOADLEVEL)
	{
		if (m_OverlappingPlayer)
		{
			GameManager::GetInstance().SetGameMode(m_GameMode);
			GameManager::GetInstance().LoadLevel(m_SceneName);
		}
	}
}