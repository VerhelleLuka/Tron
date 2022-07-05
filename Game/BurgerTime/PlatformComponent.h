#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
namespace dae
{
	class PlatformComponent final: public BaseComponent 
	{
	public:
		virtual void Update(float /*deltaTime*/) override {};
		virtual void FixedUpdate(float /*deltaTime*/)override {};
		virtual void Render() const override {};

		PlatformComponent(): m_HasNext(false), m_HasPrevious(false) {};
		virtual ~PlatformComponent() {};
		PlatformComponent(const PlatformComponent& other) = delete;
		PlatformComponent(PlatformComponent&& other) = delete;
		PlatformComponent& operator=(const PlatformComponent& other) = delete;
		PlatformComponent& operator=(PlatformComponent&& other) = delete;
		void SetHasNext(bool hasNext) { m_HasNext = hasNext; }
		void SetHasPrevious(bool hasPrevious) { m_HasPrevious = hasPrevious; }

		bool GetHasNext() { return m_HasNext; }
		bool GetHasPrevious() { return m_HasPrevious; }
	protected:
		bool m_HasNext;
		bool m_HasPrevious;
	};
}
