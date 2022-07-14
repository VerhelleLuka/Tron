#pragma once
#include "BaseComponent.h"
#include "RigidBodyComponent.h"
#include "TronStructs.h"
namespace dae
{
	class TeleporterComponent final : public BaseComponent
	{
	public:
		virtual void Update(float /*deltaTime*/) {};
		virtual void FixedUpdate(float /*deltaTime*/) {};
		virtual void Render() const {};

		TeleporterComponent();
		virtual ~TeleporterComponent();
		TeleporterComponent(const TeleporterComponent& other) = delete;
		TeleporterComponent(TeleporterComponent&& other) = delete;
		TeleporterComponent& operator=(const TeleporterComponent& other) = delete;
		TeleporterComponent& operator=(TeleporterComponent&& other) = delete;

		void SetOverlapEvent()
		{
			auto bindIng = std::bind(&TeleporterComponent::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(bindIng);
		}

	private:
		void OnOverlap(RigidBodyComponent* other);

		const int m_TopLeft = 0;
		const int m_Width = 624;
		const int m_Height = 432;
	};
}