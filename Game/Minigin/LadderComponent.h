#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
namespace dae
{
	class LadderComponent final : public BaseComponent
	{
	public:
		virtual void Update(float /*deltaTime*/) override {};
		virtual void FixedUpdate(float /*deltaTime*/)override {};
		virtual void Render() const override {};

		LadderComponent() : m_HasUp(false), m_HasDown(false) {};
		virtual ~LadderComponent() {};

		void SetHasUp(bool hasUp) { m_HasUp = hasUp; }
		void SetHasDown(bool hasDown) { m_HasDown = hasDown; }

		bool GetHasUp() { return m_HasUp; }
		bool GetHasDown() { return m_HasDown; }
	protected:
		bool m_HasUp;
		bool m_HasDown;
	};
}
