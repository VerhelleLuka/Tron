#pragma once
#include <iostream>
#include "GameObject.h"
#include "SpriteComponent.h"
#include "RigidBodyComponent.h"
namespace dae
{
	class Command {
	public:
		virtual ~Command() = default;

		virtual void Execute() = 0;
		virtual void SetGameObject(GameObject* pGo) { m_pGameObject = pGo; }
		virtual GameObject* GetGameObject() { return m_pGameObject; }
	protected:
		GameObject* m_pGameObject;
	};
}