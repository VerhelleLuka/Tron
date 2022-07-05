#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
#include <Structs.h>
#include "Observer.h"
namespace dae
{
	//struct BurgerRemovedArgs : EventArgs {
	//	EnemyType enemyType;
	//};
	class Scene;
	class BurgerSpawner final : public BaseComponent, public Observer
	{
	public:
		virtual void Update(float deltaTime) override;
		virtual void FixedUpdate(float /*deltaTime*/) override {};
		virtual void Render() const override {};

		BurgerSpawner();
		virtual ~BurgerSpawner() {};
		BurgerSpawner(const BurgerSpawner& other) = delete;
		BurgerSpawner(BurgerSpawner&& other) = delete;
		BurgerSpawner& operator=(const BurgerSpawner& other) = delete;
		BurgerSpawner& operator=(BurgerSpawner&& other) = delete;
		virtual void Initialize(Scene& /*scene*/) override {};

		void SetSpawnPositions(std::vector<Float2> spawnPositions) { m_SpawnPositions = spawnPositions; }
		virtual void OnNotify(EventType /*event_*/, std::shared_ptr<EventArgs> /*args*/) override;

	protected:

		void SpawnBurger();
		int m_MaxBurgers;
		int m_NumBurgers;
		const float m_BurgerRespawnTimer {5.f};
		float m_BurgerRespawnTime;

		std::vector<Float2> m_SpawnPositions;
		std::vector<std::string> m_BurgerSpriteNames;

	};
}
