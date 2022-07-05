#include "MiniginPCH.h"
#include "PeterPepper.h"
#include "PointsDisplayComponent.h"
#include "SpriteComponent.h"
#include "Sound.h"
#include "PlatformComponent.h"
#include "Animation.h"
#include "LadderComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "Enemy.h"
dae::PeterPepperComponent::PeterPepperComponent(Float2 spawnPos, bool isEvil)
	:m_State(PeterPepperState::Climb)
	, m_CanClimb(false)
	, m_CanDescend(false)
	, m_CanWalkLeft(false)
	, m_CanWalkRight(false)
	, m_InMenu(true)
	, m_OverlappingButton(false)
	, m_VictoryDance(false)
	, m_VictoryDanceTime(0.0f)
	, m_SpawnPos(spawnPos)
	, m_IsDead(false)
	, m_IsEvil(isEvil)
	, m_PepperShots(10)
	, m_Stunned(false)
	, m_StunTime(0.f)
{
	if (SceneManager::GetInstance().GetActiveSceneName() != "MainMenu")
		GameManager::GetInstance().AddObserver(this);
}

dae::PeterPepperComponent::~PeterPepperComponent()
{
	GameManager::GetInstance().RemoveObserver(this);

}

void dae::PeterPepperComponent::Initialize(Scene& /*scene*/)
{
}

void dae::PeterPepperComponent::Update(float elapsedSec)
{
	if (m_Stunned)
	{
		m_StunTime += elapsedSec;
		if (m_StunTime > m_StunTimer)
		{
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetStatic(false);
			m_Stunned = false;
			m_StunTime = 0.f;
		}
	}
	if (m_VictoryDance)
	{
		m_VictoryDanceTime += elapsedSec;
		if (m_VictoryDanceTime >= m_VictoryDanceTimer)
		{
			m_VictoryDance = false;
			m_VictoryDanceTime = 0.f;

			GameManager::GetInstance().LoadLevel("Not Same");
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetStatic(false);
		}
	}
	if (m_IsDead)
	{
		if (!m_IsEvil)
		{
			if (m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetFrameNr() == m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetNrFrames() - 1)
			{
				m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().SetFrameNr(0);
				m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetStatic(false);
				m_IsDead = false;

				if (GameManager::GetInstance().GetLives() == 0)
				{
					
					GameManager::GetInstance().ResetScene(true);
				}
				else
				{
					GameManager::GetInstance().ResetScene(false);

				}
			}
		}
		else if (m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetFrameNr() == m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetNrFrames() - 1)
		{
			m_IsDead = false;
			GameManager::GetInstance().ResetScene(true);
		}


	}


	if (!m_CanClimb && !m_CanDescend && !m_CanWalkLeft && !m_CanWalkRight)
	{
		m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->Reverse(elapsedSec);
	}

	if (!m_InMenu)
	{
		return;
	}

	m_CanWalkLeft = true;
	m_CanWalkRight = true;
	m_CanClimb = true;
	m_CanDescend = true;
	if (m_pParent->GetTransform().GetPosition().x < 0)
	{
		m_CanWalkLeft = false;
	}
	//640 is the window width
	//480 is the platform height
	if (m_pParent->GetTransform().GetPosition().x + m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth() > 640)
	{
		m_CanWalkRight = false;
	}
	if (m_pParent->GetTransform().GetPosition().y < 0)
	{
		m_CanClimb = false;
	}
	if (m_pParent->GetTransform().GetPosition().y + m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() > 480)
	{
		m_CanDescend = false;
	}
}

void dae::PeterPepperComponent::FixedUpdate(float /*elapsedSec*/)
{
	if (m_InMenu)
		return;
	auto overlappingBodies = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetOverlappingBodies();
	bool isOverlappingPlatform = false;
	for (size_t i{}; i < overlappingBodies.size(); ++i)
	{
		if (overlappingBodies[i]->GetWidth() < -1000 || overlappingBodies[i]->GetWidth() > 1000)
		{

		}
		else if (overlappingBodies[i]->GetWidth() > -0.1f && overlappingBodies[i]->GetWidth() < 0.1f)
		{

		}
		else if (overlappingBodies[i]->GetParent()->GetMarkForDelete())
		{

		}
		else if (overlappingBodies[i]->GetParent()->GetTag() == "Platform")
		{
			isOverlappingPlatform = true;
		}
	}
	if (!isOverlappingPlatform)
	{
		m_CanWalkLeft = false;
		m_CanWalkRight = false;
	}
}

void dae::PeterPepperComponent::UsePepper()
{
	m_PepperShots--;
	std::shared_ptr<EventArgs> emptyArgs = std::make_shared<EventArgs>();
	Notify(EventType::SHOOT, emptyArgs);
}

void dae::PeterPepperComponent::ReduceLife()
{
	//if (GameManager::GetInstance().GetLives() > 0)
	//{
		GameManager::GetInstance().ReduceLife(false);
		m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetStatic(true);
		m_pParent->GetComponent<SpriteComponent>("Sprite")->SetActiveAnimation("Death");
		m_IsDead = true;
	//}

}
void dae::PeterPepperComponent::OnNotify(EventType event_, std::shared_ptr<EventArgs> /*args*/)
{
	if (event_ == EventType::WIN && !m_VictoryDance)
	{
		if (SceneManager::GetInstance().GetActiveSceneName() != "MainMenu")
		m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetStatic(true);
		m_pParent->GetComponent<SpriteComponent>("Sprite")->SetActiveAnimation("Victory");
		m_VictoryDance = true;
	}
}


