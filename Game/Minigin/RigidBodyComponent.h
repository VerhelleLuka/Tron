#pragma once
#include "BaseComponent.h"
#include "Subject.h"
#include "Structs.h"
#include "Observer.h"
#include <functional>
namespace dae
{
	class RigidBodyComponent final : public BaseComponent
	{
	public:
		virtual void Update(float deltaTime) override;
		virtual void FixedUpdate(float deltaTime) override;
		virtual void Render() const {};

		RigidBodyComponent();
		//RigidBodyComponent(RigidBodyComponent* rb);
		RigidBodyComponent(float width, float height, bool isTrigger);
		virtual ~RigidBodyComponent() override;

		void SetDirection(Float2 velocity);

		//All the getters for component overlap
		bool GetTrigger() const { return m_IsTrigger; }
		Transform GetTransform() const { return m_pParent->GetTransform(); }
		float GetWidth()const { return m_Width; }
		float GetHeight()const { return m_Height; }
		GameObject* GetParent() const { return m_pParent; }

		void SetOnOverlapEvent(const std::function<void(RigidBodyComponent*)>& function)
		{
			m_OverlapEvent = function;
		}
		void SetOnTriggerExit(const std::function<void(RigidBodyComponent*)>& function)
		{
			m_OnTriggerExit = function;
		}
		void OnOverlap(RigidBodyComponent* other);
		void OnTriggerExit(RigidBodyComponent* other);
		Float2 GetOffset() const { return m_PositionOffset; }
		void SetOffset(Float2 offset) { m_PositionOffset = offset; }

		void Reverse(float elapsedSec);

		void AddOverlappingBody(RigidBodyComponent* overlappingBody);
		void RemoveOverlappingBody(RigidBodyComponent* overlappingBody);

		std::vector<RigidBodyComponent*> GetOverlappingBodies() const;

		void SetStatic(bool isStatic) { m_IsStatic = isStatic;}
		bool GetStatic() const { return m_IsStatic; }
	protected:

		virtual void Initialize(Scene& scene) override;
		//m_Direction is for non physics-related movement
		Float2 m_Direction;
		Float2 m_LastDirection;

		//For triggerbox
		float m_Width, m_Height;
		//Position offset
		Float2 m_PositionOffset;
		bool m_IsTrigger;

		//Function pointers
		std::function<void(RigidBodyComponent*)> m_OverlapEvent;
		std::function<void(RigidBodyComponent*)> m_OnTriggerExit;
		std::vector<RigidBodyComponent*> m_OverlappingBodies;

		bool m_IsStatic;
	};
}