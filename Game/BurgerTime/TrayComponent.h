#pragma once
#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
#include "RigidBodyComponent.h"
#include "PlatformComponent.h"
#include "PeterPepper.h"
namespace dae
{
	class TrayComponent final : public BaseComponent
	{
	public:
		virtual void Update(float /*deltaTime*/) override {};
		virtual void FixedUpdate(float /*deltaTime*/)override {};
		virtual void Render() const override {};
		TrayComponent() = default;
		virtual ~TrayComponent()
		{}
		TrayComponent(const TrayComponent& other) = delete;
		TrayComponent(TrayComponent&& other) = delete;
		TrayComponent& operator=(const TrayComponent& other) = delete;
		TrayComponent& operator=(TrayComponent&& other) = delete;
		void SetOverlapEvent()
		{
			auto binding = std::bind(&TrayComponent::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(binding);
		}
	protected:
		void OnOverlap(RigidBodyComponent* other);
	};
}

