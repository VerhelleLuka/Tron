#pragma once
#include "GameObject.h"
#include <string>
#include "BaseComponent.h"
#include "Structs.h"
#include "Observer.h"
namespace dae
{
	struct SpriteEventArgs : EventArgs
	{
		std::string name;
	};
	class Animation;
	class SpriteComponent final : public BaseComponent, public Observer
	{
	public:
		SpriteComponent() {};
		~SpriteComponent() = default;

		SpriteComponent(const SpriteComponent& rhs) = delete;
		SpriteComponent(SpriteComponent&& rhs) noexcept = delete;

		SpriteComponent& operator=(SpriteComponent&& rhs) noexcept = delete;
		SpriteComponent operator=(const SpriteComponent& rhs) = delete;

		virtual void Update(float deltaTime) override;
		virtual void FixedUpdate(float /*deltaTime*/) override {}

		virtual void Render() const;

		void SetActiveAnimation(const std::string& name);
		Animation& GetAnimation() const { return *m_pActiveAnimation; }
		std::string GetAnimationName() const { return m_pActiveAnimationName; }

		void AddAnimation(std::shared_ptr<Animation> animation, const std::string& name);
		//Observer stuff
		virtual void OnNotify(EventType event_, std::shared_ptr<EventArgs> args) override;

	private:
		std::map<std::string, std::shared_ptr<Animation>> m_pAnimations;
		std::shared_ptr<Animation> m_pActiveAnimation;
		std::string m_pActiveAnimationName;	

	};
}

