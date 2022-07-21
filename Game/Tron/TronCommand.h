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
			m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->Shoot();
		}
	};

	class Aim final : public Command
	{
		void Execute() override
		{
			float angle = atan2f(InputManager::GetInstance().GetRStickValues().y, InputManager::GetInstance().GetRStickValues().x);

			Float2 direction{ cosf(angle), sinf(angle) };
			if(!(abs(direction.x) < 0.0001 && abs(direction.y) < 0.0001))
				m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->SetAimDir(direction);
		}
	};

	class Move final : public Command
	{
	private :
		MovementDirection m_MovDir;

	public:
		Move(MovementDirection movementDirection): m_MovDir(movementDirection){}

		void Execute() override
		{
			m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->Move(m_MovDir);
		}
	};

	class Die final : public Command
	{
		void Execute() override
		{
			m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->Die();
			GameManager::GetInstance().ReduceLife(false);

		}
	};

	class Select final : public Command
	{
	public:
		void Execute() override
		{
			//if (SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
				m_pGameObject->GetComponent<PlayerComponent>("PlayerComp")->ButtonPress();
		}
	};
}