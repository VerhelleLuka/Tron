#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
#include "Subject.h"
#include "EventStructs.h"
#include "Achievements.h"
#include "Structs.h"
#include "RigidBodyComponent.h"
#include "ButtonComponent.h"
namespace dae
{
	enum class PeterPepperState
	{
		RunLeft = 1,
		RunRight = 2,
		Climb = 3,
		Descend = 4
	};
	// Defining our achievements
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,

	};
	class PeterPepperComponent final : public BaseComponent, public Subject, public Observer
	{
	public:
		virtual void Update(float deltaTime);
		virtual void FixedUpdate(float /*deltaTime*/) override;
		virtual void Render() const {};
		virtual void Initialize(Scene& scene) override;
		PeterPepperComponent(Float2 m_SpawnPos, bool isEvil);
		virtual ~PeterPepperComponent();
		PeterPepperComponent(const PeterPepperComponent& other) = delete;
		PeterPepperComponent(PeterPepperComponent&& other) = delete;
		PeterPepperComponent& operator=(const PeterPepperComponent& other) = delete;
		PeterPepperComponent& operator=(PeterPepperComponent&& other) = delete;
		void ReduceLife();
		void ButtonPress();
		void ChangeState(int state);
		void UsePepper();
		int GetNrOfPepperShots() const { return m_PepperShots; }

		bool GetCanClimb() const { return m_CanClimb; }
		bool GetCanDescend() const { return m_CanDescend; }
		bool GetCanWalkLeft() const { return m_CanWalkLeft; }
		bool GetCanWalkRight() const { return m_CanWalkRight; }

		void SetOverlapEvent()
		{
			auto bindIng = std::bind(&PeterPepperComponent::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(bindIng);
		}
		void SetOnTriggerExitEvent()
		{
			auto bindIng = std::bind(&PeterPepperComponent::OnTriggerExit, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnTriggerExit(bindIng);
		}
		bool GetStunned() const { return m_Stunned; }
		bool GetInMenu() const { return m_InMenu; }
		void SetInMenu(bool inGameScene)  { m_InMenu = inGameScene; }

		virtual void OnNotify(EventType event_, std::shared_ptr<EventArgs> args) override;

		bool GetIsDead() const { return m_IsDead; }
	protected:
		PeterPepperState m_State;
	private:
		bool m_CanClimb;
		bool m_CanDescend;
		bool m_CanWalkLeft;
		bool m_CanWalkRight;

		bool m_InMenu;
		void OnOverlap(RigidBodyComponent* other);
		void OnTriggerExit(RigidBodyComponent* other);

		bool m_OverlappingButton;
		bool m_VictoryDance;
		const float m_VictoryDanceTimer{ 3.f };
		float m_VictoryDanceTime;
		bool m_IsDead;
		int m_PepperShots;

		bool m_IsEvil;

		Float2 m_SpawnPos;

		bool m_IsInvincible;
		const float m_InivincibilityTimer{3.f};
		float m_InvincibilityTime;

		bool m_Stunned;		
		const float m_StunTimer{ 3.f };
		float m_StunTime;

	};
}
