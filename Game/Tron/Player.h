#pragma once
#include "BaseComponent.h"
#include "RigidBodyComponent.h"
namespace dae
{
	class PlayerComponent final : public BaseComponent
	{
	public:
		virtual void Update(float deltaTime);
		virtual void FixedUpdate(float /*deltaTime*/) {};
		virtual void Render() const {};

		PlayerComponent(bool isEvil);
		virtual ~PlayerComponent();
		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;
		void SetOverlapEvent()
		{
			auto bindIng = std::bind(&PlayerComponent::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(bindIng);
		}
		void SetAimDir(Float2 aimDir) { m_AimDirection = aimDir; }
		//Float2 GetAimDir() const { return m_AimDirection; }

		void Shoot();
	private:

		void OnOverlap(RigidBodyComponent* other);

		Float2 m_AimDirection;
	};
}