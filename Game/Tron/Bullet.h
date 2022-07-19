#pragma once
#include "BaseComponent.h"
#include "RigidBodyComponent.h"
namespace dae
{
	class BulletComponent final : public BaseComponent
	{
	public:
		virtual void Update(float deltaTime);
		virtual void FixedUpdate(float /*deltaTime*/) ;
		virtual void Render() const {};

		BulletComponent();
		virtual ~BulletComponent();
		BulletComponent(const BulletComponent& other) = delete;
		BulletComponent(BulletComponent&& other) = delete;
		BulletComponent& operator=(const BulletComponent& other) = delete;
		BulletComponent& operator=(BulletComponent&& other) = delete;
		void SetOverlapEvent()
		{
			auto bindIng = std::bind(&BulletComponent::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(bindIng);
		}

	private:
		void OnOverlap(RigidBodyComponent* other);
		void CheckBounce();
		int m_Bounces;
		
		bool m_Bounced;
		float m_BounceTimer;
		const float m_BounceTime = 0.1f;
		bool m_Destroy;

	};
}