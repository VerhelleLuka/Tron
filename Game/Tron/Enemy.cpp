#pragma once
#include "RigidBodyComponent.h"
#include "BaseComponent.h"
#include "EnemyState.h"
#include "PeterPepper.h"
#include "Scene.h"
namespace dae
{
	class LadderComponent;
	enum class EnemyType
	{
		TANK,
		RECOGNIZER
	};
	class Enemy final : public BaseComponent, public Subject
	{
	public:
		virtual void Update(float /*deltaTime*/) override;
		virtual void FixedUpdate(float /*deltaTime*/) override;
		virtual void Render() const {};
		void Initialize(Scene& scene) override;
		Enemy(EnemyType type);
		virtual ~Enemy() {};
		Enemy(const Enemy& other) = delete;
		Enemy(Enemy&& other) = delete;
		Enemy& operator=(const Enemy& other) = delete;
		Enemy& operator=(Enemy&& other) = delete;


		void SetOverlapEvent()
		{
			auto bindIng = std::bind(&Enemy::OnOverlap, this, std::placeholders::_1);
			m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetOnOverlapEvent(bindIng);
		}
		void Kill();

	private:
		void OnOverlap(RigidBodyComponent* other);
	};
}