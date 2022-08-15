#pragma once
#include "GameObject.h"
#include "Command.h"
#include "Player.h"

namespace dae
{

	class Shoot final : public Command
	{
	public:
		void Execute() override
		{
			if (m_pGameObject == nullptr || SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
				return;
			if (m_pGameObject->GetComponent<PlayerComponent>("PlayerComp"))
			{
				float angle = 0;

				if (InputManager::GetInstance().GetAimingWithController())
					angle = atan2f(InputManager::GetInstance().GetRStickValues(m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->GetPlayerIndex()).y, InputManager::GetInstance().GetRStickValues(m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->GetPlayerIndex()).x);

				else
				{
					angle = atan2f((InputManager::GetInstance().GetMousePos().y - m_pGameObject->GetTransform().GetPosition().y) * -1,
						(InputManager::GetInstance().GetMousePos().x - m_pGameObject->GetTransform().GetPosition().x));
				}

				Float2 direction{ cosf(angle), sinf(angle) };
				if (!(abs(direction.x) < 0.0001 && abs(direction.y) < 0.0001))
					m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->SetAimDir(direction);

				m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->Shoot();

			}
		}
	};
	class SkipLevel final : public Command
	{
		void Execute() override
		{
			GameManager::GetInstance().LoadLevel("zfreoipj");
		}
	};
	//class Aim final : public Command
	//{
	//	void Execute() override
	//	{
	//		if (m_pGameObject == nullptr)
	//			return;
	//		if (m_pGameObject->GetComponent<PlayerComponent>("PlayerComp"))
	//		{

	//		}


	//	}
	//};

	class Move final : public Command
	{
	private:
		MovementDirection m_MovDir;

	public:
		Move(MovementDirection movementDirection) : m_MovDir(movementDirection) {}

		void Execute() override
		{
			if (m_pGameObject)
				m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->Move(m_MovDir);
		}
	};

	class Die final : public Command
	{
		void Execute() override
		{
			if (m_pGameObject == nullptr)
				return;
			if (m_pGameObject->GetComponent<PlayerComponent>("PlayerComp"))
			{
				m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->Die();
				GameManager::GetInstance().ReduceLife(false);
			}


		}
	};

	class Select final : public Command
	{
	public:
		void Execute() override
		{
			if (m_pGameObject == nullptr)
				return;
			if (m_pGameObject->GetComponent<PlayerComponent>("PlayerComp"))
				m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->ButtonPress();
		}
	};
	//class SwitchAimDevice final : public Command
	//{
	//public:
	//	void Execute() override
	//	{
	//		InputManager::GetInstance().SwistchAimDevice();
	//	}
	//};
}