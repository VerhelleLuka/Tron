//#pragma once
#include "GameObject.h"
#include "Command.h"
#include "PeterPepper.h"
#include "Structs.h"
#include "SceneManager.h"
#include "SceneObject.h"
#include "Scene.h"
#include <string>
#include "PepperComponent.h"
namespace dae
{
	class MoveRight final : public Command
	{
	public:
		void Execute() override
		{
			if (m_pGameObject)
			{
				if (m_pGameObject->GetTag() != "PeterPepper")
				{ }
				else if (m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->GetCanWalkRight())
				{
					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(75.f, 0.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->ChangeState(2);

				}
				if (m_pGameObject->GetTag() != "EvilPeterPepper")
					return;
				else if (GameManager::GetInstance().GetGameMode() == GameMode::VERSUS && m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->GetCanWalkRight())
				{
					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(50.f, 0.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->ChangeState(2);
				}
			}
		}
	};
	class MoveLeft final : public Command
	{
	public:
		void Execute() override
		{
			if (m_pGameObject)
			{
				if (m_pGameObject->GetTag() != "PeterPepper")
				{
				}
				else if (m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->GetCanWalkLeft())
				{

					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(-75.f, 0.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->ChangeState(1);
				}
				if (m_pGameObject->GetTag() != "EvilPeterPepper")
					return;
				else if (GameManager::GetInstance().GetGameMode() == GameMode::VERSUS && m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->GetCanWalkLeft())
				{

					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(-50.f, 0.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->ChangeState(1);
				}
			}
		}
	};

	class MoveUp final : public Command
	{
	public:
		void Execute() override
		{
			if (m_pGameObject)
			{
				if (m_pGameObject->GetTag() != "PeterPepper")
				{
				}
				else if (m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->GetCanClimb())
				{
					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(0.f, -75.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->ChangeState(3);
				}
				if (m_pGameObject->GetTag() != "EvilPeterPepper")
					return;
				else if (GameManager::GetInstance().GetGameMode() == GameMode::VERSUS && m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->GetCanClimb())
				{
					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(0.f, -50.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->ChangeState(3);
				}


			}

		}
	};
	class MoveDown final : public Command
	{
	public:
		void Execute() override
		{
			if (m_pGameObject)
			{
				if (m_pGameObject->GetTag() != "PeterPepper")
				{
				}
				else if (m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->GetCanDescend())
				{
					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(0.f, 75.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->ChangeState(4);
				}
				if (m_pGameObject->GetTag() != "EvilPeterPepper")
					return;
				else if (GameManager::GetInstance().GetGameMode() == GameMode::VERSUS && m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->GetCanDescend())
				{
				
					m_pGameObject->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2(0.f, 50.f));
					m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->ChangeState(4);
				}
			}
		}
	};
	class Idle final : public Command
	{
	public:
		void Execute() override
		{
			if (m_pGameObject)
			{
				m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->ChangeState(0);
				if (GameManager::GetInstance().GetGameMode() == GameMode::VERSUS && m_pGameObject->GetTag() == "EvilPeterPepper")
				{

					m_pGameObject->GetComponent<PeterPepperComponent>("EvilPeterPepper")->ChangeState(4);
				}

			}
		}
	};
	class Select final : public Command
	{
	public:
		void Execute() override
		{
			if (SceneManager::GetInstance().GetActiveSceneNr() == 0 && SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
				m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->ButtonPress();
		}
	};
	class Pepper final : public Command
	{
	public:
		void Execute() override
		{
			if (SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
			{
				return;
			}
			if (m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->GetNrOfPepperShots() <= 0)
			{
				return;
			}
			m_pGameObject->GetComponent<PeterPepperComponent>("PeterPepper")->UsePepper();
			float animationScale = 1.75f;

			auto pepperGo = std::make_shared<GameObject>();
			pepperGo->SetTag("Pepper");
			auto pepperAnimation = std::make_shared<Animation>(4, 4);
			pepperAnimation->SetTexture("PeterPepper/Pepper.png");
			pepperAnimation->SetScale(animationScale);

			auto pepperSprite = std::make_shared<SpriteComponent>();
			pepperGo->AddComponent(pepperSprite, "Sprite");
			pepperSprite->AddAnimation(pepperAnimation, "Pepper");
			pepperSprite->SetActiveAnimation("Pepper");

			auto pRigidBody = std::make_shared<RigidBodyComponent>(pepperSprite->GetAnimation().GetScaledWidth(),
				pepperSprite->GetAnimation().GetScaledHeight(),
				true);

			auto pepper = std::make_shared<PepperComponent>();
			pepperGo->AddComponent(pepper, "PepperComp");
			pepperGo->AddComponent(pRigidBody, "Pepper");

			if (m_pGameObject->GetComponent<SpriteComponent>("Sprite")->GetAnimationName() == "RunRight")
			{
				pepperGo->SetTransform(m_pGameObject->GetTransform().GetPosition().x + pepperAnimation->GetScaledWidth(), m_pGameObject->GetTransform().GetPosition().y, 0.f);

			}
			else if (m_pGameObject->GetComponent<SpriteComponent>("Sprite")->GetAnimationName() == "RunLeft")
			{
				pepperGo->SetTransform(m_pGameObject->GetTransform().GetPosition().x - pepperAnimation->GetScaledWidth(), m_pGameObject->GetTransform().GetPosition().y, 0.f);

			}
			else if (m_pGameObject->GetComponent<SpriteComponent>("Sprite")->GetAnimationName() == "Climb")
			{
				pepperGo->SetTransform(m_pGameObject->GetTransform().GetPosition().x, m_pGameObject->GetTransform().GetPosition().y - pepperAnimation->GetScaledHeight(), 0.f);

			}
			else if (m_pGameObject->GetComponent<SpriteComponent>("Sprite")->GetAnimationName() == "Descend")
			{
				pepperGo->SetTransform(m_pGameObject->GetTransform().GetPosition().x, m_pGameObject->GetTransform().GetPosition().y + pepperAnimation->GetScaledHeight(), 0.f);
			}
			else if (m_pGameObject->GetComponent<SpriteComponent>("Sprite")->GetAnimationName() == "Idle")
			{
				pepperGo->SetTransform(m_pGameObject->GetTransform());

			}
			SceneManager::GetInstance().GetActiveScene().Add(pepperGo);
		}
	};
}
