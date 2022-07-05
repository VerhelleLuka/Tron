#include "MiniginPCH.h"
#include "BurgerComponent.h"
#include "PeterPepper.h"
#include "SpriteComponent.h"
#include "Animation.h"
#include "PlatformComponent.h"
#include "Enemy.h"
dae::BurgerComponent::BurgerComponent()
	:m_Fall(false),
	m_Caught(false),
	m_LevelsToFall(0),
	m_StartFall(false)
{
}
void dae::BurgerComponent::Initialize()
{
	for (int i{}; i < m_NrParts; ++i)
	{
		m_xPositions[i] = m_pParent->GetTransform().GetPosition().x + (m_pParent->GetComponent<SpriteComponent>("BurgerSprite")->GetAnimation().GetScaledWidth() * i) / 5;
		m_WalkedOver[i] = false;
	}
	
}
void dae::BurgerComponent::FixedUpdate(float /*elapsedSec*/)
{
	if (m_Fall && !m_Caught)
	{
		m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2{ 0, 100 });
	}
	m_StartFall = false;

	if (m_Caught && GameManager::GetInstance().GetGameMode() == GameMode::VERSUS)
	{
		m_pParent->MarkForDelete();
	}
}

void dae::BurgerComponent::ForceFall()
{
	for (int i{}; i < m_NrParts - 1; ++i)
	{
		m_WalkedOver[i] = true;
	}
	m_Fall = true;
	m_StartFall = true;
}

void dae::BurgerComponent::SetCaught(bool caught)
{
	m_Caught = caught;
	if (caught)
	{
		std::shared_ptr<EventArgs> emptyArgs = std::make_shared<EventArgs>();
		Notify(EventType::BURGERDROPPED, emptyArgs);
	}
}
void dae::BurgerComponent::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Enemy")
	{
		if (other->GetTransform().GetPosition().y > m_pParent->GetTransform().GetPosition().y && m_Fall)
		{
			if (other->GetParent()->GetComponent<Enemy>("Enemy")->GetEnemyType() == EnemyType::SAUSAGE && !other->GetParent()->GetComponent<Enemy>("Enemy")->IsDead())
			{
					GameManager::GetInstance().AddPoints(100);

			}
			other->GetParent()->GetComponent<Enemy>("Enemy")->Kill();
		}
		if (!other->GetParent()->GetComponent<Enemy>("Enemy")->GetFalling())
		{
			if (other->GetTransform().GetPosition().y <= m_pParent->GetTransform().GetPosition().y && m_Fall && m_StartFall)
			{
				other->GetParent()->GetComponent<Enemy>("Enemy")->Fall();
				GameManager::GetInstance().AddPoints(500);
				m_LevelsToFall++;
			}
		}
	}
	if (other->GetParent()->GetTag() == "Platform")
	{
		if (m_pPlatformComp.get() == nullptr)
		{
			m_pPlatformComp = other->GetParent()->GetComponent<PlatformComponent>("PlatformComp");
		}
		if (m_Fall && other->GetParent()->GetComponent<PlatformComponent>("PlatformComp") != m_pPlatformComp)
		{
			m_pPlatformComp = other->GetParent()->GetComponent<PlatformComponent>("PlatformComp");
			m_Fall = false;

			GameManager::GetInstance().AddPoints(50);

			
			for (int i{}; i < m_NrParts - 1; ++i)
			{
				m_WalkedOver[i] = false;
			}
			if (m_LevelsToFall > 0)
			{
				ForceFall();
				m_LevelsToFall--;
			}
		}
	}
	if (other->GetParent()->GetTag() == "PeterPepper")
	{

		for (int i{}; i < m_NrParts - 1; ++i)
		{

			if (other->GetParent()->GetTransform().GetPosition().x - 4 > m_xPositions[i]
				&& other->GetParent()->GetTransform().GetPosition().x < m_xPositions[i + 1])
			{
				m_WalkedOver[i] = true;
				bool m_AllTrue = true;
				for (int j{}; j < m_NrParts - 1; ++j)
				{
					if (m_WalkedOver[j] == false)
					{
						m_AllTrue = false;
					}
				}
				m_Fall = m_AllTrue;
				m_StartFall = m_AllTrue;
			}

		}

	}
	if (other->GetParent()->GetTag() == "EvilPeterPepper")
	{
		if (other->GetTransform().GetPosition().y > m_pParent->GetTransform().GetPosition().y && m_Fall)
		{
			if (!other->GetParent()->GetComponent<PeterPepperComponent>("EvilPeterPepper")->GetIsDead())
			{
				other->GetParent()->GetComponent<PeterPepperComponent>("EvilPeterPepper")->ReduceLife();
			}
		}
	}
	if (other->GetParent()->GetTag() == "Burger")
	{
		if (other->GetParent()->GetComponent<BurgerComponent>("BurgerComp")->GetCaught())
		{
			if (m_pParent->GetComponent<SpriteComponent>("BurgerSprite")->GetAnimationName() == "Top_bun" && !m_Caught)
			{
				GameManager::GetInstance().BurgerCompleted();
			}
			m_Caught = true;

		}
		else if (other->GetParent()->GetTransform().GetPosition().y < m_pParent->GetTransform().GetPosition().y)
		{
			ForceFall();
		}
		else if(m_Fall)
		{
			other->GetParent()->GetComponent<BurgerComponent>("BurgerComp")->ForceFall();
		}
	}
}

