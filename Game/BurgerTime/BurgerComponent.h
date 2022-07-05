#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
#include "RigidBodyComponent.h"
#include "PlatformComponent.h"
#include "PeterPepper.h"
namespace dae
{
	class BurgerComponent final : public BaseComponent, public Subject
	{
	public:
		virtual void Update(float /*deltaTime*/) override {};
		virtual void FixedUpdate(float /*deltaTime*/)override;
		virtual void Render() const override {};
		void Initialize();
		BurgerComponent();
		virtual ~BurgerComponent() {}
		
		BurgerComponent(const BurgerComponent& other) = delete;
		BurgerComponent(BurgerComponent&& other) = delete;
		BurgerComponent& operator=(const BurgerComponent& other) = delete;
		BurgerComponent& operator=(BurgerComponent&& other) = delete;
		void SetOverlapEvent()
		{
			auto binding = std::bind(&BurgerComponent::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(binding);
		}
		void ForceFall();
		bool GetCaught() const { return m_Caught; }
		void SetCaught(bool caught);

	protected:
		static const int m_NrParts = 4;
		bool m_WalkedOver[m_NrParts] = { false };
		bool m_Fall;
		bool m_StartFall;
		bool m_Caught;
		float m_xPositions[m_NrParts];
		std::shared_ptr<PlatformComponent> m_pPlatformComp;
		void OnOverlap(RigidBodyComponent* other);
		int m_LevelsToFall;

	};
}

