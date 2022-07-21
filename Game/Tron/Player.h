#pragma once
#include "BaseComponent.h"
#include "RigidBodyComponent.h"
#include "TronStructs.h"
#include "Subject.h"
namespace dae
{
	class PlayerComponent final : public BaseComponent, public Subject
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
		void SetOnTriggerExitEvent()
		{
			auto bindIng = std::bind(&PlayerComponent::OnTriggerExit, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnTriggerExit(bindIng);
		}
		void SetAimDir(Float2 aimDir) { m_AimDirection = aimDir; }
		//Float2 GetAimDir() const { return m_AimDirection; }
		void ButtonPress();
		void Shoot();
		void Die();
		void Move(MovementDirection movDir);
	private:
		enum class PlayerState
		{
			Dead,
			Win,
			Alive
		};
		PlayerState m_PlayerState = PlayerState::Alive;
		void OnOverlap(RigidBodyComponent* other);
		void OnTriggerExit(RigidBodyComponent* other);

		Float2 m_AimDirection;
		const float m_MoveSpeed = 100.f;
		bool m_OverlappingButton;
	};
}