void dae::PeterPepperComponent::ChangeState(int state = 0)
{
	m_State = static_cast<PeterPepperState>(state);
	std::shared_ptr<SpriteEventArgs> args = std::make_shared<SpriteEventArgs>();
	if (m_VictoryDance || m_IsDead || m_Stunned)
	{
		return;
	}
	switch (m_State)
	{

	case PeterPepperState::RunRight:
		args->name = "RunRight";
		break;
	case PeterPepperState::RunLeft:
		args->name = "RunLeft";
		break;
	case PeterPepperState::Climb:
		args->name = "Climb";
		break;
	case PeterPepperState::Descend:
		args->name = "Descend";
		break;
	default:
		args->name = "Idle";
		break;
	}
	Notify(EventType::STATECHANGED, args);

}
void dae::PeterPepperComponent::ButtonPress()
{
	if (m_OverlappingButton)
	{
		std::shared_ptr<EventArgs> emptyArgs = std::make_shared<EventArgs>();
		Notify(EventType::LOADLEVEL, emptyArgs);
	}
}
void dae::PeterPepperComponent::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent())
	{
		if (other->GetParent()->GetTag() == "Button")
		{
			other->GetParent()->GetComponent<ButtonComponent>("ButtonComp")->SetOverlapping(true);
			m_OverlappingButton = true;
			return;
		}
		//if the other overlap is a platform
		if (other->GetParent()->GetTag() == "Platform")
		{
			m_CanWalkLeft = false;
			m_CanWalkRight = false;
			Float2 platformPos = { other->GetTransform().GetPosition().x, other->GetTransform().GetPosition().y };
			float platformWidth = other->GetWidth();
			if (m_pParent->GetTransform().GetPosition().y + m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() - 5 < platformPos.y)
				m_CanWalkLeft = true;


			//IF there is no platform left to this one
			if (!other->GetParent()->GetComponent<PlatformComponent>("PlatformComp")->GetHasPrevious())
			{
				//And the overlap is at its end
				if (m_pParent->GetTransform().GetPosition().x < platformPos.x)
				{
					m_CanWalkLeft = false;
				}
			}
			if (m_pParent->GetTransform().GetPosition().y + m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() - 5 < platformPos.y)
				m_CanWalkRight = true;

			//IF there is no platform left to this one
			if (!other->GetParent()->GetComponent<PlatformComponent>("PlatformComp")->GetHasNext())
			{
				//And the overlap is at its end
				if ((m_pParent->GetTransform().GetPosition().x + m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth()) >= (platformPos.x + (platformWidth)))
				{
					m_CanWalkRight = false;
				}
			}
			return;
		}
		if (other->GetParent()->GetTag() == "Enemy" && !m_IsEvil && !m_IsDead)
		{
			if (!other->GetParent()->GetComponent<Enemy>("Enemy")->GetFalling() &&
				!other->GetParent()->GetComponent<Enemy>("Enemy")->GetDead() &&
				!other->GetParent()->GetComponent<Enemy>("Enemy")->GetStunned() &&
				!m_VictoryDance)
				ReduceLife();
			return;
		}
		else if (other->GetParent()->GetTag() == "EvilPeterPepper" && !m_IsEvil && !m_IsDead)
		{
			if(!other->GetParent()->GetComponent<PeterPepperComponent>("EvilPeterPepper")->GetStunned())
			ReduceLife();
		}

		//if it's a ladder
		if (other->GetParent()->GetTag() == "Ladder")
		{
			m_CanClimb = false;
			m_CanDescend = false;
			Float2 ladderPos = { other->GetTransform().GetPosition().x, other->GetTransform().GetPosition().y };
			float ladderHeight = other->GetHeight();
			//8 is a ladder offset
			if (m_pParent->GetTransform().GetPosition().y >= (ladderPos.y - ladderHeight / 2) - 8 ||
				other->GetParent()->GetComponent<LadderComponent>("LadderComp")->GetHasUp())
			{
				m_CanClimb = true;
			}
			if (m_pParent->GetTransform().GetPosition().y +
				m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() +
				m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetOffset().y <= ladderPos.y + ladderHeight)
			{
				m_CanDescend = true;
			}
		}
		if (other->GetParent()->GetTag() == "Pepper" && m_IsEvil)
		{
			if (!m_Stunned)
			{
				m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetStatic(true);
				m_Stunned = true;
				m_pParent->GetComponent<SpriteComponent>("Sprite")->SetActiveAnimation("Stunned");
			}
		}
		return;


	}
	else
	{
		m_CanDescend = false;
		m_CanClimb = false;
	}
}

void dae::PeterPepperComponent::OnTriggerExit(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Ladder")
	{
		m_CanDescend = false;
		m_CanClimb = false;
	}
	if (other->GetParent()->GetTag() == "Button")
	{
		other->GetParent()->GetComponent<ButtonComponent>("ButtonComp")->SetOverlapping(false);
		m_OverlappingButton = false;
	}
}